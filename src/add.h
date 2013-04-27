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

#ifndef GTKMM_GTEST_ADD
#define GTKMM_GTEST_ADD

#include <gtkmm.h>

class Add_Popup : public Gtk::Window{
private:

public:

	Add_Popup();

protected:

	void on_button_confirm();

	Gtk::Box main_box;
	Gtk::HBox top_box;
	Gtk::ButtonBox bottom_box;
	Gtk::Entry nation_input;
	Gtk::Button confirm_button;
};

#endif
