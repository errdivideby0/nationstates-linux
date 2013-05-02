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

#include "nationstates.h"
#include "preferences.h"
#include "saveview.h"
#include "statview.h"
#include "nationview.h"
#include "statview.h"
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <gtkmm.h>
#include <string>

std::vector<Glib::ustring> Nationstates::stat_vector;
std::vector<double> Nationstates::values_vector;

using namespace std;

Nationstates::Nationstates(): main_box(Gtk::ORIENTATION_HORIZONTAL){

	set_position(Gtk::WIN_POS_CENTER);
	set_events(Gdk::BUTTON_RELEASE_MASK);
	set_default_size(1000, 600);

	add(mainmain);

	mainmain.pack_start(menu_box, Gtk::PACK_SHRINK);
	mainmain.pack_start(main_box);

	action_group = Gtk::ActionGroup::create();

	action_group->add(Gtk::Action::create("File", "File"));

	action_group->add(Gtk::Action::create("AddNation", "Add Nation"), Gtk::AccelKey("<control>N"), sigc::mem_fun(*this, &Nationstates::menu_add_nation));
	action_group->add(Gtk::Action::create("Quit", Gtk::Stock::QUIT), sigc::mem_fun(*this, &Nationstates::menu_quit));

	action_group->add(Gtk::Action::create("Edit", "Edit"));
	action_group->add(Gtk::Action::create("Copy", Gtk::Stock::COPY), sigc::mem_fun(*this, &Nationstates::menu_empty));
	action_group->add(Gtk::Action::create("Paste", Gtk::Stock::PASTE), sigc::mem_fun(*this, &Nationstates::menu_empty));
	action_group->add(Gtk::Action::create("Preferences", "Preferences"), Gtk::AccelKey("<control><alt>P"), sigc::mem_fun(*this, &Nationstates::menu_preferences));

	action_group->add(Gtk::Action::create("View", "View"));
	view_info_box_hide_check = Gtk::ToggleAction::create("Hide Info/Flag Box", "Hide Info/Flag Box");
	action_group->add(view_info_box_hide_check);
	view_info_box_hide_check->signal_toggled().connect(sigc::mem_fun(*this, &Nationstates::view_info_box_hide));

	action_group->add(Gtk::Action::create("Tools", "Tools"));
	action_group->add(Gtk::Action::create("UpdateAll", "Update All"), Gtk::AccelKey("<control>U"), sigc::mem_fun(*this, &Nationstates::menu_update_all));

	action_group->add(Gtk::Action::create("MenuHelp", "Help"));
	action_group->add(Gtk::Action::create("Help", "Help"), Gtk::AccelKey("F1"), sigc::mem_fun(*this, &Nationstates::menu_help));
	action_group->add(Gtk::Action::create("About", "About"), sigc::mem_fun(*this, &Nationstates::menu_about));

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
		"    <menu action='View'>"
		"       <menuitem action='Hide Info/Flag Box' />"
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
				scrolled_stats.add(Stat_View::instance());
				scrolled_stats.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
				scrolled_stats.set_size_request(420, 348);
		left_box.pack_start(search_entry, Gtk::PACK_SHRINK);
	main_box.pack_start(right_box);
		right_box.set_size_request(580, 500);
		right_box.pack_start(v_header, Gtk::PACK_SHRINK);
			v_header.set_size_request(580, 87);
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
						scrolled_nation.set_size_request(160,0);
					save_box.pack_start(scrolled_save);
						scrolled_save.add(Save_View::instance());
						scrolled_save.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
						scrolled_save.set_size_request(420, 0);
				save_box_big.pack_start(events_notebook, Gtk::PACK_SHRINK);
					events_notebook.set_tab_pos(Gtk::POS_BOTTOM);
					events_notebook.append_page(latest_events_box, "Latest Events");
						latest_events_box.set_size_request(580, 120);
						latest_events_box.pack_start(events_preview);
							events_preview.set_wrap_mode(Gtk::WRAP_WORD);
							events_preview.set_border_width(8);
							events_preview.set_editable(false);
					events_notebook.append_page(save_comments, "User Comments");
						save_comments.set_size_request(580, 120);
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

	//// SIGNAL HANDLERS ////
	notebook.signal_switch_page().connect(sigc::mem_fun(*this, &Nationstates::on_page_switch));
	preferences.signal_hide().connect(sigc::mem_fun(*this, &Nationstates::load_preferences));
	search_entry.signal_key_release_event().connect(sigc::mem_fun(*this, &Nationstates::on_search_key));
	Stat_View::instance().signal_key_release_event().connect(sigc::mem_fun(*this, &Nationstates::on_tree_key));

	//// CALLS TO OTHER CLASSES ////
	Stat_View::instance().set_enable_search(false);
	Stat_View::instance().print_blank("");
	Nation_View::instance().refresh_nations();

	show_all_children();
	events_notebook.hide();
	load_preferences();
}

void Nationstates::menu_add_nation(){
	adder.show();
}

void Nationstates::new_nation(Glib::ustring nationer){
	nation = fun.lowercase(nationer);
	if(nation.length() > 0){
		int nation_size = fun.get_nation_data(nation);
		if(nation_size>69){
			update_nation(nation);
			Nation_View::instance().refresh_nations();
		}
	}
}

void Nationstates::on_page_switch(Gtk::Widget*, guint page_num){
	std::vector<Glib::ustring> previous_dates;
	std::vector< std::vector<Glib::ustring> > temp_vectors;

	if((page_num == 3)&&(nation.length()>0)){
		stat_vector.clear();
		values_vector.clear();
		stat_vector = Stat_View::instance().get_selected_stat();

		if(stat_vector.size()>1){
			for(int j=0; j<stat_vector.size()/3; j++){
				previous_dates.clear();
				Glib::ustring name = stat_vector.at(j*3);
				vector<Glib::ustring> previous_dates = fun.read("./nations-store/"+nation+"/datelog.txt");

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
					else{
						bool death_found = false;
						vector<Glib::ustring> det = fun.load_data(nation, previous_dates.at(i), 3);
						for(int k=0; k<det.size()/2; k++){
							if(name.find(det.at(k*2))!=-1){
								values_vector.push_back(fun.strouble(det.at((k*2)+1)));
								death_found = true;
								break;
							}
						}
						if(death_found==false)
							values_vector.push_back(0.0);
					}
				}
			}
		}
	}
}

void Nationstates::update_event_preview(Glib::ustring selected_save){
	try{
		//if(selected_save.length()>0){
			Glib::ustring selected_nation = 		Nation_View::instance().get_selected_nation();
			vector<Glib::ustring> events_times = 	fun.convert_times(fun.load_data(selected_nation, selected_save, 4), "%b %d, %R");
			vector<Glib::ustring> events = 			fun.load_data(selected_nation, selected_save, 5);

			Gtk::TextIter iter_at;
			events_buffer = Gtk::TextBuffer::create();

			bold_tag = events_buffer->create_tag("bold");
			bold_tag->set_property("weight", Pango::WEIGHT_BOLD);

			vector< Glib::RefPtr<Gtk::TextTag> > tags;
			tags.push_back(bold_tag);

			for(int i=0; i<3; i++){
				iter_at = events_buffer->end();
				events_buffer->insert_with_tags(iter_at, "\n"+events_times.at(i), tags);
				iter_at = events_buffer->end();
				events_buffer->insert(iter_at, " - "+events.at(i));
			}
			events_preview.set_buffer(events_buffer);
			//if(events_preview_hide == false) // for the hide events preview
					events_notebook.show();

	}catch(exception e){
		//Save_View::instance().select_default();
	}
}

void Nationstates::menu_update_all(){
	std::vector<Glib::ustring> nation_list = fun.read("./name-store/nation_list.txt");
	if(nation_list.size()>0){
		for( int i=0; i<nation_list.size(); i++)
			update_nation(nation_list.at(i));
	}
}

void Nationstates::load_latest(Glib::ustring nationed){
	load_main(fun.read("./nations-store/"+nationed+"/datelog.txt").back(), nationed, 1);

	vector<Glib::ustring> previous_dates = fun.read(fun.strchar("./nations-store/"+nationed+"/datelog.txt"));
	int lines = previous_dates.size();
	if(lines>1)
		Stat_View::instance().print_data(nationed, previous_dates.back(), nationed, previous_dates.at(lines-2),  search_entry.get_text());
	else
		Stat_View::instance().print_data(nationed, previous_dates.back(), nationed, previous_dates.back(), search_entry.get_text());
}

std::vector<Glib::ustring> Nationstates::get_stat_vector(){
	return stat_vector;
}

std::vector<double> Nationstates::get_value_vector(){
	return values_vector;
}

void Nationstates::load_compare(Glib::ustring selected_save, Glib::ustring selected_nation){;
	if(nation.length()>0)
		Stat_View::instance().print_data(nation, loaded, selected_nation, selected_save, search_entry.get_text());
}

void Nationstates::load_main(Glib::ustring selected_save, Glib::ustring selected_nation, int skip_tree_print){

	nation = selected_nation;
	loaded = selected_save;

	if(skip_tree_print==0){
		Stat_View::instance().print_data(selected_nation, selected_save, selected_nation, selected_save, search_entry.get_text());
	}

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
	//fun.curl_grab("./nations-store/"+nation+"/flag.jpg", basics.at(4));
	flag.set("./nations-store/"+nation+"/flag.jpg");
}

void Nationstates::delete_nation(Glib::ustring nationer){
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

void Nationstates::update_nation(Glib::ustring nationer){
	Glib::ustring currenter_time = fun.get_time(0, false)+"-"+fun.get_time(1, false)+".csv";
	fun.get_nation_data(nationer);
	fun.save_data(currenter_time, nationer);
	Nation_View::instance().refresh_nations();
}

void Nationstates::force_notebook_refresh(int page){
	if(notebook.get_current_page() == page){
		if(page>2)
			notebook.set_current_page(page-1);
		else
			notebook.set_current_page(page+1);
		notebook.set_current_page(page);
	}
}

void Nationstates::set_notebook_page(int page){
	notebook.set_current_page(page);
}

bool Nationstates::on_tree_key(GdkEventKey* event){
	vector<Glib::ustring> st_vect {" ", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "-", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
	for(int i=0; i<st_vect.size(); i++){
		if(event->string==st_vect.at(i)){
			search_entry.set_text(event->string);
			on_search_key(event);
			break;
		}
	}
}

bool Nationstates::on_search_key(GdkEventKey* event){
	if(nation!="")
		Stat_View::instance().print_hide(search_entry.get_text());
	else
		Stat_View::instance().print_blank(search_entry.get_text());
	set_focus(search_entry);
	search_entry.set_position(search_entry.get_text().length());
	return false;
}

void Nationstates::menu_quit(){
	hide();
}

void Nationstates::menu_empty(){
}

void Nationstates::menu_preferences(){
	preferences.show();
}

void Nationstates::menu_help(){
}

void Nationstates::menu_about(){
	about.show();
}

void Nationstates::view_info_box_hide(){
	vector<Glib::ustring> settings = fun.read("./settings.conf");
	if (settings.size() == 0){
		for(int i=0;i<4;i++)
			settings.push_back("");
	}
	if(view_info_box_hide_check->get_active())
		settings.at(3) = "Hide Info/Flag Box: true";
	else
		settings.at(3) = "Hide Info/Flag Box: false";
	ofstream save;
	save.open("./settings.conf");
	for(int i=0;i<settings.size();i++){
		cout<<settings.at(i)<<"\n";
		save<<settings.at(i)<<"\n";
	}
	save.close();
	load_preferences();
}

void Nationstates::load_preferences(){
	vector<Glib::ustring> settings = fun.read("./settings.conf");
	if(settings.size() > 0){
		if(settings.at(3).find("true") != -1){
			view_info_box_hide_check->set_active();
			v_header.hide();
		}
		else
			v_header.show();
	}
}

