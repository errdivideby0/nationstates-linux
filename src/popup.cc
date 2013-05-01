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

#include "popup.h"
#include "saveview.h"
#include <cstdlib>
#include <gtkmm.h>

using namespace std;

popup::popup(): main_box(Gtk::ORIENTATION_VERTICAL), confirm_button("Confirm"), cancel_button("Cancel"){

	set_border_width(6);

	add(main_box);
	main_box.pack_start(top_box);
	main_box.pack_start(bottom_box);

	set_title("Rename Save");

	top_box.pack_start(rename_input);
	bottom_box.pack_start(cancel_button, Gtk::PACK_SHRINK);
	bottom_box.pack_start(confirm_button, Gtk::PACK_SHRINK);
	set_focus_child(rename_input);
	bottom_box.set_border_width(5);
	bottom_box.set_layout(Gtk::BUTTONBOX_END);

	cancel_button.signal_clicked().connect(sigc::mem_fun(*this, &popup::on_button_cancel));
	confirm_button.signal_clicked().connect(sigc::mem_fun(*this, &popup::on_button_confirm));
	signal_show().connect(sigc::mem_fun(*this, &popup::set_rename_text));
	rename_input.signal_activate().connect(sigc::mem_fun(*this, &popup::on_button_confirm));
	show_all_children();
}

void popup::on_button_cancel(){
	hide();
}

void popup::on_button_confirm(){
	hide();
	Save_View::instance().save_menu_rename(rename_input.get_text());
}

void popup::set_rename_text(){
	string name = Save_View::instance().get_selected_save();
	rename_input.set_text(name);
	rename_input.select_region(0, name.length());
}

void popup::set_width(){
	string name = Save_View::instance().get_selected_save();
	int text_width, text_height;
	create_pango_layout(name)->get_pixel_size(text_width, text_height);
	if(text_width<200)
		text_width = 200;
	set_default_size(text_width*1.2, 60);
	set_focus(rename_input);
}
