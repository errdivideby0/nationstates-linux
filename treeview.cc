#include <iostream>
#include <cstdlib>
#include "treeview.h"
#include "functions.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

Tree_View::Tree_View(){

	TreeModel = Gtk::TreeStore::create(stat_columns);
	set_model(TreeModel);

	Gtk::CellRendererText *render = Gtk::manage(new Gtk::CellRendererText());
	Gtk::TreeView::Column *viewcol = Gtk::manage( new Gtk::TreeView::Column ("Nation Statistic", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_name);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column ("Old", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_value2);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column ("Current", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_value);
	append_column (*viewcol);
	viewcol = Gtk::manage( new Gtk::TreeView::Column ("+/-", *render));
	viewcol->add_attribute (render->property_markup(), stat_columns.stat_update);
	append_column (*viewcol);

	names.clear();

	ifstream read;
	read.open("./stat_names.txt");
	int nlines = -1;
	string nline;
	while (read.good()){
		getline (read,nline);
		nlines++;
		}
	read.close();

	ifstream reader;
	string dBuffer;
	reader.open("./stat_names.txt");
	for(int i=0; i<nlines; i++){
		getline (reader,dBuffer);
		names.push_back(dBuffer);
	}
	reader.close();

}

Tree_View::~Tree_View(){
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

void Tree_View::set_stat_row(Glib::ustring text1, Glib::ustring text2, Glib::ustring text3, Glib::ustring text4){
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
		for(int i=0; i<comparor.at(1).size(); i++){
			append_stat_row();
			if(previous_dates.size()>1){
				for(int j=0; j<comparee.at(1).size(); j++){
					if(comparor.at(1).at(i) == comparee.at(1).at(j)){
						change_value = fun.strouble(comparor.at(1).at(i+1)) - fun.strouble(comparee.at(1).at(j+1));
						if(change_value != 0)
								set_stat_row("<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+comparor.at(1).at(i)+"</b>", "<b>"+comparor.at(1).at(i+1)+"</b>%", "<b>"+comparee.at(1).at(j+1)+"</b>%");
						j = comparee.at(1).size()-1;
					}
				}
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row("", comparor.at(1).at(i), comparor.at(1).at(i+1)+"%", comparor.at(1).at(i+1)+"%");
			else if(previous_dates.size() == 0)
				set_stat_row("", comparor.at(1).at(i), comparor.at(1).at(i+1)+"%", "");
			i++;
		}
	}
	catch(exception& e){
		cout<<"error in deaths\n";
	}

	try{
		append_category_row("Economy");
		for(int i=0; i<comparor.at(5).size(); i++){
			append_stat_row();
			if(previous_dates.size()>1){
				change_value = fun.strouble(comparor.at(5).at(i)) - fun.strouble(comparee.at(2).at(i));
				if(change_value != 0)
					set_stat_row("<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+names.at(80+i)+"</b>", "<b>"+comparor.at(5).at(i)+"</b>%", "<b>"+comparee.at(2).at(i)+"</b>%");
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row("", names.at(80+i), comparor.at(5).at(i)+"%", comparor.at(5).at(i)+"%");
			else if(previous_dates.size() == 0)
				set_stat_row("", names.at(80+i), comparor.at(5).at(i)+"%", "");
		}
	}
	catch(exception& e){
		cout<<"error in Economy\n";
	}

	try{
		append_category_row("Budget");
		for(int i=0; i<comparor.at(4).size(); i++){
			append_stat_row();
			if(previous_dates.size()>1){
				change_value = fun.strouble(fun.trim(comparor.at(4).at(i), 0, 1)) - fun.strouble(fun.trim(comparee.at(3).at(i), 0, 1));
				if(change_value != 0)
					set_stat_row("<b>"+fun.doubstr(change_value)+"</b>%", "<b>"+names.at(69+i)+"</b>", "<b>"+comparor.at(4).at(i)+"</b>", "<b>"+comparee.at(3).at(i)+"</b>");
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row("", names.at(69+i), comparor.at(4).at(i), comparor.at(4).at(i));
			else if(previous_dates.size() == 0)
				set_stat_row("", names.at(69+i), comparor.at(4).at(i), "");
		}
	}
	catch(exception& e){
		cout<<"error in Budget\n";
	}

	try{
		append_category_row("Census Data");
		for(int i=0; i<comparor.at(0).size(); i++){
			append_stat_row();
			if(previous_dates.size()>1){
				change_value = fun.strouble(comparor.at(0).at(i)) - fun.strouble(comparee.at(0).at(i));
				if(change_value != 0)
					set_stat_row("<b>"+fun.doubstr(change_value)+"</b>", "<b>"+names.at(i)+"</b>", "<b>"+comparor.at(0).at(i)+"</b>", "<b>"+comparee.at(0).at(i)+"</b>");
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row("", names.at(i), comparor.at(0).at(i), comparor.at(0).at(i));
			else if(previous_dates.size() == 0)
				set_stat_row("", names.at(i), comparor.at(0).at(i), "");
			if(i == 9)
				i = i + 17;
		}
	}
	catch(exception& e){
		cout<<"error in Census\n";
	}

	try{
		append_category_row("Manufacturing");
		for(int i=9; i<26; i++){
			append_stat_row();
			if(i==9)
				i = 26;
			if(previous_dates.size()>1){
				change_value = fun.strouble(comparor.at(0).at(i)) - fun.strouble(comparee.at(0).at(i));
				if(change_value != 0)
					set_stat_row("<b>"+fun.doubstr(change_value)+"</b>", "<b>"+names.at(i)+"</b>", "<b>"+comparor.at(0).at(i)+"</b>", "<b>"+comparee.at(0).at(i)+"</b>");
			}
			if((change_value==0)&&(previous_dates.size()>1))
				set_stat_row("", names.at(i), comparor.at(0).at(i), comparor.at(0).at(i));
			else if(previous_dates.size() == 0)
				set_stat_row("", names.at(i), comparor.at(0).at(i), "");
			if(i==26)
				i = 10;
		}
	}
	catch(exception& e){
		cout<<"error in Manufacturing\n";
	}
	expand_stat_list();
}



