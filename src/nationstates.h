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
#include "functions.h"
#include "ploter.h"
#include "add.h"

class Nationstates : public Gtk::Window{
private:

	Nationstates();
	Nationstates(const Nationstates&);
	Nationstates& operator=(const Nationstates&);

public:

	static Nationstates& instance(){
		static Nationstates singleton;
		return singleton;
	}

	void load_compare(Glib::ustring selected_save, Glib::ustring selected_nation);
	void load_main(Glib::ustring selected_save, Glib::ustring selected_nation, int skip_tree_print);
	void load_latest(Glib::ustring nationed);

	void update_event_preview(Glib::ustring selected_save);

	void new_nation(Glib::ustring nationer);
	void delete_nation(Glib::ustring nationer);
	void update_nation(Glib::ustring nationer);

	void force_notebook_refresh(int page);
	void set_notebook_page(int page);

	static std::vector<Glib::ustring> stat_vector;
	static std::vector<double> values_vector;
	std::vector<Glib::ustring> get_stat_vector();
	std::vector<double> get_value_vector();
	std::vector< std::vector<Glib::ustring> > get_colour_vectors();

protected:

	void menu_add_nation();
	void menu_preferences();
	void menu_help();
	void menu_about();
	void menu_update_all();
	void menu_quit();
	void menu_empty();
	void load_preferences();
	void view();
	void view_info_box_hide();

	std::vector<Glib::ustring> view_settings;

	void on_page_switch(Gtk::Widget* page, guint page_num);
	virtual bool on_tree_key(GdkEventKey* event);
	virtual bool on_search_key(GdkEventKey* event);

	std::vector<Glib::ustring> red_colours, green_colours, blue_colours;
	std::vector< std::vector<Glib::ustring> > colours;

	Glib::ustring nation, loaded;

	Gtk::Box main_box, big_box, flag_box;
	Gtk::Entry search_entry;
	Gtk::VBox right_box, v_header, save_box_big, menu_box, mainmain, left_box;
	Gtk::HBox header_box, description_box, issues_box, region_box, graph_box, event_box, save_box, nation_box, header_upper_box, latest_events_box;
	Gtk::ScrolledWindow scrolled_save, scrolled_nation, scrolled_events, scrolled_stats;
	Gtk::Notebook notebook, events_notebook;
	Gtk::Label fullname, events_label, description_label, rights, nation_label;
	Gtk::Image flag_image;
	Glib::RefPtr<Gtk::UIManager> ui_manager;
	Glib::RefPtr<Gtk::ActionGroup> action_group;
	Glib::RefPtr<Gtk::ToggleAction> view_info_box_hide_check;
	Gtk::TextView events_preview, save_comments;
	Glib::RefPtr<Gtk::TextBuffer> events_buffer, save_comments_buffer;
	Glib::RefPtr<Gtk::TextTag> bold_tag;

	Census_Plot plotter;
	functions fun;
	Add_Popup adder;
	Preferences_Window preferences;
	About_Page about;
};

#endif
