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

#include "nationview.h"
#include "gtest.h"
#include "saveview.h"
#include <fstream>

using namespace std;

Glib::ustring Nation_View::selected_nation;

Nation_View::Nation_View(){

	nation_model = Gtk::ListStore::create(nation_columns);
	set_model(nation_model);
	append_column("Nation", nation_columns.stat_nation);

	Gtk::MenuItem* title_item = Gtk::manage(new Gtk::MenuItem("Get Latest Data", true));
	title_item->signal_activate().connect(sigc::mem_fun(*this, &Nation_View::save_title_menu_get));
	save_title_menu.append(*title_item);
	title_item = Gtk::manage(new Gtk::MenuItem("Delete Nation's Data", true));
	title_item->signal_activate().connect(sigc::mem_fun(*this, &Nation_View::save_title_menu_delete_all));
	save_title_menu.append(*title_item);

	save_title_menu.accelerate(*this);
	save_title_menu.show_all();

	signal_button_press_event().connect(sigc::mem_fun(*this, &Nation_View::on_button_press_event));
	get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Nation_View::on_selection_changed));
}

bool Nation_View::on_button_press_event(GdkEventButton* event){

	bool return_value = false;
	return_value = TreeView::on_button_press_event(event);

	if((event->type == GDK_BUTTON_PRESS) && (event->button == 3) && (selected_row->parent() == 0))
		save_title_menu.popup(event->button, event->time);

	return return_value;
}

void Nation_View::on_selection_changed(){
	if(get_selection()->count_selected_rows() >= 1){
		selected_nation.clear();
		iter = get_selection()->get_selected();
		selected_row = *iter;
		Glib::ustring nname = selected_row[nation_columns.stat_nation];
		selected_nation = nname.substr(0, nname.find(' '));
		gTest::instance().refresh_saves();
	}
}

void Nation_View::clear_nation_list(){
	nation_model->clear();
}

Glib::ustring Nation_View::get_selected_nation(){
	return selected_nation;
}

void Nation_View::append_nation(Glib::ustring nation_pass, Glib::ustring n_dates){
	nation_row = *(nation_model->append());
	nation_row[nation_columns.stat_nation] = nation_pass+" ("+n_dates+")";
}

int Nation_View::number_nation_selected(){
	return get_selection()->count_selected_rows();
}

void Nation_View::save_title_menu_delete_all(){
	gTest::instance().goto_delete_all(selected_nation);
}

void Nation_View::save_title_menu_get(){
	gTest::instance().goto_get_all(selected_nation);
}
