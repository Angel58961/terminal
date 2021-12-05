/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- IUiaData.hpp

Abstract:
- This serves as the interface defining all information needed for the UI Automation Tree

Author(s):
- Carlos Zamora (CaZamor) Aug-2019
--*/

#pragma once

#include "IBaseData.h"

struct ITextRangeProvider;

namespace Microsoft::Console::Types
{
    class IUiaData : public IBaseData
    {
    public:
        ~IUiaData() = 0;

    protected:
        IUiaData() = default;
        IUiaData(const IUiaData&) = default;
        IUiaData(IUiaData&&) = default;
        IUiaData& operator=(const IUiaData&) = default;
        IUiaData& operator=(IUiaData&&) = default;

    public:
        virtual const bool IsSelectionActive() const = 0;
        virtual const bool IsBlockSelection() const = 0;
        virtual void ClearSelection() = 0;
        virtual void SelectNewRegion(const til::coord coordStart, const til::coord coordEnd) = 0;
        virtual const til::coord GetSelectionAnchor() const noexcept = 0;
        virtual const til::coord GetSelectionEnd() const noexcept = 0;
        virtual void ColorSelection(const til::coord coordSelectionStart, const til::coord coordSelectionEnd, const TextAttribute attr) = 0;
        virtual const bool IsUiaDataInitialized() const noexcept = 0;
    };

    // See docs/virtual-dtors.md for an explanation of why this is weird.
    inline IUiaData::~IUiaData() {}
}
