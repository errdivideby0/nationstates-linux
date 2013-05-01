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
#include "preferences.h"
#include "saveview.h"
#include "treeview.h"
#include "nationview.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <gtkmm.h>
#include <string>

std::vector<Glib::ustring> gTest::stat_vector;
std::vector<double> gTest::values_vector;

using namespace std;

gTest::gTest(): main_box(Gtk::ORIENTATION_HORIZONTAL){

	set_position(Gtk::WIN_POS_CENTER);
	set_events(Gdk::BUTTON_RELEASE_MASK);
	set_default_size(1000, 600);

	add(mainmain);

	mainmain.pack_start(menu_box, Gtk::PACK_SHRINK);
	mainmain.pack_start(main_box);

	action_group = Gtk::ActionGroup::create();

	action_group->add(Gtk::Action::create("File", "File"));

	action_group->add(Gtk::Action::create("AddNation", "Add Nation"), Gtk::AccelKey("<control>N"), sigc::mem_fun(*this, &gTest::on_add_nation));
	action_group->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &gTest::on_menu_file_quit));

	action_group->add(Gtk::Action::create("Edit", "Edit"));
	action_group->add(Gtk::Action::create("Copy", Gtk::Stock::COPY), sigc::mem_fun(*this, &gTest::on_menu_others));
	action_group->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE), sigc::mem_fun(*this, &gTest::on_menu_others));
	action_group->add(Gtk::Action::create("Preferences", "Preferences"), Gtk::AccelKey("<control><alt>P"), sigc::mem_fun(*this, &gTest::on_menu_pref));

	action_group->add(Gtk::Action::create("Tools", "Tools"));
	action_group->add(Gtk::Action::create("UpdateAll", "Update All"), Gtk::AccelKey("<control>U"), sigc::mem_fun(*this, &gTest::fetch_all));

	action_group->add(Gtk::Action::create("MenuHelp", "Help"));
	action_group->add(Gtk::Action::create("Help", "Help"), Gtk::AccelKey("F1"), sigc::mem_fun(*this, &gTest::on_menu_help));
	action_group->add(Gtk::Action::create("About", "About"), sigc::mem_fun(*this, &gTest::on_menu_about));


	ui_manager = Gtk::UIManager::create();
	ui_manager->insert_action_group(action_group);
	add_accel_group(ui_manager->get_accel_group());

	Glib::ustring ui_info =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='File'>"
        "      <menuitem action='AddNation'/>"
        "      <separator/>"
        "      <menuitem action='Quit'/>"
        "    </menu>"
        "    <menu action='Edit'>"
        "      <menuitem action='Copy'/>"
        "      <menuitem action='Paste'/>"
        "      <separator/>"
        "      <menuitem action='Preferences'/>"
        "    </menu>"
        "    <menu action='Tools'>"
        "      <menuitem action='UpdateAll'/>"
        "    </menu>"
        "    <menu action='MenuHelp'>"
        "      <menuitem action='Help'/>"
        "      <menuitem action='About'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";

	try{
		ui_manager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex){
		std::cerr << "building menus failed: \n" <<  ex.what();
	}

	Gtk::Widget* pMenubar = ui_manager->get_widget("/MenuBar");
	if(pMenubar)
		menu_box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

	main_box.pack_start(left_box);
		left_box.pack_start(scrolled_stats);
				scrolled_stats.add(Tree_View::instance());
				scrolled_stats.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
				scrolled_stats.set_size_request(400, 348);
		left_box.pack_start(search_entry, Gtk::PACK_SHRINK);
	main_box.pack_start(right_box);
		right_box.set_size_request(600, 500);
		right_box.pack_start(v_header, Gtk::PACK_SHRINK);
			v_header.set_size_request(600, 87);
			v_header.pack_start(header_upper_box);
				header_upper_box.set_border_width(2);
				header_upper_box.pack_start(nation_label);
					nation_label.set_selectable(true);
					nation_label.set_markup("<b><big>Nation</big></b>");
					nation_label.set_justify(Gtk::JUSTIFY_CENTER);
			v_header.pack_start(header_box);
				header_box.pack_start(flag_box, Gtk::PACK_SHRINK);
					flag_box.set_border_width(6);
					flag_box.pack_start(flag, Gtk::PACK_SHRINK);
					flag_box.set_size_request(130, 72);
				header_box.pack_start(nation_box, Gtk::PACK_EXPAND_WIDGET);
					nation_box.pack_start(fullname, Gtk::PACK_SHRINK);
						fullname.set_selectable(true);
						fullname.set_markup("\"Motto\"\nCategory:\nPopulation:");
						fullname.set_justify(Gtk::JUSTIFY_LEFT);
						fullname.set_line_wrap();
					nation_box.pack_start(rights, Gtk::PACK_EXPAND_WIDGET);
						rights.set_selectable(true);
						rights.set_markup("Civil Rights:   ( )\nEconomy:   ( )\nPolitical Freedom:   ( )\nRegional Influence:   ( )");
						rights.set_justify(Gtk::JUSTIFY_RIGHT);
		right_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET);
			notebook.append_page(save_box_big, "Saved Data");
				save_box_big.pack_start(save_box);
					save_box.pack_start(scrolled_nation);
						scrolled_nation.add(Nation_View::instance());
						scrolled_nation.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
						scrolled_nation.set_size_request(200,0);
					save_box.pack_start(scrolled_save);
						scrolled_save.add(Save_View::instance());
						scrolled_save.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
						scrolled_save.set_size_request(400, 0);
				save_box_big.pack_start(latest_events_box, Gtk::PACK_SHRINK);
					latest_events_box.set_size_request(600, 120);
					latest_events_box.set_border_width(8);
					latest_events_box.pack_start(latest_events_label);
						latest_events_label.set_justify(Gtk::JUSTIFY_LEFT);
						latest_events_label.set_line_wrap();
			notebook.append_page(description_box, "Description");
				description_box.set_valign(Gtk::ALIGN_START);
				description_box.set_halign(Gtk::ALIGN_START);
				description_box.set_border_width(8);
				description_box.pack_start(description_label, Gtk::PACK_SHRINK);
					description_label.set_line_wrap();
			notebook.append_page(region_box, "Region");
			notebook.append_page(graph_box, "Census Graph");
				graph_box.pack_start(plotter);
			notebook.append_page(scrolled_events, "Events");
				scrolled_events.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
				scrolled_events.add(event_box);
					event_box.set_valign(Gtk::ALIGN_START);
					event_box.set_halign(Gtk::ALIGN_START);
					event_box.pack_start(events_label);
					event_box.set_border_width(8);
					events_label.set_line_wrap();

	set_title("NationStates");

	notebook.signal_switch_page().connect(sigc::mem_fun(*this, &gTest::on_page_switch));
	preferences.signal_hide().connect(sigc::mem_fun(*this, &gTest::load_preferences));
	search_entry.signal_key_release_event().connect(sigc::mem_fun(*this, &gTest::on_search_key));


	Tree_View::instance().print_blank();
	Nation_View::instance().refresh_nations();

	show_all_children();
	load_preferences();
}

void gTest::on_add_nation(){
	adder.show();
}

void gTest::new_nation(Glib::ustring nationer){
	nation = fun.lowercase(nationer);
	if(nation.length() > 0){
		int nation_size = fun.get_nation_data(nation);
		if(nation_size>69){
			fetch(nation);
			Nation_View::instance().refresh_nations();
		}
	}
}

void gTest::on_page_switch(Gtk::Widget*, guint page_num){
	std::vector<Glib::ustring> previous_dates;
	std::vector< std::vector<Glib::ustring> > temp_vectors;

	if((page_num == 3)&&(nation.length()>0)){
		stat_vector.clear();
		values_vector.clear();
		stat_vector = Tree_View::instance().get_selected_stat();

		if(stat_vector.size()>1){
			for(int j=0; j<stat_vector.size()/3; j++){
				previous_dates.clear();
				Glib::ustring name = stat_vector.at(j*3);
				vector<Glib::ustring> death_names;
				vector<Glib::ustring> previous_dates = fun.read("./nations-store/"+nation+"/datelog.txt");

				/*if(stat_vector.at((j*3)+1)=="Deaths"){
					vector<Glib::ustring> death_temp = fun.load_data(nation, previous_dates.back(), 3);
					for(int k=0; k<death_temp.size()/2; k++)
						death_names.push_back(death_temp.at(k*2));
				}*/

				for(int i=0; i<previous_dates.size(); i++){

					// This makes loading the graphs much faster and efficent by loading the files only till it finds the line it wants.
					if(stat_vector.at((j*3)+1)=="Census Data"){
						vector<Glib::ustring> cen = fun.load_data(nation, previous_dates.at(i), 1);
						values_vector.push_back(fun.strouble(cen.at(std::stoi(stat_vector.at((j*3)+2)))));
					}
					else if(stat_vector.at((j*3)+1)=="Manufacturing"){
						vector<Glib::ustring> man = fun.load_data(nation, previous_dates.at(i), 2);
						values_vector.push_back(fun.strouble(man.at(std::stoi(stat_vector.at((j*3)+2)))));
					}

					else if(stat_vector.at((j*3)+1)=="Economy"){
						vector<Glib::ustring> eco = fun.load_data(nation, previous_dates.at(i), 8);
						if(name.find("Tax") != -1)
							values_vector.push_back(fun.strouble(eco.at(0)));
						else if(name.find("Public") != -1)
							values_vector.push_back(fun.strouble(eco.at(1)));
						else
							values_vector.push_back(fun.strouble(eco.at(2)));
					}

					else if(stat_vector.at((j*3)+1)=="Budget"){
						vector<Glib::ustring> bud = fun.load_data(nation, previous_dates.at(i), 7);
						if(name.find("Enviroment")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(0), 0, 1)));
						else if(name.find("Social")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(1), 0, 1)));
						else if(name.find("Education")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(2), 0, 1)));
						else if(name.find("Law")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(3), 0, 1)));
						else if(name.find("Admin")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(4), 0, 1)));
						else if(name.find("Welfare")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(5), 0, 1)));
						else if(name.find("Spirit")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(6), 0, 1)));
						else if(name.find("Defence")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(7), 0, 1)));
						else if(name.find("Public")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(8), 0, 1)));
						else if(name.find("Health")!= -1)
							values_vector.push_back(fun.strouble(fun.trim(bud.at(9), 0, 1)));
						else
							values_vector.push_back(fun.strouble(fun.trim(bud.at(10), 0, 1)));
					}
					/*else{
						vector<Glib::ustring> det = fun.load_data(nation, previous_dates.at(i), 3);
						for(int k=0; k<death_names.size(); k++){
							if(death_names.at(k).find(name)!=-1)
								values_vector.push_back(fun.strouble(det.at((k*2)+1)));
						}
					}*/
				}
			}
		}
	}
}

void gTest::update_event_preview(Glib::ustring selected_save){
	try{
		if(selected_save.length()>0){
			Glib::ustring selected_nation = Nation_View::instance().get_selected_nation();
			vector<Glib::ustring> events_times = 	fun.convert_times(fun.load_data(selected_nation, selected_save, 4), "%b %d, %R");
			vector<Glib::ustring> events = 				fun.load_data(selected_nation, selected_save, 5);

			Glib::ustring events_text = "•<b>"+events_times.at(0)+"</b> - "+events.at(0);
			for(int i=1; i<3; i++)
				events_text = events_text+"\n•<b>"+events_times.at(i)+"</b> - "+events.at(i);
			latest_events_label.set_markup(events_text);
		}
	}catch(exception e){
		//Save_View::instance().select_default();
	}
}

void gTest::fetch_all(){
	std::vector<Glib::ustring> nation_list = fun.read("./name-store/nation_list.txt");
	if(nation_list.size()>0){
		for( int i=0; i<nation_list.size(); i++)
			fetch(nation_list.at(i));
	}
}

void gTest::compare_latest(Glib::ustring nationed){
	load_main(fun.read("./nations-store/"+nationed+"/datelog.txt").back(), nationed, 1);

	vector<Glib::ustring> previous_dates = fun.read(fun.strchar("./nations-store/"+nationed+"/datelog.txt"));
	int lines = previous_dates.size();
	if(lines>1)
		Tree_View::instance().print_data(nationed, previous_dates.back(), nationed, previous_dates.at(lines-2),  "");
	else
		Tree_View::instance().print_data(nationed, previous_dates.back(), nationed, previous_dates.back(), "");
}

std::vector<Glib::ustring> gTest::get_stat_vector(){
	return stat_vector;
}

std::vector<double> gTest::get_value_vector(){
	return values_vector;
}

void gTest::compare_to_loaded(Glib::ustring selected_save, Glib::ustring selected_nation){;
	if(nation.length()>0)
		Tree_View::instance().print_data(nation, loaded, selected_nation, selected_save, "");
}

void gTest::load_main(Glib::ustring selected_save, Glib::ustring selected_nation, int skip_tree_print){

	nation = selected_nation;
	loaded = selected_save;

	if(skip_tree_print==0)
		Tree_View::instance().print_data(selected_nation, selected_save, selected_nation, selected_save, "");

	vector<Glib::ustring> basics = 			fun.load_data(nation, loaded, 0);
	vector<Glib::ustring> census = 			fun.load_data(nation, loaded, 1);
	vector<Glib::ustring> des = 			fun.load_data(nation, loaded, 6);
	vector<Glib::ustring> events_times = 	fun.load_data(nation, loaded, 4);
	vector<Glib::ustring> events = 			fun.load_data(nation, loaded, 5);
	vector<Glib::ustring> rights_v = 		fun.load_data(nation, loaded, 9);


	Glib::ustring events_text;
	for(int i=0; i<events.size(); i++)
		events_text = events_text+"<b>"+events_times.at(i)+"</b>\n"+events.at(i)+"\n\n";

	Glib::ustring rights_text = "Civil Rights: "+rights_v.at(0)+" ("+census.at(0)+")\nEconomy: "+rights_v.at(1)+" ("+census.at(1)+")\nPolitical Freedom: "+
									rights_v.at(2)+" ("+census.at(2)+")\nRegional Influence: "+basics.at(5)+" ("+census.at(40)+")";

	Glib::ustring description = nation+" is a "+des.at(0)+" nation, renowned for its "+des.at(1)+", and "+des.at(2)+" citizens.\n\n"+des.at(4)+" "+
								des.at(5)+"\n\n"+nation+", "+des.at(7)+ ", "+des.at(8)+", and "+des.at(9)+". "+des.at(10)+" "+nation+
								"\'s national animal is the "+des.at(11)+", which "+des.at(12)+", and its currency is the "+des.at(13)+".";

	nation_label		.set_markup("<b><big>"+basics.at(0)+"</big></b>");
	fullname			.set_markup("\""+basics.at(1)+"\"\n"+basics.at(2)+"\n"+basics.at(3));
	rights				.set_markup(rights_text);
	description_label	.set_label(description);
	events_label		.set_markup(events_text);

	flag.clear();
	fun.curl_grab("./nations-store/"+nation+"/flag.jpg", basics.at(4));
	flag.set("./nations-store/"+nation+"/flag.jpg");
}

void gTest::delete_nation(Glib::ustring nationer){
	vector<Glib::ustring> delete_vector = fun.read("./name-store/nation_list.txt");
	vector<Glib::ustring> deleter = fun.read("./nations-store/"+nationer+"/datelog.txt");
	ofstream savenation;
	savenation.open(fun.strchar("./name-store/nation_list.txt"));
	for(int i=0; i<delete_vector.size(); i++){
		if(delete_vector.at(i)==nationer){
		}
		else
			savenation<<delete_vector.at(i)<<"\n";
	}
	savenation.close();
	for(int i=0; i<deleter.size(); i++)
		std::remove(fun.strchar("./nations-store/"+nationer+"/"+deleter.at(i)));

	std::remove(fun.strchar("./nations-store/"+nationer+"/datelog.txt"));
	std::remove(fun.strchar("./nations-store/"+nationer+"/flag.jpg"));
	rmdir(fun.strchar("./nations-store/"+nationer));
	Nation_View::instance().refresh_nations();
}

void gTest::fetch(Glib::ustring nationer){
	Glib::ustring currenter_time = fun.get_time(0, false)+"-"+fun.get_time(1, false)+".csv";
	fun.get_nation_data(nationer);
	fun.save_data(currenter_time, nationer);
	Nation_View::instance().refresh_nations();
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

void gTest::set_notebook_page(int page){
	notebook.set_current_page(page);
}

bool gTest::on_search_key(GdkEventKey* event){
	Glib::ustring search_text = search_entry.get_text();
	Tree_View::instance().print_hide(search_text);
	return false;
}

void gTest::on_menu_file_quit(){
	hide();
}

void gTest::on_menu_others(){
}

void gTest::on_menu_pref(){
	preferences.show();
}

void gTest::on_menu_help(){
}

void gTest::on_menu_about(){
	about.show();
}

void gTest::load_preferences(){
	vector<Glib::ustring> pref_settings = fun.read("./settings.conf");
	if (pref_settings.size() > 0){
		if (pref_settings.at(0).find("true") != -1)
			v_header.hide();
		else
			v_header.show();
	}
}

