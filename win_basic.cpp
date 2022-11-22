#include "win_basic.h"

#define NOMINMAX
#include <windows.h>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void set_clipboard_text(std::wstring_view text)
{
	if (OpenClipboard(0))//открываем буфер обмена
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		EmptyClipboard(); //очищаем буфер
		size_t ll = text.size() * 2 + 2;
		hgBuffer = GlobalAlloc(GMEM_DDESHARE, ll);//выделяем память
		if (!hgBuffer) goto end;
		chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
		if (!chBuffer) goto end;
		memcpy(chBuffer, text.data(), ll);
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_UNICODETEXT, hgBuffer);//помещаем текст в буфер обмена
	end:
		CloseClipboard(); //закрываем буфер обмена
	}
}

void set_clipboard_text(astr text)
{
	if (OpenClipboard(0))//открываем буфер обмена
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		EmptyClipboard(); //очищаем буфер
		size_t ll = strlen(text) + 1;
		hgBuffer = GlobalAlloc(GMEM_DDESHARE, ll);//выделяем память
		if (!hgBuffer) goto end;
		chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
		if (!chBuffer) goto end;
		strcpy_s(chBuffer, ll, LPCSTR(text));
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
	end:
		CloseClipboard(); //закрываем буфер обмена
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
