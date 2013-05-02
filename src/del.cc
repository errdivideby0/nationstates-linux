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

#include "nationstates.h"
#include "del.h"
#include <gtkmm.h>
#include <cstdlib>

using namespace std;

Glib::ustring Delete_Popup::select_nat;

Delete_Popup::Delete_Popup(): main_box(Gtk::ORIENTATION_VERTICAL), delete_button("Yes"), cancel_button("No"){
	set_border_width(6);
	set_default_size(200, 80);

	add(main_box);
	main_box.pack_start(top_box);
	main_box.pack_start(bottom_box);

	set_title("Delete Nation");

	top_box.pack_start(text);
	text.set_markup("Are you sure you want to delete this nation?");
	text.set_line_wrap();
	text.set_size_request(200,40);
	bottom_box.pack_start(cancel_button, Gtk::PACK_SHRINK);
	bottom_box.pack_start(delete_button, Gtk::PACK_SHRINK);
	set_focus(cancel_button);
	bottom_box.set_border_width(5);
	bottom_box.set_layout(Gtk::BUTTONBOX_END);

	cancel_button.signal_clicked().connect(sigc::mem_fun(*this, &Delete_Popup::on_button_cancel));
	delete_button.signal_clicked().connect(sigc::mem_fun(*this, &Delete_Popup::on_button_delete));
	show_all_children();
}

void Delete_Popup::on_button_cancel(){
	hide();
}

void Delete_Popup::on_button_delete(){
	hide();
	Nationstates::instance().delete_nation(select_nat);
}

void Delete_Popup::saved(Glib::ustring selected_nation){
	select_nat = selected_nation;
}
