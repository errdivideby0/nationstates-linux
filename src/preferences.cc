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

#include "preferences.h"
#include <cstdlib>
#include <gtkmm.h>

using namespace std;

Preferences_Window::Preferences_Window(): main_box(Gtk::ORIENTATION_VERTICAL), confirm_button("Confirm"), close_button("Close"){
	set_border_width(5);
	set_default_size(400, 400);
	set_title("Preferences");
	add(main_box);
	main_box.pack_start(pref_notebook);
	pref_notebook.append_page(general_tab,"General");
	pref_notebook.append_page(test_two,"Test Two");
	general_tab.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	test_two.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	general_tab.add(ltest_one);
	test_two.add(ltest_two);
	main_box.pack_start(bbox_one, Gtk::PACK_SHRINK);
	bbox_one.set_border_width(5);
	bbox_one.set_layout(Gtk::BUTTONBOX_END);
	bbox_one.pack_start(confirm_button, Gtk::PACK_SHRINK);
	bbox_one.pack_start(close_button, Gtk::PACK_SHRINK);
	close_button.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_close_button));
	show_all_children();
}

void on_close_button(){
	hide();
}


