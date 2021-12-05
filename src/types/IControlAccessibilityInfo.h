/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- IControlAccessibilityInfo.h

Abstract:
- This serves as the interface defining all information known by the control
  hosting the terminal renderer that is needed for the UI Automation Tree.

Author(s):
- Zoey Riordan (zorio) Feb-2020
--*/

#pragma once

#include <wtypes.h>

namespace Microsoft::Console::Types
{
    class IControlAccessibilityInfo
    {
    public:
        virtual ~IControlAccessibilityInfo() = 0;

        virtual til::coord GetFontSize() const noexcept = 0;
        virtual RECT GetBounds() const noexcept = 0;
        virtual RECT GetPadding() const noexcept = 0;
        virtual double GetScaleFactor() const noexcept = 0;
        virtual void ChangeViewport(const til::small_rect NewWindow) = 0;
        virtual HRESULT GetHostUiaProvider(IRawElementProviderSimple** provider) = 0;

    protected:
        IControlAccessibilityInfo() = default;
        IControlAccessibilityInfo(const IControlAccessibilityInfo&) = default;
        IControlAccessibilityInfo(IControlAccessibilityInfo&&) = default;
        IControlAccessibilityInfo& operator=(const IControlAccessibilityInfo&) = default;
        IControlAccessibilityInfo& operator=(IControlAccessibilityInfo&&) = default;
    };

    inline IControlAccessibilityInfo::~IControlAccessibilityInfo() {}
}
