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

#ifndef GTKMM_GTEST
#define GTKMM_GTEST

#include <gtkmm.h>
#include "treeview.h"
#include "functions.h"
#include "ploter.h"
#include "add.h"

class gTest : public Gtk::Window{
private:

	gTest();
	gTest(const gTest&);
	gTest& operator=(const gTest&);

public:

	static gTest& instance(){
		static gTest singleton;
		return singleton;
	}

	void on_menu_new(Glib::ustring nationer);
	void goto_data(std::vector<Glib::ustring> nation_data);
	void goto_load(std::vector<Glib::ustring> nation_data);
	void update_latest_events(Glib::ustring selected_save);
	void goto_delete_all(Glib::ustring nationer);
	void goto_get_all(Glib::ustring nationer);
	void compare_latest(Glib::ustring nationed);
	void force_notebook_refresh(int page);
	void refresh_saves();
	static std::vector<Glib::ustring> stat_vector;
	static std::vector<double> values_vector;
	std::vector<Glib::ustring> get_stat_vector();
	std::vector<double> get_value_vector();
	std::vector<Glib::ustring> get_units();

protected:

	void on_menu_new_window();
	void on_menu_pref();
	void on_menu_help();
	void on_menu_about();
	void on_menu_update_all();
	void on_menu_file_quit();
	void on_menu_file_new_generic();
	void on_menu_others();
	void on_notebook_switch_page(Gtk::Widget* page, guint page_num);

	std::vector< std::vector<Glib::ustring> > data_vectors;
	Glib::ustring current_time, nation;

	Gtk::Box main_box, big_box, flag_box;
	Gtk::VBox right_box, v_header, save_box_big, menu_box, mainmain;
	Gtk::HBox header_box, description_box, issues_box, region_box, graph_box, event_box, save_box, nation_box, header_upper_box, latest_events_box;
	Gtk::ScrolledWindow scrolled_save, scrolled_nation, scrolled_events, scrolled_stats;
	Gtk::Notebook notebook;
	Tree_View stats;
	Census_Plot plotter;
	functions fun;
	Add_Popup adder;
	Gtk::Image flag;
	Gtk::Label fullname, events_label, description_label, rights, nation_label, latest_events_label;
	Glib::RefPtr<Gtk::UIManager> ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> action_group;
};

#endif
