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

#include "gtest.h"
#include "saveview.h"
#include "nationview.h"
#include <cstdlib>
#include <iostream>
#include <exception>
#include <gtkmm.h>
#include <string>

std::vector<Glib::ustring> gTest::stat_vector;
std::vector<double> gTest::values_vector;

using namespace std;

gTest::gTest(): main_box(Gtk::ORIENTATION_HORIZONTAL), next_button("Next"){

	set_border_width(0);
	set_default_size(1040, 620);

	update_button.set_label("Update All");

	Save_View& saves = Save_View::instance();
	Nation_View& nations = Nation_View::instance();

	scrolled_save.add(saves);
	scrolled_save.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_nation.add(nations);
	scrolled_nation.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	scrolled_events.add(event_box);
	scrolled_events.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	add(main_box);
	main_box.pack_start(scrolled_stats);
	main_box.pack_start(right_box);

	scrolled_stats.add(stats);
	scrolled_stats.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_stats.set_size_request(440, 570);

	right_box.set_size_request(600, 570);
	right_box.pack_start(v_header, Gtk::PACK_SHRINK);
	right_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET);
	right_box.pack_start(input_box, Gtk::PACK_SHRINK);

	v_header.set_size_request(600, 100);
	v_header.pack_start(header_upper_box);
	v_header.pack_start(header_box);

	notebook.append_page(description_box, "Description");
	notebook.append_page(issues_box, "Issues");
 	notebook.append_page(region_box, "Region");
	notebook.append_page(graph_box, "Census Graph");
	notebook.append_page(scrolled_events, "Events");
	notebook.append_page(save_box, "Saved Data");

	save_box.pack_start(scrolled_nation);
	save_box.pack_start(scrolled_save);

	graph_box.pack_start(plotter);

	header_upper_box.set_border_width(5);
	header_upper_box.pack_start(nation_label);

	header_box.pack_start(flag, Gtk::PACK_SHRINK);
	header_box.pack_start(nation_box, Gtk::PACK_EXPAND_WIDGET);

	nation_label.set_selectable(true);
	nation_label.set_markup("<b><big>Nation</big></b>");
	nation_label.set_justify(Gtk::JUSTIFY_CENTER);

	fullname.set_selectable(true);
	fullname.set_markup("\"Motto\"\nCategory\nPopulation:");
	fullname.set_justify(Gtk::JUSTIFY_LEFT);
	fullname.set_line_wrap();

	rights.set_selectable(true);
	rights.set_markup("Civil Rights:   ( )\nEconomy:   ( )\nPolitical Freedom:    ( )\nInfluence:    ");
	rights.set_justify(Gtk::JUSTIFY_RIGHT);

	description_box.pack_start(description_label, Gtk::PACK_SHRINK);
	description_box.set_valign(Gtk::ALIGN_START);
	description_box.set_halign(Gtk::ALIGN_START);
	description_box.set_border_width(8);
	description_label.set_line_wrap();

	event_box.set_valign(Gtk::ALIGN_START);
	event_box.set_halign(Gtk::ALIGN_START);
	event_box.pack_start(events_label);
	event_box.set_border_width(8);
	events_label.set_line_wrap();

	nation_box.pack_start(fullname);
	nation_box.pack_start(rights);
	nation_box.set_border_width(5);

	set_title("NationStates");
	nation_input.set_placeholder_text("Enter Nation Name");

	input_box.pack_start(nation_input);
	input_box.pack_start(next_button, Gtk::PACK_SHRINK);
	input_box.pack_start(update_button);
	input_box.set_border_width(5);
	input_box.set_layout(Gtk::BUTTONBOX_END);

	next_button.signal_clicked().connect(sigc::mem_fun(*this, &gTest::on_button_next));
	update_button.signal_clicked().connect(sigc::mem_fun(*this, &gTest::on_button_update));
	notebook.signal_switch_page().connect(sigc::mem_fun(*this, &gTest::on_notebook_switch_page));

	show_all_children();
}

void gTest::on_button_next(){

	nation = fun.lowercase(nation_input.get_text());

	errorPopup = fun.error_setup();
	errorPopup->set_transient_for(*this);

	if(nation.length() > 0){

		int nation_size = fun.get_nation_data(nation);

		if(nation_size>69){

			current_time.clear();
			current_time = fun.get_time(0, false)+"-"+fun.get_time(1, false)+".txt";

			xmlpp::DomParser parser;
			parser.parse_file("./nation.xml");

			std::vector<Glib::ustring> all_data;
			all_data = fun.print_node(parser.get_document()->get_root_node(), all_data);

			fun.save_data(all_data, current_time, nation);

			data_vectors.clear();
			data_vectors = fun.vectors_generate(all_data, nation);

			//loads the latest data saved
			std::vector<Glib::ustring> nation_datar;
			nation_datar.push_back(fun.read("./nations-store/"+nation+"/datelog.txt").back());
			nation_datar.push_back(nation);
			goto_load(nation_datar);

			std::vector< std::vector<Glib::ustring> > last_vectors;
			int lines = fun.count_lines("./nations-store/"+nation+"/datelog.txt");
			if(lines>1){
				last_vectors = fun.last_vectors_generate(fun.read("./nations-store/"+nation+"/"+fun.read("./nations-store/"+nation+"/datelog.txt").at(lines-2)));
				stats.print_data(data_vectors, last_vectors, 1);
			}
			else
				stats.print_data(data_vectors, last_vectors, 0);
		}
		else{
			if(nation_size>0)
				fun.set_error(errorPopup, "Invalid Nation", "No nation found with name "+nation+".");
			else
				fun.set_error(errorPopup, "Could Not Reach Server", "Please check your internet connection and try again.");
			set_title("NationStates");
		}
	}
	else{
		fun.set_error(errorPopup, "No Nation Input", "Please type a nation in the box.");
		set_title("NationStates"); }
}

void gTest::on_notebook_switch_page(Gtk::Widget*, guint page_num){
	std::vector<Glib::ustring> previous_dates;
	std::vector< std::vector<Glib::ustring> > temp_vectors;
	if(page_num == 5){
		Nation_View& nations = Nation_View::instance();
		std::vector<Glib::ustring> nation_list = fun.read("./name-store/nation_list.txt");

		if(nation_list.size()>0){
			nations.clear_nation_list();
			for(int j=0; j<nation_list.size(); j++){
				previous_dates.clear();
				previous_dates = fun.read("./nations-store/"+nation_list.at(j)+"/datelog.txt");
				nations.append_nation(nation_list.at(j), std::to_string(previous_dates.size()));
			}
		update_button.show();
		}
	}
	else if(page_num == 3){
		update_button.hide();
		stat_vector.clear();
		values_vector.clear();
		stat_vector = stats.get_selected_stat();

		if(stat_vector.size()>1){
			for(int j=0; j<stat_vector.size()/3; j++){
				previous_dates.clear();
				Glib::ustring name = stat_vector.at((j*3)+0);
				std::vector<Glib::ustring> death_names;
				std::vector<Glib::ustring> previous_dates = fun.read("./nations-store/"+nation+"/datelog.txt");

				if(stat_vector.at((j*3)+1)=="Deaths")
					death_names = fun.get_deaths(previous_dates.back().c_str(), nation);

				for(int i=0; i<previous_dates.size(); i++){
					// This makes loading the graphs much faster and efficent by loading the files only till it finds the line it wants.

					Glib::ustring date = "./nations-store/"+nation+"/"+previous_dates.at(i);
					if((stat_vector.at((j*3)+1)=="Census Data")||(stat_vector.at((j*3)+1)=="Manufacturing"))
						values_vector.push_back(fun.strouble(fun.read_single(date.c_str(), "CENSUSSCORE-"+stat_vector.at((j*3)+2))));

					else if(stat_vector.at((j*3)+1)=="Economy"){
						if(name.find("Tax") != -1)
							values_vector.push_back(fun.strouble(fun.read_single(date.c_str(), "TAX")));
						else if(name.find("Public") != -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "PUBLICSECTOR"), 0, 1)));
						else
							values_vector.push_back(100.0 - fun.strouble(fun.trim(fun.read_single(date.c_str(), "PUBLICSECTOR"), 0, 1)));
					}

					else if(stat_vector.at((j*3)+1)=="Budget"){
						if(name.find("Enviroment")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "ENVIROMENT"), 0, 1)));
						else if(name.find("Social")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "SOCIALEQUALITY"), 0, 1)));
						else if(name.find("Education")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "EDUCATION"), 0, 1)));
						else if(name.find("Law")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "LAWANDORDER"), 0, 1)));
						else if(name.find("Admin")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "ADMINISTRATION"), 0, 1)));
						else if(name.find("Welfare")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "WELFARE"), 0, 1)));
						else if(name.find("Spirit")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "SPIRITUALITY"), 0, 1)));
						else if(name.find("Defence")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "DEFENCE"), 0, 1)));
						else if(name.find("Public")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "PUBLICTRANSPORT"), 0, 1)));
						else if(name.find("Health")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "HEALTHCARE"), 0, 1)));
						else
							values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), "COMMERCE"), 0, 1)));
					}
					else{
						for(int k=0; k<death_names.size()/2; k++){
							if(death_names.at(k*2).find(name)!=-1)
								values_vector.push_back(fun.strouble(fun.trim(fun.read_single(date.c_str(), death_names.at(k*2)), 0, 1)));
						}
					}
				}
			}
		}
	}
	else
		update_button.hide();
}

void gTest::refresh_saves(){
		Nation_View& nations = Nation_View::instance();
		Save_View& saves = Save_View::instance();
		Glib::ustring select_nation = nations.get_selected_nation();

		if(select_nation != ""){
			saves.clear_save_list();
			vector<Glib::ustring> previous_dates = fun.read("./nations-store/"+select_nation+"/datelog.txt");
			for(signed int i=previous_dates.size()-1; i>-1; i--)
				saves.append_save(fun.trim(previous_dates.at(i), 0, 4));
		}
}

void gTest::on_button_update(){
	std::vector<Glib::ustring> nation_list = fun.read("./name-store/nation_list.txt");
	if(nation_list.size()>0){
		for( int i=0; i<nation_list.size(); i++)
			goto_get_all(nation_list.at(i));
	}
}

std::vector<Glib::ustring> gTest::get_stat_vector(){
	return stat_vector;
}

std::vector<double> gTest::get_value_vector(){
	return values_vector;
}

void gTest::goto_data(std::vector<Glib::ustring> nation_data){
	std::vector< std::vector<Glib::ustring> > last_vectors = fun.last_vectors_generate(fun.read("./nations-store/"+nation_data.at(1)+"/"+nation_data.at(0)));
	stats.print_data(data_vectors, last_vectors, 1);
}

void gTest::goto_load(std::vector<Glib::ustring> nation_data){
	nation = nation_data.at(1);
	current_time = nation_data.at(0);
	std::vector<Glib::ustring> all_data = fun.read("./nations-store/"+nation+"/"+current_time);
	std::vector< std::vector<Glib::ustring> > last_vectors = fun.last_vectors_generate(all_data);
	data_vectors.clear();
	data_vectors = fun.vectors_generate(all_data, nation);

	stats.print_data(data_vectors, last_vectors, 0);
	nation_label		.set_markup("<b><big>"+all_data.at(2)+"</big></b>");
	fullname			.set_markup(fun.make_fullname_text(all_data, data_vectors));
	rights				.set_markup(fun.make_rights_text(all_data, data_vectors));
	description_label	.set_label(fun.make_description_text(all_data, data_vectors, nation));
	events_label		.set_markup(fun.make_events_text(data_vectors));

	flag.clear();
	fun.curl_grab("./nations-store/"+nation+"/flag.jpg", all_data.at(25));
	flag.set("./nations-store/"+nation+"/flag.jpg");
}

void gTest::goto_delete_all(Glib::ustring nationer){
	//delete nationer folder, and from nation_log
}

void gTest::goto_get_all(Glib::ustring nationer){
	Glib::ustring currenter_time = fun.get_time(0, false)+"-"+fun.get_time(1, false)+".txt";

	fun.get_nation_data(nationer);
	xmlpp::DomParser parser;
	parser.parse_file("./nation.xml");

	std::vector<Glib::ustring> all_data;
	all_data = fun.print_node(parser.get_document()->get_root_node(), all_data);
	fun.save_data(all_data, currenter_time, nationer);
	force_notebook_refresh(5);
}

void gTest::force_notebook_refresh(int page){
	if(notebook.get_current_page() == page){
		if(page>2)
			notebook.set_current_page(page-1);
		else
			notebook.set_current_page(page+1);
		notebook.set_current_page(page);
	}
}
