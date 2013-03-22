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

#include "saveview.h"
#include "gtest.h"
#include <iostream>

using namespace std;

std::vector<Glib::ustring> Save_View::nation_data;

Save_View::Save_View(){

	SaveModel = Gtk::TreeStore::create(save_columns);
	set_model(SaveModel);
	append_column("Date", save_columns.stat_date);

	Gtk::MenuItem*  item = Gtk::manage(new Gtk::MenuItem("Compare", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_print));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Load", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_load));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Delete", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_delete));
	save_menu.append(*item);

	Gtk::MenuItem* title_item = Gtk::manage(new Gtk::MenuItem("Get Latest Data", true));
	title_item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_title_menu_get));
	save_title_menu.append(*title_item);
	title_item = Gtk::manage(new Gtk::MenuItem("Delete Nation's Data", true));
	title_item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_title_menu_delete_all));
	save_title_menu.append(*title_item);

	save_title_menu.accelerate(*this);
	save_title_menu.show_all();

	save_menu.accelerate(*this);
	save_menu.show_all();

	signal_button_press_event().connect(sigc::mem_fun(*this, &Save_View::on_button_press_event));
}

Save_View::~Save_View(){
}

bool Save_View::on_button_press_event(GdkEventButton* event){

	bool return_value = false;
	return_value = TreeView::on_button_press_event(event);

	nation_data.clear();
	// Get the selected row that was clicked
	selection = get_selection();
	iter = selection->get_selected();
	selected_row = *iter;

	// If user has clicked a nation date in the nation tree
	if((event->type == GDK_BUTTON_PRESS) && (event->button == 3) && (selected_row->parent())){
		Glib::ustring ndate = selected_row[save_columns.stat_date];
		nation_data.push_back(ndate + ".txt");
		parentrow = selected_row->parent();
		parent_row = *parentrow;
		Glib::ustring nname = parent_row[save_columns.stat_date];
		nation_data.push_back(nname.substr(0, nname.find(' ')));
		save_menu.popup(event->button, event->time);
	}
	// If user has clicked a row without a parent (the nation)
	else if((event->type == GDK_BUTTON_PRESS) && (event->button == 3) && (selected_row->parent() == 0)){
		Glib::ustring nname = selected_row[save_columns.stat_date];
		nation_data.push_back(nname.substr(0, nname.find(' ')));
		save_title_menu.popup(event->button, event->time);
	}
	return return_value;
}

void Save_View::save_menu_load(){
	if(nation_data.at(0).at(0) == '*')
		nation_data.at(0) = fun.trim(nation_data.at(0), 1, 0);
	gTest::instance().goto_load(nation_data);
}

void Save_View::save_menu_nothing(){
}

void Save_View::save_menu_delete(){
}

void Save_View::clear_save_list(){
	SaveModel->clear();
}

void Save_View::append_nation(Glib::ustring nation_pass, Glib::ustring n_dates){
	nation_row = *(SaveModel->append());
	nation_row[save_columns.stat_date] = nation_pass+" ("+n_dates+")";
}

void Save_View::append_row(){
	save_row = *(SaveModel->append(nation_row.children()));
}

void Save_View::set_row(Glib::ustring text){
	save_row[save_columns.stat_date] = text;
}

int Save_View::number_selected(){
	return get_selection()->count_selected_rows();
}

void Save_View::expand_category(){
	//expand_row(get_path(save_row.parent()), true);
}

void Save_View::save_menu_print(){
	if(nation_data.at(0).at(0) == '*')
		nation_data.at(0) = fun.trim(nation_data.at(0), 1, 0);
	gTest::instance().goto_data(nation_data);
}

void Save_View::save_title_menu_delete_all(){
	gTest::instance().goto_delete_all(nation_data.at(0));
}

void Save_View::save_title_menu_get(){
	gTest::instance().goto_get_all(nation_data.at(0));
}
