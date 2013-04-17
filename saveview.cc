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
#include "nationview.h"
#include <fstream>

using namespace std;

Glib::ustring Save_View::selected_save;

Save_View::Save_View(){

	save_model = Gtk::ListStore::create(save_columns);
	set_model(save_model);
	append_column("Date", save_columns.stat_save);

	Gtk::MenuItem*  item = Gtk::manage(new Gtk::MenuItem("Compare", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_print));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Load", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_load));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Rename", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::pop_show));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Delete", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_delete));
	save_menu.append(*item);

	save_menu.accelerate(*this);
	save_menu.show_all();

	signal_button_press_event().connect(sigc::mem_fun(*this, &Save_View::on_button_press_event));
	get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Save_View::on_save_changed));
}

bool Save_View::on_button_press_event(GdkEventButton* event){
	bool return_value = false;
	return_value = TreeView::on_button_press_event(event);

	if((event->type == GDK_BUTTON_PRESS) && (event->button == 3))
		save_menu.popup(event->button, event->time);

	return return_value;
}

void Save_View::on_save_changed(){
	if(get_selection()->count_selected_rows() >= 1){
		selected_save.clear();
		iter = get_selection()->get_selected();
		selected_row = *iter;
		Glib::ustring ndate = selected_row[save_columns.stat_save];
		selected_save = ndate + ".txt";
		try{
			gTest::instance().update_latest_events(selected_save);
		}
		catch(exception e){}
	}
}

void Save_View::save_menu_load(){
	if(selected_save.at(0) == '*')
		selected_save = fun.trim(selected_save, 1, 0);
	vector<Glib::ustring> nation_data;
	nation_data.push_back(selected_save);
	nation_data.push_back(Nation_View::instance().selected_nation);
	gTest::instance().goto_load(nation_data);
}

void Save_View::pop_show(){
	pop.show();
}

Glib::ustring Save_View::get_selected_save(){
	return selected_save.substr(0, selected_save.length()-4);
}

void Save_View::save_menu_rename(Glib::ustring newname){
	vector<Glib::ustring> datelist = fun.read("./nations-store/"+Nation_View::instance().selected_nation+"/datelog.txt");
	if(selected_save.at(0) == '*')
		selected_save = fun.trim(selected_save, 1, 0);
	int datelistsize = datelist.size();
	for(int i=0; i<datelistsize; i++){
		if(selected_save == datelist.at(i)){
			datelist.at(i) = newname+".txt";
			int result = rename(fun.strchar("./nations-store/"+Nation_View::instance().selected_nation+"/"+selected_save), fun.strchar("./nations-store/"+Nation_View::instance().selected_nation+"/"+datelist.at(i)));
			break;
		}
	}

	ofstream savedate;
	savedate.open(fun.strchar("./nations-store/"+Nation_View::instance().selected_nation+"/datelog.txt"));
	for(int i=0; i<datelistsize; i++)
		savedate<<datelist.at(i)<<"\n";
	savedate.close();

	gTest::instance().refresh_saves();
}

void Save_View::save_menu_delete(){
}

void Save_View::clear_save_list(){
	save_model->clear();
}

void Save_View::append_save(Glib::ustring text){
	save_row = *(save_model->append());
	save_row[save_columns.stat_save] = text;
}

int Save_View::number_selected(){
	return get_selection()->count_selected_rows();
}

void Save_View::save_menu_print(){
	if(selected_save.at(0) == '*')
		selected_save = fun.trim(selected_save, 1, 0);
	vector<Glib::ustring> nation_data;
	nation_data.push_back(selected_save);
	nation_data.push_back(Nation_View::instance().selected_nation);
	gTest::instance().goto_data(nation_data);
}
