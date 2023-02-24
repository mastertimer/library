#pragma once

#include "geometry.h"

#define NOMINMAX
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

union _color
{
	uint c;
	struct
	{
		uchar b, g, r, a;
	};

	_color() = default;
	_color(uint c2) : c(c2) {}

	operator uint() const { return c; }

	void operator=(uint c2) { c = c2; }

	bool operator==(const _color color) const { return c == color.c; }
	bool operator!=(const _color color) const { return c != color.c; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _picture
{
	union
	{
		uint* data = nullptr;
		_color* data2;
	};
	_isize size;
	bool transparent = false;
	_iarea drawing_area; // разрешенная область для рисования

	_picture() = default;
	explicit _picture(_isize r);
	explicit _picture(_isize r, _color c);
	_picture(const _picture& copy);
	_picture(_picture&& move) noexcept;
	~_picture();

	_picture& operator=(_picture&& move) noexcept;
	_picture& operator=(const _picture& copy);
	bool operator==(const _picture& pic) const;

	bool save_to_file(const std::filesystem::path& file_name);
	bool load_from_file(const std::filesystem::path& file_name);
	bool set_from_text(std::string_view s, uint c00, uint cc1);

	_color* scan_line(i64 y) const { return &data2[y * size.x]; }

	virtual bool resize(_isize wh);
	void set_drawing_area(const _iarea& q);
	void reset_drawing_area();

	void clear(_color c = 0xFF000000); // *
	void set_transparent(); // * узнать, есть ли прозрачные пиксели

	void draw(_ixy r, const _picture& bm);

	void line(_ixy p1, _ixy p2, _color c, bool rep = false);
	void lines(_xy p1, _xy p2, double l, uint c); // точная линия заданной толщины
	void text16(_ixy p, std::string_view st, uint c, uint bg = 0); // простой текст высотой 16
	void text16n(i64 x, i64 y, std::string_view st, i64 n, uint c, uint bg = 0); // простой текст высотой 16*n
	static _isize size_text16(std::string_view s, i64 n = 1); // размер текста *n

	void fill_ring(_xy p, double r, double d, _color c, _color c2);
	void fill_circle(_xy p, double r, _color c);
	void ring(_xy p, double r, double d, _color c);

	void fill_rectangle(_iarea r, _color c, bool rep = false);
	void fill_rectangle(_area r, _color c);

	void rectangle(_iarea oo, _color c);

	void stretch_draw(_picture* bm, i64 x, i64 y, double m);
	void stretch_draw_speed(_picture* bm, i64 nXDest, i64 nYDest, double m);

	//	void text0(int x, int y, std::string_view s, int h, uint c, uint bg);
	friend struct _picture_functions;

private:
	void set_transparent(const _color c) { transparent |= c.a != 0xff; } // *

	_color& pixel(const i64 x, const i64 y) { return data2[y * size.x + x]; }
	const _color& pixel(const i64 x, const i64 y) const { return data2[y * size.x + x]; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_stack& operator<<(_stack& o, _picture const& p);
_stack& operator>>(_stack& o, _picture& p);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _bitmap : public _picture
{
	HBITMAP bitmap2;
	HDC hdc = 0;

	LOGFONT font;
	bool izm_font = true;// шрифт был изменен
	HFONT hfont = 0;
	uint f_c = 0; // цвет шрифта
	uint f_cf = 0; // цвет фона шрифта

	std::wstring get_font_name();
	void set_font(std::wstring_view name, bool bold);
	void podg_font(int r); // подготовка шрифта в выводу
	void podg_cc(uint c, uint cf); // подготовка цветов к выводу
	_bitmap& operator=(const _picture& copy);
	_bitmap& operator=(const _bitmap& copy);

	explicit _bitmap(int rx3 = 0, int ry3 = 0);
	~_bitmap();

	virtual bool resize(_isize wh);

	void text(_ixy p, std::wstring_view s, int h, uint c, uint bg = 0);
	void text(_ixy p, std::string_view s, int h, uint c, uint bg = 0);
	_isize size_text(std::wstring_view s, int h);
	_isize size_text(std::string_view s, int h);

	void grab_ecran_oo2(HWND hwnd); // украсть часть экрана
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline uint cclow(uint c) { return ((c >> 2) & 0x3F3F3F) + (c & 0xFF000000); }
uint brighten(uint c);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
