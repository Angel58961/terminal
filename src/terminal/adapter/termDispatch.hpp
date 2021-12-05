// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

/*
Module Name:
- termDispatch.hpp

Abstract:
- This is a useful default implementation of all of the ITermDispatch callbacks.
    Fails on every callback, but handy for tests.
*/
#include "ITermDispatch.hpp"
#pragma once

namespace Microsoft::Console::VirtualTerminal
{
    class TermDispatch;
};

class Microsoft::Console::VirtualTerminal::TermDispatch : public Microsoft::Console::VirtualTerminal::ITermDispatch
{
public:
    void Execute(const wchar_t wchControl) override = 0;
    void Print(const wchar_t wchPrintable) override = 0;
    void PrintString(const std::wstring_view string) override = 0;
};
