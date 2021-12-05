// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "precomp.h"

#include "CommandListPopup.hpp"
#include "stream.h"
#include "_stream.h"
#include "cmdline.h"
#include "misc.h"
#include "_output.h"
#include "dbcs.h"
#include "../types/inc/GlyphWidth.hpp"

#include "../interactivity/inc/ServiceLocator.hpp"

static constexpr size_t COMMAND_NUMBER_SIZE = 8; // size of command number buffer

// Routine Description:
// - Calculates what the proposed size of the popup should be, based on the commands in the history
// Arguments:
// - history - the history to look through to measure command sizes
// Return Value:
// - the proposed size of the popup with the history list taken into account
static til::coord calculatePopupSize(const CommandHistory& history)
{
    // this is the historical size of the popup, so it is now used as a minimum
    static constexpr til::vec2<size_t> minSize{ 40, 10 };

    // padding is for the command number listing before a command is printed to the window.
    // ex: |10: echo blah
    //      ^^^^ <- these are the cells that are being accounted for by padding
    static constexpr size_t padding = 4;

    // find the widest command history item and use it for the width
    const auto& commands = history.GetCommands();
    auto width = minSize.x;
    for (const auto& historyItem : commands)
    {
        width = std::max(width, historyItem.size() + padding);
    }

    // calculate height, it'll be clamped later to up to 20 rows
    const auto height = std::max(minSize.y, commands.size());

    return {
        gsl::narrow_cast<til::CoordType>(std::min<size_t>(width, SHRT_MAX)),
        gsl::narrow_cast<til::CoordType>(std::min<size_t>(height, 20)),
    };
}

CommandListPopup::CommandListPopup(SCREEN_INFORMATION& screenInfo, const CommandHistory& history) :
    Popup(screenInfo, calculatePopupSize(history)),
    _history{ history },
    _currentCommand{ std::min(history.LastDisplayed, gsl::narrow<int>(history.GetNumberOfCommands() - 1)) }
{
    FAIL_FAST_IF(_currentCommand < 0);
    _setBottomIndex();
}

[[nodiscard]] NTSTATUS CommandListPopup::_handlePopupKeys(COOKED_READ_DATA& cookedReadData,
                                                          const wchar_t wch,
                                                          const DWORD modifiers) noexcept
{
    try
    {
        auto Index = 0;
        const bool shiftPressed = WI_IsFlagSet(modifiers, SHIFT_PRESSED);
        switch (wch)
        {
        case VK_F9:
        {
            const HRESULT hr = CommandLine::Instance().StartCommandNumberPopup(cookedReadData);
            if (S_FALSE == hr)
            {
                // If we couldn't make the popup, break and go around to read another input character.
                break;
            }
            else
            {
                return hr;
            }
        }
        case VK_ESCAPE:
            CommandLine::Instance().EndCurrentPopup();
            return CONSOLE_STATUS_WAIT_NO_BLOCK;
        case VK_UP:
            if (shiftPressed)
            {
                return _swapUp(cookedReadData);
            }
            else
            {
                _update(-1);
            }
            break;
        case VK_DOWN:
            if (shiftPressed)
            {
                return _swapDown(cookedReadData);
            }
            else
            {
                _update(1);
            }
            break;
        case VK_END:
            // Move waaay forward, UpdateCommandListPopup() can handle it.
            _update(cookedReadData.History().GetNumberOfCommands());
            break;
        case VK_HOME:
            // Move waaay back, UpdateCommandListPopup() can handle it.
            _update(-cookedReadData.History().GetNumberOfCommands());
            break;
        case VK_PRIOR:
            _update(-Height());
            break;
        case VK_NEXT:
            _update(Height());
            break;
        case VK_DELETE:
            return _deleteSelection(cookedReadData);
        case VK_LEFT:
        case VK_RIGHT:
            Index = _currentCommand;
            CommandLine::Instance().EndCurrentPopup();
            SetCurrentCommandLine(cookedReadData, Index);
            return CONSOLE_STATUS_WAIT_NO_BLOCK;
        default:
            break;
        }
    }
    CATCH_LOG();
    return STATUS_SUCCESS;
}

void CommandListPopup::_setBottomIndex()
{
    if (_currentCommand < _history.GetNumberOfCommands() - Height())
    {
        _bottomIndex = std::max(_currentCommand, Height() - 1);
    }
    else
    {
        _bottomIndex = _history.GetNumberOfCommands() - 1;
    }
}

[[nodiscard]] NTSTATUS CommandListPopup::_deleteSelection(COOKED_READ_DATA& cookedReadData) noexcept
{
    try
    {
        auto& history = cookedReadData.History();
        history.Remove(static_cast<short>(_currentCommand));
        _setBottomIndex();

        if (history.GetNumberOfCommands() == 0)
        {
            // close the popup
            return CONSOLE_STATUS_READ_COMPLETE;
        }
        else if (_currentCommand >= static_cast<short>(history.GetNumberOfCommands()))
        {
            _currentCommand = static_cast<short>(history.GetNumberOfCommands() - 1);
            _bottomIndex = _currentCommand;
        }

        _drawList();
    }
    CATCH_LOG();
    return STATUS_SUCCESS;
}

// Routine Description:
// - moves the selected history item up in the history list
// Arguments:
// - cookedReadData - the read wait object to operate upon
[[nodiscard]] NTSTATUS CommandListPopup::_swapUp(COOKED_READ_DATA& cookedReadData) noexcept
{
    try
    {
        auto& history = cookedReadData.History();

        if (history.GetNumberOfCommands() <= 1 || _currentCommand == 0)
        {
            return STATUS_SUCCESS;
        }
        history.Swap(_currentCommand, _currentCommand - 1);
        _update(-1);
        _drawList();
    }
    CATCH_LOG();
    return STATUS_SUCCESS;
}

// Routine Description:
// - moves the selected history item down in the history list
// Arguments:
// - cookedReadData - the read wait object to operate upon
[[nodiscard]] NTSTATUS CommandListPopup::_swapDown(COOKED_READ_DATA& cookedReadData) noexcept
{
    try
    {
        auto& history = cookedReadData.History();

        if (history.GetNumberOfCommands() <= 1 || _currentCommand == history.GetNumberOfCommands() - 1)
        {
            return STATUS_SUCCESS;
        }
        history.Swap(_currentCommand, _currentCommand + 1);
        _update(1);
        _drawList();
    }
    CATCH_LOG();
    return STATUS_SUCCESS;
}

void CommandListPopup::_handleReturn(COOKED_READ_DATA& cookedReadData)
{
    auto Index = 0;
    NTSTATUS Status = STATUS_SUCCESS;
    DWORD LineCount = 1;
    Index = _currentCommand;
    CommandLine::Instance().EndCurrentPopup();
    SetCurrentCommandLine(cookedReadData, Index);
    cookedReadData.ProcessInput(UNICODE_CARRIAGERETURN, 0, Status);
    // complete read
    if (cookedReadData.IsEchoInput())
    {
        // check for alias
        cookedReadData.ProcessAliases(LineCount);
    }

    Status = STATUS_SUCCESS;
    size_t NumBytes;
    if (cookedReadData.BytesRead() > cookedReadData.UserBufferSize() || LineCount > 1)
    {
        if (LineCount > 1)
        {
            const wchar_t* Tmp;
            for (Tmp = cookedReadData.BufferStartPtr(); *Tmp != UNICODE_LINEFEED; Tmp++)
            {
                FAIL_FAST_IF(!(Tmp < (cookedReadData.BufferStartPtr() + cookedReadData.BytesRead())));
            }
            NumBytes = (Tmp - cookedReadData.BufferStartPtr() + 1) * sizeof(*Tmp);
        }
        else
        {
            NumBytes = cookedReadData.UserBufferSize();
        }

        // Copy what we can fit into the user buffer
        const size_t bytesWritten = cookedReadData.SavePromptToUserBuffer(NumBytes / sizeof(wchar_t));

        // Store all of the remaining as pending until the next read operation.
        cookedReadData.SavePendingInput(NumBytes / sizeof(wchar_t), LineCount > 1);
        NumBytes = bytesWritten;
    }
    else
    {
        NumBytes = cookedReadData.BytesRead();
        NumBytes = cookedReadData.SavePromptToUserBuffer(NumBytes / sizeof(wchar_t));
    }

    cookedReadData.SetReportedByteCount(NumBytes);
}

void CommandListPopup::_cycleSelectionToMatchingCommands(COOKED_READ_DATA& cookedReadData, const wchar_t wch)
{
    auto Index = 0;
    if (cookedReadData.History().FindMatchingCommand({ &wch, 1 },
                                                     _currentCommand,
                                                     Index,
                                                     CommandHistory::MatchOptions::JustLooking))
    {
        _update(Index - _currentCommand, true);
    }
}

// Routine Description:
// - This routine handles the command list popup.  It returns when we're out of input or the user has selected a command line.
// Return Value:
// - CONSOLE_STATUS_WAIT - we ran out of input, so a wait block was created
// - CONSOLE_STATUS_READ_COMPLETE - user hit return
[[nodiscard]] NTSTATUS CommandListPopup::Process(COOKED_READ_DATA& cookedReadData) noexcept
{
    NTSTATUS Status = STATUS_SUCCESS;

    for (;;)
    {
        WCHAR wch = UNICODE_NULL;
        bool popupKeys = false;
        DWORD modifiers = 0;

        Status = _getUserInput(cookedReadData, popupKeys, modifiers, wch);
        if (!NT_SUCCESS(Status))
        {
            return Status;
        }

        if (popupKeys)
        {
            Status = _handlePopupKeys(cookedReadData, wch, modifiers);
            if (Status != STATUS_SUCCESS)
            {
                return Status;
            }
        }
        else if (wch == UNICODE_CARRIAGERETURN)
        {
            _handleReturn(cookedReadData);
            return CONSOLE_STATUS_READ_COMPLETE;
        }
        else
        {
            // cycle through commands that start with the letter of the key pressed
            _cycleSelectionToMatchingCommands(cookedReadData, wch);
        }
    }
}

void CommandListPopup::_DrawContent()
{
    _drawList();
}

// Routine Description:
// - Draws a list of commands for the user to choose from
void CommandListPopup::_drawList()
{
    // draw empty popup
    til::coord WriteCoord;
    WriteCoord.x = _region.left + 1;
    WriteCoord.y = _region.top + 1;
    size_t lStringLength = Width();
    for (auto i = 0; i < Height(); ++i)
    {
        const OutputCellIterator spaces(UNICODE_SPACE, _attributes, lStringLength);
        const auto result = _screenInfo.Write(spaces, WriteCoord);
        lStringLength = result.GetCellDistance(spaces);
        WriteCoord.y += 1;
    }

    auto& api = Microsoft::Console::Interactivity::ServiceLocator::LocateGlobals().api;

    WriteCoord.y = _region.top + 1;
    auto i = std::max(_bottomIndex - Height() + 1, 0);
    for (; i <= _bottomIndex; i++)
    {
        CHAR CommandNumber[COMMAND_NUMBER_SIZE];
        // Write command number to screen.
        if (0 != _itoa_s(i, CommandNumber, ARRAYSIZE(CommandNumber), 10))
        {
            return;
        }

        PCHAR CommandNumberPtr = CommandNumber;

        size_t CommandNumberLength;
        if (FAILED(StringCchLengthA(CommandNumberPtr, ARRAYSIZE(CommandNumber), &CommandNumberLength)))
        {
            return;
        }

        if (CommandNumberLength + 1 >= ARRAYSIZE(CommandNumber))
        {
            return;
        }

        CommandNumber[CommandNumberLength] = ':';
        CommandNumber[CommandNumberLength + 1] = ' ';
        CommandNumberLength += 2;
        if (CommandNumberLength > static_cast<ULONG>(Width()))
        {
            CommandNumberLength = static_cast<ULONG>(Width());
        }

        WriteCoord.x = _region.left + 1;

        LOG_IF_FAILED(api.WriteConsoleOutputCharacterAImpl(_screenInfo,
                                                           { CommandNumberPtr, CommandNumberLength },
                                                           WriteCoord,
                                                           CommandNumberLength));

        // write command to screen
        auto command = _history.GetNth(i);
        lStringLength = command.size();
        {
            size_t lTmpStringLength = lStringLength;
            LONG lPopupLength = static_cast<LONG>(Width() - CommandNumberLength);
            PCWCHAR lpStr = command.data();
            while (lTmpStringLength--)
            {
                if (IsGlyphFullWidth(*lpStr++))
                {
                    lPopupLength -= 2;
                }
                else
                {
                    lPopupLength--;
                }

                if (lPopupLength <= 0)
                {
                    lStringLength -= lTmpStringLength;
                    if (lPopupLength < 0)
                    {
                        lStringLength--;
                    }

                    break;
                }
            }
        }

        // CommandNumberLength is < ARRAYSIZE(CommandNumber)-1
        WriteCoord.x += gsl::narrow_cast<decltype(WriteCoord.x)>(CommandNumberLength);

        size_t used;
        LOG_IF_FAILED(api.WriteConsoleOutputCharacterWImpl(_screenInfo,
                                                           { command.data(), lStringLength },
                                                           WriteCoord,
                                                           used));

        // write attributes to screen
        if (i == _currentCommand)
        {
            WriteCoord.x = _region.left + 1;
            // inverted attributes
            lStringLength = Width();
            TextAttribute inverted = _attributes;
            inverted.Invert();

            const OutputCellIterator it(inverted, lStringLength);
            const auto done = _screenInfo.Write(it, WriteCoord);

            lStringLength = done.GetCellDistance(it);
        }

        WriteCoord.y += 1;
    }
}

// Routine Description:
// - For popup lists, will adjust the position of the highlighted item and
//   possibly scroll the list if necessary.
// Arguments:
// - originalDelta - The number of lines to move up or down
// - wrap - Down past the bottom or up past the top should wrap the command list
void CommandListPopup::_update(const int originalDelta, const bool wrap)
{
    auto delta = originalDelta;
    if (delta == 0)
    {
        return;
    }
    const auto Size = Height();

    auto CurCmdNum = _currentCommand;
    auto NewCmdNum = CurCmdNum + delta;

    if (wrap)
    {
        // Modulo the number of commands to "circle" around if we went off the end.
        NewCmdNum %= _history.GetNumberOfCommands();
    }
    else
    {
        if (NewCmdNum >= _history.GetNumberOfCommands())
        {
            NewCmdNum = gsl::narrow_cast<decltype(NewCmdNum)>(_history.GetNumberOfCommands() - 1);
        }
        else if (NewCmdNum < 0)
        {
            NewCmdNum = 0;
        }
    }
    delta = NewCmdNum - CurCmdNum;

    bool Scroll = false;
    // determine amount to scroll, if any
    if (NewCmdNum <= _bottomIndex - Size)
    {
        _bottomIndex += delta;
        if (_bottomIndex < Size - 1)
        {
            _bottomIndex = Size - 1;
        }
        Scroll = true;
    }
    else if (NewCmdNum > _bottomIndex)
    {
        _bottomIndex += delta;
        if (_bottomIndex >= _history.GetNumberOfCommands())
        {
            _bottomIndex = gsl::narrow_cast<decltype(_bottomIndex)>(_history.GetNumberOfCommands() - 1);
        }
        Scroll = true;
    }

    // write commands to popup
    if (Scroll)
    {
        _currentCommand = NewCmdNum;
        _drawList();
    }
    else
    {
        _updateHighlight(_currentCommand, NewCmdNum);
        _currentCommand = NewCmdNum;
    }
}

// Routine Description:
// - Adjusts the highlighted line in a list of commands
// Arguments:
// - OldCurrentCommand - The previous command highlighted
// - NewCurrentCommand - The new command to be highlighted.
void CommandListPopup::_updateHighlight(const int OldCurrentCommand, const int NewCurrentCommand)
{
    int TopIndex;
    if (_bottomIndex < Height())
    {
        TopIndex = 0;
    }
    else
    {
        TopIndex = _bottomIndex - Height() + 1;
    }
    til::coord WriteCoord;
    WriteCoord.x = _region.left + 1;
    size_t lStringLength = Width();

    WriteCoord.y = _region.top + 1 + OldCurrentCommand - TopIndex;

    const OutputCellIterator it(_attributes, lStringLength);
    const auto done = _screenInfo.Write(it, WriteCoord);
    lStringLength = done.GetCellDistance(it);

    // highlight new command
    WriteCoord.y = _region.top + 1 + NewCurrentCommand - TopIndex;

    // inverted attributes
    TextAttribute inverted = _attributes;
    inverted.Invert();
    const OutputCellIterator itAttr(inverted, lStringLength);
    const auto doneAttr = _screenInfo.Write(itAttr, WriteCoord);
    lStringLength = done.GetCellDistance(itAttr);
}
