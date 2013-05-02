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
#include "rename.h"

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

	void append_save(Glib::ustring text);
	void refresh_saves();
	void save_menu_rename(Glib::ustring newname);
	Glib::ustring get_selected_save();

protected:

	virtual bool on_button_press_event(GdkEventButton *ev);
	virtual bool on_key_press_event(GdkEventKey *ev);

	void compare();
	void load();
	void save_menu_delete();
	void show_rename_popup();
	void on_save_changed();

	class SaveColumns : public Gtk::TreeModel::ColumnRecord{
		public:

	 	SaveColumns(){
			add(stat_save);
		}

	 	 Gtk::TreeModelColumn<Glib::ustring> stat_save;
	};
	SaveColumns save_columns;

	static Glib::ustring selected_save;

	Glib::RefPtr<Gtk::ListStore> save_model;
	Glib::RefPtr<Gtk::TreeSelection> selection;
	Gtk::TreeModel::Row save_row, selected_row;
	Gtk::TreeModel::iterator parentrow, iter;
	Gtk::Menu save_menu, save_title_menu;
	functions fun;
	rename_popup pop;
};

#endif
