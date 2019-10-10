// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#ifdef UNICODE
typedef std::basic_string<TCHAR> nstring;
#ifndef TEXT
#define TEXT(quote) L##quote
#endif
#else
typedef std::string nstring;
#ifndef TEXT
#define TEXT(quote) quote
#endif
#endif



// TODO: reference additional headers your program requires here
