﻿#pragma once

#define NOMINMAX
#include <windows.h>

#include "basic.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_clipboard_text(std::string_view text);
void set_clipboard_text(std::wstring_view text);

std::filesystem::path get_exe_path(HINSTANCE hinstance);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class _cursor { normal, size_all, hand_point, size_we, size_ns, drag, vcursor }; // виды курсора

inline _cursor g_cursor = _cursor::normal; // установленный курсор

void set_cursor(_cursor x);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
