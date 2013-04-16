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

popup::popup(): main_box(Gtk::ORIENTATION_VERTICAL), confirm_button("Confirm"){

	set_border_width(0);
	set_default_size(200, 40);

	add(main_box);
	main_box.pack_start(top_box);
	main_box.pack_start(bottom_box);

	set_title("Rename Save");

	top_box.pack_start(rename_input);
	bottom_box.pack_start(confirm_button, Gtk::PACK_SHRINK);
	rename_input.set_placeholder_text("type save name");
	bottom_box.set_border_width(5);
	bottom_box.set_layout(Gtk::BUTTONBOX_END);

	confirm_button.signal_clicked().connect(sigc::mem_fun(*this, &popup::on_button_confirm));
	show_all_children();
}

void popup::on_button_confirm(){
	hide();
	Save_View::instance().save_menu_rename(rename_input.get_text());
}
