﻿#pragma once

#include "graphics.h"
#include <memory>
#include <optional>
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui;

struct _ui_element : public std::enable_shared_from_this<_ui_element>
{
	_area local_area;
	_trans trans;

	_ui_element(_ui* ui_);
	virtual ~_ui_element();

	void ris(_trans tr); // нарисовать
	void add_child(std::shared_ptr<_ui_element> element);
	void cha_area(std::optional<_area> a = std::nullopt);
	_area calc_area(); // вычислить область

	virtual void run();
	virtual void key_down(ushort key);
	virtual void key_press(ushort key);
	virtual bool mouse_wheel2(_xy r, short value);
	virtual bool mouse_down_left2(_xy r);
	virtual void mouse_move_left2(_xy r);
	virtual void mouse_up_left2(_xy r); // конец перетаскивания
	bool mouse_down_left(_trans tr); // щелчок мышки
	bool mouse_move(_trans tr); // перемещение мышки
	bool mouse_wheel_turn(_trans tr, short value); // поворот колеса мышки
	virtual bool mouse_move2(_xy r); // перемещение мышки действие
	virtual void mouse_finish_move(); // мышка ушла
	virtual bool test_local_area(_xy b); // лежит ли точка внутри

protected:
	_ui* ui;
	bool key_fokus = false;
	_color c{ 0xFF208040 };
	_color c2{ 0 };

	virtual void ris2(_trans tr);


private:
	std::optional<_area> area;
	std::set<std::shared_ptr<_ui_element>> subelements;
	std::shared_ptr<_ui_element> parent;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct _ui
{
	_bitmap canvas;
	_area changed_area;

	std::shared_ptr<_ui_element> n_ko;
	std::shared_ptr<_ui_element> n_act_key;
	std::shared_ptr<_ui_element> n_tani;
	std::shared_ptr<_ui_element> n_go_move;
	std::set<std::shared_ptr<_ui_element>> n_timer1000;
	bool n_s_ctrl = false;
	bool n_s_shift = false;
	bool n_s_left = false;
	bool n_s_right = false;
	bool n_s_middle = false;
	bool n_s_double = false;
	bool n_perenos = false;
	_xy mouse_xy{ 0, 0 };
	_xy mouse_xy_pr{ 0, 0 };

	_trans master_trans_go; // трансформация тяни-толкай объекта, или объекта под мышкой
	_trans master_trans_go_move; // трансформация n_go_move

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
	void mouse_move();
	void mouse_button_left_down();
	void mouse_button_left_up();
	void mouse_button_right_down();
	void mouse_button_right_up();
	void mouse_button_middle_down();
	void mouse_button_middle_up();
	void mouse_wheel_turn(short value);

private:
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
