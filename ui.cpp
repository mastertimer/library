#include "ui.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_ui_element::_ui_element(_ui* ui_): ui(ui_)
{
}

_ui_element::~_ui_element()
{
}

void _ui_element::ris2(_trans tr)
{
}

void _ui_element::run()
{
	auto copy_subelements = subelements;
	for (auto element : copy_subelements) element->run();
}

void _ui_element::key_down(ushort key)
{
}

void _ui_element::key_press(ushort key)
{
}

bool _ui_element::mouse_wheel2(_xy r)
{
	return false;
}

bool _ui_element::mouse_down_left2(_xy r)
{
	return false;
}

void _ui_element::mouse_move_left2(_xy r)
{
}

void _ui_element::cha_area(std::optional<_area> a)
{
	if (!a) a = local_area;
	if (!parent)
	{
		if (ui->n_ko.get() == this) ui->add_changed_area(trans(*a));
		return;
	}
	parent->cha_area(trans(*a));
}

void _ui_element::ris(_trans tr)
{
	tr *= trans;
	if (((tr(calc_area())) & ui->changed_area).empty()) return;
	ui->canvas.set_drawing_area(tr(local_area) & ui->changed_area);
	ris2(tr);
	for (auto element : subelements) element->ris(tr);
}

_area _ui_element::calc_area()
{
	if (area) return *area;
	area = local_area;
	for (auto element : subelements)
	{
		element->calc_area();
		*area |= element->trans(*element->area);
	}
	return *area;
}

void _ui_element::add_child(std::shared_ptr<_ui_element> element)
{
	subelements.insert(element);
	element->parent = shared_from_this();
}

bool _ui_element::test_local_area(_xy b)
{
	return local_area.test(b);
}

bool _ui_element::mouse_move2(_xy r)
{
	return true;
}

bool _ui_element::mouse_move(_trans tr)
{
	for (auto element : subelements)
	{
		_trans tr2 = tr * element->trans;
		if (!element->calc_area().test(tr2.inverse(ui->mouse_xy))) continue;
		if (element->mouse_move(tr2)) return true;
	}
	_xy r = tr.inverse(ui->mouse_xy);
	if (test_local_area(r)) // ДЕЙСТВИЕ
	{
		ui->master_trans_go = tr;
		if (mouse_move2(r))
		{
			if ((ui->n_go_move.get() != this) || (ui->master_trans_go_move != ui->master_trans_go))
			{
				if (ui->n_go_move)
				{
					ui->n_go_move->mouse_finish_move();
				}
				ui->n_go_move = shared_from_this();
				ui->master_trans_go_move = ui->master_trans_go;
			}
			return true;
		}
	}
	return false;
}

void _ui_element::mouse_finish_move()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

_ui::_ui()
{
	n_ko = std::make_shared<_ui_element>(this);
}

void _ui::add_changed_area(_area a)
{
	changed_area |= (a & canvas.size);
}

void _ui::draw(_isize r)
{
	if (canvas.resize(r)) changed_area = r;
	if (changed_area.empty()) return;
	canvas.set_drawing_area(changed_area);
	changed_area &= canvas.size;
	canvas.clear(cc0);
	n_ko->ris(_trans());
	changed_area = _area();
}

void _ui::run_timer1000()
{
	for (auto element : n_timer1000) element->run();
}

void _ui::key_down(ushort key)
{
	if (!n_act_key) return;
	n_act_key->key_down(key);
}

void _ui::key_press(ushort key)
{
	if (!n_act_key) return;
	n_act_key->key_press(key);
}

void _ui::mouse_move()
{
	if (n_tani)
	{
		if (!n_s_left)
		{
			mouse_button_left(false);
			return;
		}
		n_tani->mouse_move_left2(master_trans_go.inverse(mouse_xy));
		mouse_xy_pr = mouse_xy;
		return;
	}
	if (n_perenos)
	{
		if (n_s_left)
		{
			n_ko->cha_area(n_ko->calc_area());
			n_ko->trans.offset.x += mouse_xy.x - mouse_xy_pr.x;
			n_ko->trans.offset.y += mouse_xy.y - mouse_xy_pr.y;
			n_ko->cha_area(n_ko->calc_area());
			mouse_xy_pr = mouse_xy;
		}
		return; // закомментировать - колесо будет работать, но курсор будет сбрасываться при выделении
	}
	if (!n_ko->mouse_move(n_ko->trans))
		if (n_go_move)
		{
			n_go_move->mouse_finish_move();
			n_go_move.reset();
		}
}

void _ui::mouse_button_left(bool pressed)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
