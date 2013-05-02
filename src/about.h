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

#ifndef GTKMM_GTEST_ABOUT
#define GTKMM_GTEST_ABOUT

#include <gtkmm.h>

class About_Page : public Gtk::Window{

private:

public:

	About_Page();

protected:
	virtual bool on_link_clicked(GdkEventButton* event);
	void on_close_button();
	Gtk::Box main_box;
	Gtk::ButtonBox bbox_one;
	Gtk::VBox vbox_one;
	Gtk::Label label_one, label_two;
	Gtk::Button close_button;

};

#endif

