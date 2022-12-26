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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
