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

#include "functions.h"
#include <gtkmm.h>

class Preferences_Window : public Gtk::Window{
private:

public:

	Preferences_Window();

protected:

	std::vector<Glib::ustring> pref_settings;
	std::vector<Glib::ustring> red;
	std::vector<Glib::ustring> green;
	std::vector<Glib::ustring> blue;
	void on_select_colour();
	void set_pref();
	void on_close_button();
	void on_apply_button();
	void on_info_box_check_clicked();
	void on_baked_potato_clicked();
	void on_not_a_potato_clicked();
	Gtk::Notebook pref_notebook;
	Gtk::Box main_box;
	Gtk::HBox general_tab;
	Gtk::VBox general_tab_vbox_one, general_tab_vbox_two, colour_tab_vbox_one;
	Gtk::ButtonBox bbox_one, colour_tab_button_box_one;
	Gtk::ScrolledWindow colour_tab_scroll_window_one;
	Gtk::Button close_button, select_colour_button;
	Gtk::CheckButton info_box_check, baked_potato, not_a_potato;
	Gtk::ColorButton colour_tab_colour_button;
	Gtk::DrawingArea colour_tab_drawing_area;

	Gdk::RGBA colour_tab_rgba;

	functions fun;
};

#endif

