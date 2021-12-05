// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

// This header prototypes the Pseudoconsole symbols from conpty.lib with their original names.
// This is required because we cannot import __imp_CreatePseudoConsole from a static library
// as it doesn't produce an import lib.
// We can't use an /ALTERNATENAME trick because it seems that that name is only resolved when the
// linker cannot otherwise find the symbol.

#pragma once

#include <consoleapi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PSEUDOCONSOLE_RESIZE_QUIRK (2u)
#define PSEUDOCONSOLE_WIN32_INPUT_MODE (4u)

HRESULT WINAPI ConptyCreatePseudoConsole(til::coord size, HANDLE hInput, HANDLE hOutput, DWORD dwFlags, HPCON* phPC);

HRESULT WINAPI ConptyResizePseudoConsole(HPCON hPC, til::coord size);

HRESULT WINAPI ConptyClearPseudoConsole(HPCON hPC);

VOID WINAPI ConptyClosePseudoConsole(HPCON hPC);

HRESULT WINAPI ConptyPackPseudoConsole(HANDLE hServerProcess, HANDLE hRef, HANDLE hSignal, HPCON* phPC);

#ifdef __cplusplus
}
#endif
