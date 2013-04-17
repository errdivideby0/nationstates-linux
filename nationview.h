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

#ifndef GTKMM_EXAMPLE_NATIONVIEW
#define GTKMM_EXAMPLE_NATIONVIEW

#include <gtkmm.h>
#include "functions.h"

class Nation_View : public Gtk::TreeView{

private:

	Nation_View();
	Nation_View(const Nation_View&);
	Nation_View& operator=(const Nation_View&);

public:

	static Nation_View& instance(){
		static Nation_View singleton;
		return singleton;
	}

	static Glib::ustring selected_nation;
	void append_nation(Glib::ustring nation_pass, Glib::ustring n_dates);
	void clear_nation_list();
	int number_nation_selected();
	Glib::ustring get_selected_nation();
	void set_selection();


protected:

	virtual bool on_button_press_event(GdkEventButton *ev);

	void pop_show();
	void on_selection_changed();
	void save_title_menu_get();
	void save_title_menu_delete_all();

	class NationColumns : public Gtk::TreeModel::ColumnRecord{
		public:

	 	NationColumns(){
			add(stat_nation);
		}

	 	 Gtk::TreeModelColumn<Glib::ustring> stat_nation;
	};
	NationColumns nation_columns;

	Glib::RefPtr<Gtk::ListStore> nation_model;
	Glib::RefPtr<Gtk::TreeSelection> nation_selection;
	Gtk::TreeModel::Row nation_row, selected_row;
	Gtk::TreeModel::iterator iter;
	Gtk::Menu save_title_menu;
	functions fun;
};

#endif

