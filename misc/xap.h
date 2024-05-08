#pragma once

#include <mmsystem.h>
#include <dsound.h>
#include <virtual_binary.h>

void xap_play(string, LPDIRECTSOUND, Cvirtual_binary);

inline atomic<BOOL> xapTestBool;
inline string xapTestString;
