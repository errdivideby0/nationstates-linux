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
	reader.open("./name-store/blank_names.txt");
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

void Tree_View::append_all(vector<Glib::ustring> main_vector, vector<Glib::ustring> compare, int shift){
	double change_value = 0.0;
	for( int i=0; i<main_vector.size(); i++){
		append_stat_row();
		change_value = fun.strouble(main_vector.at(i)) - fun.strouble(compare.at(i));
		if(change_value != 0)
			set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>", "<b>"+names.at(i+shift)+"</b>", "<b>"+main_vector.at(i)+"</b>", "<b>"+compare.at(i)+"</b>");
		else
			set_stat_row(i, "", names.at(i+shift), main_vector.at(i), main_vector.at(i));
	}
}

void Tree_View::print_data(Glib::ustring main_nation, Glib::ustring main_save, Glib::ustring compare_nation, Glib::ustring compare_save, int print_mode){

	clear_stat_list();
	double change_value = 0.0;

	try{
		vector<Glib::ustring> main_deaths = fun.load_data(main_nation, main_save, 3);
		vector<Glib::ustring> compare_deaths = fun.load_data(compare_nation, compare_save, 3);
		append_category_row("Deaths");
		for(int i=0; i<main_deaths.size(); i++){
			append_stat_row();
			bool find = false;
			for(int j=0; j<compare_deaths.size(); j++){
				if(main_deaths.at(i) == compare_deaths.at(j)){
					change_value = fun.strouble(main_deaths.at(i+1)) - fun.strouble(compare_deaths.at(j+1));
					if(change_value != 0)
						set_stat_row(i, "<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+main_deaths.at(i)+"</b>", "<b>"+main_deaths.at(i+1)+"</b>%", "<b>"+compare_deaths.at(j+1)+"</b>%");
					else
						set_stat_row(i, "", main_deaths.at(i), main_deaths.at(i+1)+"%", main_deaths.at(i+1)+"%");
					j = compare_deaths.size()-1;
					find = true;
				}
			}
			if(find == false)
				set_stat_row(i, "<b>-"+main_deaths.at(i+1)+"</b>%", main_deaths.at(i), main_deaths.at(i+1)+"%", "0%");
			i++;
		}
	}
	catch(exception& e){
		cout<<"error in deaths\n";
	}
	append_category_row("Economy");
	vector<Glib::ustring> eco_main = fun.load_data(main_nation, main_save, 8);
	vector<Glib::ustring> eco_compare = fun.load_data(compare_nation, compare_save, 8);
	for( int i=0; i<eco_main.size(); i++){
		append_stat_row();
		change_value = fun.strouble(eco_main.at(i)) - fun.strouble(eco_compare.at(i));
		if(change_value != 0)
			set_stat_row(i, "<b>"+fun.doubstr(change_value)+"%</b>", "<b>"+names.at(i+10)+"</b>", "<b>"+eco_main.at(i)+"%</b>", "<b>"+eco_compare.at(i)+"%</b>");
		else
			set_stat_row(i, "", names.at(i+10), eco_main.at(i)+"%", eco_main.at(i)+"%");
	}

	append_category_row("Budget");
	append_all(fun.load_data(main_nation, main_save, 7), fun.load_data(compare_nation, compare_save, 7), 13);

	append_category_row("Census Data");
	append_all(fun.load_data(main_nation, main_save, 1), fun.load_data(compare_nation, compare_save, 1), 24);

	append_category_row("Manufacturing");
	append_all(fun.load_data(main_nation, main_save, 2), fun.load_data(compare_nation, compare_save, 2), 76);

	expand_stat_list();
	columns_autosize();
}

void Tree_View::print_blank(){

	clear_stat_list();

	append_category_row("Deaths");
	for(int i=0; i<10; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Economy");
	for(int i=10; i<13; i++){
		append_stat_row();
		stat_row[stat_columns.stat_name] = names.at(i);
	}

	append_category_row("Budget");
	for(int i=13; i<24; i++){
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
	columns_autosize();
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

void Tree_View::print_hide(Glib::ustring search_text){
	cout<<search_text<<"\n";
}
