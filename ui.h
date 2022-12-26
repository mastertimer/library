#pragma once

#include "graphics.h"
#include <memory>
#include <optional>
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui;

struct _ui_element : public std::enable_shared_from_this<_ui_element>
{
	_area local_area;

	_ui_element(_ui* ui_);
	virtual ~_ui_element();

	void ris(_trans tr); // нарисовать
	void add_child(std::shared_ptr<_ui_element> element);

	virtual void run();
	virtual void key_down(ushort key);
	virtual void key_press(ushort key);

protected:
	_ui* ui;
	bool key_fokus = false;
	_color c{ 0xFF208040 };
	_color c2{ 0 };

	virtual void ris2(_trans tr);
	virtual bool mouse_wheel2(_xy r);
	virtual bool mouse_down_left2(_xy r);
	virtual void mouse_move_left2(_xy r);

	void cha_area(std::optional<_area> a = std::nullopt);

private:
	_trans trans;
	std::optional<_area> area;
	std::set<std::shared_ptr<_ui_element>> subelements;
	std::shared_ptr<_ui_element> parent;

	_area calc_area(); // вычислить область

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui
{
	_bitmap canvas;
	_area changed_area;

	std::shared_ptr<_ui_element> n_ko;
	std::shared_ptr<_ui_element> n_act_key;
	std::set<std::shared_ptr<_ui_element>> n_timer1000;
	bool n_s_ctrl = false;
	i64 n_wheel = 0;

	_trans master_trans_go;

	const _color c00{ 0 };          // прозрачный цвет
	const _color cc0{ 0xFF000000 }; // цвет фона
	const _color cc1{ 0xFF208040 }; // цвет 1
	const _color cc2{ 0xFF40FF80 }; // цвет 2
	const _color cc3{ 0xFFA0FFC0 }; // цвет 3
	const _color cc4{ 0xFF104020 }; // цвет 4

	_ui();
	void add_changed_area(_area a);
	void draw(_isize r);
	void run_timer1000();
	void key_down(ushort key);
	void key_press(ushort key);

private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
