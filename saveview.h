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

#ifndef GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H
#define GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H

#include <gtkmm.h>
#include "functions.h"
#include "popup.h"

class Save_View : public Gtk::TreeView{

private:

	Save_View();
	Save_View(const Save_View&);
	Save_View& operator=(const Save_View&);

public:

	static Save_View& instance(){
		static Save_View singleton;
		return singleton;
	}

	void append_nation(Glib::ustring nation_pass, Glib::ustring n_dates);
	void clear_save_list();
	void append_row();
	void set_row(Glib::ustring text);
	int number_selected();
	void expand_category();
	void save_menu_rename(Glib::ustring);

protected:

	virtual bool on_button_press_event(GdkEventButton *ev);

	void save_menu_load();
	void save_menu_delete();
	void pop_show();
	void save_menu_print();
	void save_title_menu_get();
	void save_title_menu_delete_all();

	class SaveColumns : public Gtk::TreeModel::ColumnRecord{
		public:

	 	SaveColumns(){
			add(stat_date);
		}

	 	 Gtk::TreeModelColumn<Glib::ustring> stat_date;
	};
	SaveColumns save_columns;

	static std::vector<Glib::ustring> nation_data;

	Glib::RefPtr<Gtk::TreeStore> SaveModel;
	Glib::RefPtr<Gtk::TreeSelection> selection;
	Gtk::TreeModel::Row nation_row, save_row, selected_row, parent_row;
	Gtk::TreeModel::iterator parentrow, iter;
	Gtk::Menu save_menu, save_title_menu;
	functions fun;
	popup pop;
};

#endif
