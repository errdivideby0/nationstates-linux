/*
	This file is part of nationstates-linux.

    nationstates-linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nationstates-linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nationstates-linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "add.h"
#include "nationstates.h"
#include <cstdlib>
#include <gtkmm.h>

using namespace std;

Add_Popup::Add_Popup(): main_box(Gtk::ORIENTATION_VERTICAL), confirm_button("Confirm"), cancel_button("Cancel"){
	set_border_width(6);
	set_default_size(220, 60);

	add(main_box);
	main_box.pack_start(top_box);
	main_box.pack_start(bottom_box);

	set_title("Add Nation");

	top_box.pack_start(nation_input);
	nation_input.set_placeholder_text("input nation name");
	bottom_box.pack_start(cancel_button, Gtk::PACK_SHRINK);
	bottom_box.pack_start(confirm_button, Gtk::PACK_SHRINK);
	set_focus(nation_input);
	bottom_box.set_border_width(5);
	bottom_box.set_layout(Gtk::BUTTONBOX_END);

	cancel_button.signal_clicked().connect(sigc::mem_fun(*this, &Add_Popup::on_button_cancel));
	confirm_button.signal_clicked().connect(sigc::mem_fun(*this, &Add_Popup::on_button_confirm));
	nation_input.signal_activate().connect(sigc::mem_fun(*this, &Add_Popup::on_button_confirm));
	show_all_children();
}

void Add_Popup::on_button_cancel(){
	hide();
}

void Add_Popup::on_button_confirm(){
	hide();
	Nationstates::instance().new_nation(nation_input.get_text());
}
