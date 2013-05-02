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
#include "preferences.h"
#include "about.h"
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

	void new_nation(Glib::ustring nationer);
	void set_notebook_page(int page);
	void compare_to_loaded(Glib::ustring selected_save, Glib::ustring selected_nation);
	void load_main(Glib::ustring selected_save, Glib::ustring selected_nation, int skip_tree_print);
	void update_event_preview(Glib::ustring selected_save);
	void delete_nation(Glib::ustring nationer);
	void fetch(Glib::ustring nationer);
	void compare_latest(Glib::ustring nationed);
	void force_notebook_refresh(int page);
	static std::vector<Glib::ustring> stat_vector;
	static std::vector<double> values_vector;
	std::vector<Glib::ustring> get_stat_vector();
	std::vector<double> get_value_vector();

protected:

	void on_add_nation();
	void on_menu_pref();
	void on_menu_help();
	void on_menu_about();
	void fetch_all();
	void on_menu_file_quit();
	void on_menu_file_new_generic();
	void on_menu_others();
	void on_page_switch(Gtk::Widget* page, guint page_num);
	void load_preferences();

	virtual bool on_tree_key(GdkEventKey* event);
	virtual bool on_search_key(GdkEventKey* event);

	Glib::ustring nation, loaded;

	Gtk::Box main_box, big_box, flag_box;
	Gtk::Entry search_entry;
	Gtk::VBox right_box, v_header, save_box_big, menu_box, mainmain, left_box;
	Gtk::HBox header_box, description_box, issues_box, region_box, graph_box, event_box, save_box, nation_box, header_upper_box, latest_events_box;
	Gtk::ScrolledWindow scrolled_save, scrolled_nation, scrolled_events, scrolled_stats;
	Gtk::Notebook notebook;
	Census_Plot plotter;
	functions fun;
	Add_Popup adder;
	Gtk::Image flag;
	Gtk::Label fullname, events_label, description_label, rights, nation_label;
	Glib::RefPtr<Gtk::UIManager> ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> action_group;
	Gtk::TextView events_preview;
	Glib::RefPtr<Gtk::TextBuffer> events_buffer;
	Glib::RefPtr<Gtk::TextTag> bold_tag;
	Preferences_Window preferences;
	About_Page about;
};

#endif
