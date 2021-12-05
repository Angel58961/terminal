/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- renderData.hpp

Abstract:
- This method provides an interface for rendering the final display based on the current console state

Author(s):
- Michael Niksa (miniksa) Nov 2015
--*/

#pragma once

#include "../renderer/inc/IRenderData.hpp"
#include "../types/inc/colorTable.hpp"
#include "../types/IUiaData.h"

class RenderData final :
    public Microsoft::Console::Render::IRenderData,
    public Microsoft::Console::Types::IUiaData
{
public:
#pragma region BaseData
    Microsoft::Console::Types::Viewport GetViewport() noexcept override;
    til::coord GetTextBufferEndPosition() const noexcept override;
    const TextBuffer& GetTextBuffer() noexcept override;
    const FontInfo& GetFontInfo() noexcept override;
    std::pair<COLORREF, COLORREF> GetAttributeColors(const TextAttribute& attr) const noexcept override;

    std::vector<Microsoft::Console::Types::Viewport> GetSelectionRects() noexcept override;

    void LockConsole() noexcept override;
    void UnlockConsole() noexcept override;
#pragma endregion

#pragma region IRenderData
    const TextAttribute GetDefaultBrushColors() noexcept override;

    til::coord GetCursorPosition() const noexcept override;
    bool IsCursorVisible() const noexcept override;
    bool IsCursorOn() const noexcept override;
    ULONG GetCursorHeight() const noexcept override;
    CursorType GetCursorStyle() const noexcept override;
    ULONG GetCursorPixelWidth() const noexcept override;
    COLORREF GetCursorColor() const noexcept override;
    bool IsCursorDoubleWidth() const override;

    bool IsScreenReversed() const noexcept override;

    const std::vector<Microsoft::Console::Render::RenderOverlay> GetOverlays() const noexcept override;

    const bool IsGridLineDrawingAllowed() noexcept override;

    const std::wstring_view GetConsoleTitle() const noexcept override;

    const std::wstring GetHyperlinkUri(uint16_t id) const noexcept override;
    const std::wstring GetHyperlinkCustomId(uint16_t id) const noexcept override;

    const std::vector<size_t> GetPatternId(const til::coord location) const noexcept override;
#pragma endregion

#pragma region IUiaData
    const bool IsSelectionActive() const override;
    const bool IsBlockSelection() const noexcept override;
    void ClearSelection() override;
    void SelectNewRegion(const til::coord coordStart, const til::coord coordEnd) override;
    const til::coord GetSelectionAnchor() const noexcept;
    const til::coord GetSelectionEnd() const noexcept;
    void ColorSelection(const til::coord coordSelectionStart, const til::coord coordSelectionEnd, const TextAttribute attr);
    const bool IsUiaDataInitialized() const noexcept override { return true; }
#pragma endregion
};
