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

#include <iostream>
#include <cstdlib>
#include "treeview.h"
#include "functions.h"
#include "gtest.h"
#include <fstream>

std::vector<Glib::ustring> Tree_View::stat_vector;

using namespace std;

Tree_View::Tree_View(){

	TreeModel = Gtk::TreeStore::create(stat_columns);
	set_model(TreeModel);

	Gtk::CellRendererText *render = Gtk::manage(new Gtk::CellRendererText());
	Gtk::TreeView::Column *viewcol = Gtk::manage( new Gtk::TreeView::Column (" Nation Statistic", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_name);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column (" A", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_value2);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column (" B", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_value);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column (" +/-", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_update);
	append_column (*viewcol);
	names.clear();

	signal_button_press_event().connect(sigc::mem_fun(*this, &Tree_View::on_button_press_event));
	get_selection()->signal_changed().connect( sigc::mem_fun(*this, &Tree_View::on_selection_changed));
	get_selection()->set_mode(Gtk::SELECTION_MULTIPLE);

	ifstream reader;
	string dBuffer;
	reader.open("./name-store/stat_names.txt");
	while(getline(reader, dBuffer)){
		names.push_back(dBuffer);
	}
	reader.close();
}

bool Tree_View::on_button_press_event(GdkEventButton* event){
	bool return_value = false;
	return_value = TreeView::on_button_press_event(event);

	if(event->type == GDK_2BUTTON_PRESS)
		gTest::instance().set_notebook_page(3);

	return return_value;
}

void Tree_View::clear_stat_list(){
	TreeModel->clear();
}

void Tree_View::expand_stat_list(){
	expand_all();
}

void Tree_View::append_category_row(Glib::ustring category_name){
	category_row = *(TreeModel->append());
	category_row[stat_columns.stat_name] = category_name;
}

void Tree_View::append_stat_row(){
	stat_row = *(TreeModel->append(category_row.children()));
}

void Tree_View::set_stat_row(int index, Glib::ustring text1, Glib::ustring text2, Glib::ustring text3, Glib::ustring text4){
	stat_row[stat_columns.index_number] = index;
	stat_row[stat_columns.stat_update] = text1;
	stat_row[stat_columns.stat_name] = text2;
	stat_row[stat_columns.stat_value] = text3;
	stat_row[stat_columns.stat_value2] = text4;
}

Glib::ustring Tree_View::get_name_at(int place){
	return names.at(place);
}

void Tree_View::print_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee, int print_mode){

	std::vector<Glib::ustring> previous_dates;
	double change_value = 0;

	clear_stat_list();
	if(print_mode == 1){
		previous_dates.push_back("one");
		previous_dates.push_back("two");
	}

	try{
		append_category_row("Deaths");
		for( int i=0; i<comparor.at(1).size(); i++){
			append_stat_row();
			if(previous_dates.size()>1){
				for( int j=0; j<comparee.at(1).size(); j++){
					if(comparor.at(1).at(i) == comparee.at(1).at(j)){
						change_value = fun.strouble(comparor.at(1).at(i+1)) - fun.strouble(comparee.at(1).at(j+1));
						if(change_value != 0)
								set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+comparor.at(1).at(i)+"</b>", "<b>"+comparor.at(1).at(i+1)+"</b>%", "<b>"+comparee.at(1).at(j+1)+"</b>%");
						j = comparee.at(1).size()-1;
					}
				}
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row(i, "", comparor.at(1).at(i), comparor.at(1).at(i+1)+"%", comparor.at(1).at(i+1)+"%");
			else if(previous_dates.size() == 0)
				set_stat_row(i, "", comparor.at(1).at(i), comparor.at(1).at(i+1)+"%", "");
			i++;
		}
	}
	catch(exception& e){
		cout<<"error in deaths\n";
	}
	append_category_row("Economy");
	for( int i=0; i<comparor.at(5).size(); i++){
		append_stat_row();
		if(previous_dates.size()>1){
			change_value = fun.strouble(comparor.at(5).at(i)) - fun.strouble(comparee.at(2).at(i));
			if(change_value != 0)
				set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+names.at(80+i)+"</b>", "<b>"+comparor.at(5).at(i)+"</b>%", "<b>"+comparee.at(2).at(i)+"</b>%");
		}
		if((change_value==0)&&(previous_dates.size()>1))
			set_stat_row(i, "", names.at(80+i), comparor.at(5).at(i)+"%", comparor.at(5).at(i)+"%");
		else if(previous_dates.size() == 0)
			set_stat_row(i, "", names.at(80+i), comparor.at(5).at(i)+"%", "");
	}
	append_category_row("Budget");
	for( int i=0; i<comparor.at(4).size(); i++){
		append_stat_row();
		if(previous_dates.size()>1){
			change_value = fun.strouble(fun.trim(comparor.at(4).at(i), 0, 1)) - fun.strouble(fun.trim(comparee.at(3).at(i), 0, 1));
			if(change_value != 0)
				set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+names.at(69+i)+"</b>", "<b>"+comparor.at(4).at(i)+"</b>", "<b>"+comparee.at(3).at(i)+"</b>");
		}
		if((change_value==0)&&(previous_dates.size()>1))
			set_stat_row(i, "", names.at(69+i), comparor.at(4).at(i), comparor.at(4).at(i));
		else if(previous_dates.size() == 0)
			set_stat_row(i, "", names.at(69+i), comparor.at(4).at(i), "");
	}
	append_category_row("Census Data");
	for( int i=0; i<comparor.at(0).size(); i++){
		append_stat_row();
		if(previous_dates.size()>1){
			change_value = fun.strouble(comparor.at(0).at(i)) - fun.strouble(comparee.at(0).at(i));
			if(change_value != 0)
				set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>", "<b>"+names.at(i)+"</b>", "<b>"+comparor.at(0).at(i)+"</b>", "<b>"+comparee.at(0).at(i)+"</b>");
		}
		if((change_value==0)&&(previous_dates.size()>1))
			set_stat_row(i, "", names.at(i), comparor.at(0).at(i), comparor.at(0).at(i));
		else if(previous_dates.size() == 0)
			set_stat_row(i, "", names.at(i), comparor.at(0).at(i), "");
		if(i == 9)
			i = i + 17;
	}
	append_category_row("Manufacturing");
	for( int i=9; i<26; i++){
		append_stat_row();
		if(i==9)
			i = 26;
		if(previous_dates.size()>1){
			change_value = fun.strouble(comparor.at(0).at(i)) - fun.strouble(comparee.at(0).at(i));
			if(change_value != 0)
				set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>", "<b>"+names.at(i)+"</b>", "<b>"+comparor.at(0).at(i)+"</b>", "<b>"+comparee.at(0).at(i)+"</b>");
		}
		if((change_value==0)&&(previous_dates.size()>1))
			set_stat_row(i, "", names.at(i), comparor.at(0).at(i), comparor.at(0).at(i));
		else if(previous_dates.size() == 0)
			set_stat_row(i, "", names.at(i), comparor.at(0).at(i), "");
		if(i==26)
			i = 9;
		}
	expand_stat_list();
}

void Tree_View::print_blank(){

	vector<Glib::ustring> names = fun.read("./name-store/blank_names.txt");

	append_category_row("Deaths");
	for(int i=0; i<10; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Economy");
	for(int i=10; i<14; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Budget");
	for(int i=14; i<24; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Census Data");
	for( int i=24; i<76; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Manufacturing");
	for( int i=76; i<93; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}
	expand_stat_list();
}

void Tree_View::selected_row_callback(const Gtk::TreeModel::iterator& iter){
	if(iter){
		Gtk::TreeModel::Row selected_row = *iter;
		if((selected_row->parent())&&(to_string(selected_row[stat_columns.index_number]).length()>0)){
			Glib::ustring name = selected_row[stat_columns.stat_name];
			Glib::ustring stat = to_string(selected_row[stat_columns.index_number]);
			parent_row = *(selected_row->parent());
			Glib::ustring category = parent_row[stat_columns.stat_name];
			stat_vector.push_back(name);
			stat_vector.push_back(category);
			stat_vector.push_back(stat);
		}
	}
}

void Tree_View::on_selection_changed(){
	//if(get_selection()->count_selected_rows()<=1)
		//srand(time(NULL));
	stat_vector.clear();
	get_selection()->selected_foreach_iter(sigc::mem_fun(*this, &Tree_View::selected_row_callback));
	gTest::instance().force_notebook_refresh(3);
}

std::vector<Glib::ustring> Tree_View::get_selected_stat(){
	return stat_vector;
}
