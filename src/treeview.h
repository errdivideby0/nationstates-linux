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

#ifndef GTKMM_TREEVIEW
#define GTKMM_TREEVIEW

#include <gtkmm.h>
#include "functions.h"

class Tree_View : public Gtk::TreeView{

public:
	Tree_View();
	virtual ~Tree_View();

	Glib::ustring get_name_at(int place);
	void print_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee, int print_mode);
	void clear_stat_list();
	std::vector<Glib::ustring> get_selected_stat();

protected:

	virtual bool on_button_press_event(GdkEventButton *ev);
	void append_category_row(Glib::ustring category_name);
	void append_stat_row();
	void set_stat_row(int index, Glib::ustring text, Glib::ustring text2, Glib::ustring text3, Glib::ustring text4);
	void expand_stat_list();
	void on_selection_changed();
	void selected_row_callback(const Gtk::TreeModel::iterator& iter);

	class ModelColumns : public Gtk::TreeModel::ColumnRecord{
		public:

		ModelColumns(){
			add(index_number); add(stat_name); add(stat_value); add(stat_value2); add(stat_update);
		}

		Gtk::TreeModelColumn<int> index_number;
		Gtk::TreeModelColumn<Glib::ustring> stat_name;
		Gtk::TreeModelColumn<Glib::ustring> stat_value;
		Gtk::TreeModelColumn<Glib::ustring> stat_value2;
		Gtk::TreeModelColumn<Glib::ustring> stat_update;
	};
	ModelColumns stat_columns;

	Gtk::TreeView TreeView;
	Glib::RefPtr<Gtk::TreeStore> TreeModel;
	Gtk::TreeModel::Row category_row, stat_row, parent_row;
	Gtk::TreeModel::iterator parentrow;
	std::vector<Glib::ustring> names;
	static std::vector<Glib::ustring> stat_vector;
	functions fun;

};

#endif
