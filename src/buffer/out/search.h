/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- search.h

Abstract:
- This module is used for searching through the screen for a substring

Author(s):
- Michael Niksa (MiNiksa) 20-Apr-2018

Revision History:
- From components of find.c by Jerry Shea (jerrysh) 1-May-1997
--*/

#pragma once

#include <WinConTypes.h>
#include "TextAttribute.hpp"
#include "textBuffer.hpp"
#include "../types/IUiaData.h"

// This used to be in find.h.
#define SEARCH_STRING_LENGTH (80)

class Search final
{
public:
    enum class Direction
    {
        Forward,
        Backward
    };

    enum class Sensitivity
    {
        CaseInsensitive,
        CaseSensitive
    };

    Search(Microsoft::Console::Types::IUiaData& uiaData,
           const std::wstring& str,
           const Direction dir,
           const Sensitivity sensitivity);

    Search(Microsoft::Console::Types::IUiaData& uiaData,
           const std::wstring& str,
           const Direction dir,
           const Sensitivity sensitivity,
           const til::coord anchor);

    bool FindNext();
    void Select() const;
    void Color(const TextAttribute attr) const;

    std::pair<til::coord, til::coord> GetFoundLocation() const noexcept;

private:
    wchar_t _ApplySensitivity(const wchar_t wch) const noexcept;
    bool _FindNeedleInHaystackAt(const til::coord pos, til::coord& start, til::coord& end) const;
    bool _CompareChars(const std::wstring_view one, const std::wstring_view two) const noexcept;
    void _UpdateNextPosition();

    void _IncrementCoord(til::coord& coord) const noexcept;
    void _DecrementCoord(til::coord& coord) const noexcept;

    static til::coord s_GetInitialAnchor(Microsoft::Console::Types::IUiaData& uiaData, const Direction dir);

    static std::vector<std::vector<wchar_t>> s_CreateNeedleFromString(const std::wstring& wstr);

    bool _reachedEnd = false;
    til::coord _coordNext;
    til::coord _coordSelStart;
    til::coord _coordSelEnd;

    const til::coord _coordAnchor;
    const std::vector<std::vector<wchar_t>> _needle;
    const Direction _direction;
    const Sensitivity _sensitivity;
    Microsoft::Console::Types::IUiaData& _uiaData;

#ifdef UNIT_TESTING
    friend class SearchTests;
#endif
};
