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

#ifndef GTKMM_GTEST_PREFERENCES
#define GTKMM_GTEST_PREFERENCES

#include <gtkmm.h>

class Preferences_Window : public Gtk::Window{
private:

public:

	Preferences_Window();

protected:

	void on_close_button();
	Gtk::Notebook pref_notebook;
	Gtk::Box main_box;
	Gtk::HBox general_tab, test_two, general_tab_hbox_one, general_tab_hbox_two;
	//Gtk::VBox general_tab_vbox_one, general_tab_vbox_two;
	Gtk::ButtonBox bbox_one;
	Gtk::Button confirm_button, close_button;
	Gtk::CheckButton one, two, three;
};

#endif

