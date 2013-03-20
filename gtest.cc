#include "gtest.h"
#include <cstdlib>
#include <exception>
#include <gtkmm.h>

using namespace std;

gTest::gTest(): main_box(Gtk::ORIENTATION_VERTICAL), big_box(Gtk::ORIENTATION_HORIZONTAL), next_button("Next"){

	set_border_width(0);
	set_default_size(1060, 620);

	scrolled_save.add(saves);
	scrolled_save.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	scrolled_events.add(event_box);
	scrolled_events.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	add(main_box);
	main_box.pack_start(big_box);
	main_box.pack_start(input_box, Gtk::PACK_SHRINK);

	big_box.pack_start(scrolled_stats);
	big_box.pack_start(right_box);

	scrolled_stats.add(stats);
	scrolled_stats.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_stats.set_size_request(460, 570);

	right_box.set_size_request(600, 570);
	right_box.pack_start(v_header, Gtk::PACK_SHRINK);
	right_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET);

	v_header.set_size_request(600, 120);
	v_header.pack_start(header_box);
	v_header.pack_start(header_lower_box);

	notebook.append_page(description_box, "Description");
	notebook.append_page(Info3, "Issues");
 	notebook.append_page(Info4, "Region");
	notebook.append_page(Info5, "Compare");
	notebook.append_page(Info6, "Census Graph");
	notebook.append_page(scrolled_events, "Events");
	notebook.append_page(save_box, "Saved Data");

	save_box.pack_start(save_buttons);
	save_box.pack_start(scrolled_save);

	header_lower_box.set_border_width(8);

	header_box.pack_start(flag);
	header_box.pack_start(nation_box);
	header_box.set_border_width(8);

	flag.set_size_request(120, 120);
	nation_box.set_size_request(480, 120);

	fullname.set_selectable(true);
	fullname.set_markup("<b><big>Nation</big></b>\n\"Motto\"\nCategory");
	fullname.set_justify(Gtk::JUSTIFY_LEFT);

	rights.set_selectable(true);
	rights.set_markup("Civil Rights:   ( )\nEconomy:   ( )\nPolitical Freedom:    ( )");
	rights.set_justify(Gtk::JUSTIFY_RIGHT);

	description_box.pack_start(description_label, Gtk::PACK_SHRINK);
	description_box.set_valign(Gtk::ALIGN_START);
	description_box.set_halign(Gtk::ALIGN_START);
	description_box.set_border_width(10);
	description_label.set_line_wrap();

	event_box.set_valign(Gtk::ALIGN_START);
	event_box.set_halign(Gtk::ALIGN_START);
	event_box.pack_start(events_label);
	event_box.set_border_width(10);
	events_label.set_line_wrap();

	nation_box.add(fullname);
	nation_box.add(rights);

	set_title("NationStates");
	nation_input.set_placeholder_text("Enter Nation Name");

	input_box.pack_start(nation_input);
	input_box.pack_start(next_button, Gtk::PACK_SHRINK);
	input_box.set_border_width(5);
	input_box.set_layout(Gtk::BUTTONBOX_END);

	next_button.signal_clicked().connect(sigc::mem_fun(*this, &gTest::on_button_next));
	notebook.signal_switch_page().connect(sigc::mem_fun(*this, &gTest::on_notebook_switch_page));

	show_all_children();
}

gTest::~gTest(){
}

void gTest::on_button_next(){

	string nationer = nation_input.get_text().lowercase();
	nationer[0] = toupper(nationer[0]);

	nation.clear();
	nation = nationer;

	errorPopup = fun.error_setup();
	errorPopup->set_transient_for(*this);

	if(nation.length() > 0){

		int nation_size = fun.get_nation_data(nation);

		if(nation_size>69){

			current_time.clear();
			current_time = fun.get_time(0)+"-"+fun.get_time(1)+".txt";

			xmlpp::DomParser parser;
			parser.parse_file("./nation.xml");

			std::vector<Glib::ustring> all_data = fun.print_node(parser.get_document()->get_root_node(), all_data);

			data_vectors.clear();
			data_vectors = fun.vectors_generate(all_data, nation);

			std::vector< std::vector<Glib::ustring> > last_vectors;
			if(fun.number_of_sets(nation)>1)
				last_vectors = fun.last_vectors_generate(fun.load_data(current_time, nation));

			stats.print_data(data_vectors, last_vectors, 0);

			fun.save_data(all_data, current_time, nation);

			fun.curl_grab("./"+nation+"/flag.jpg", all_data.at(25));
			flag.set("./"+nation+"/flag.jpg");

			fullname.set_markup("<b><big>"+all_data.at(2)+"</big></b>"+"\n\""+all_data.at(4)+"\"\n"+all_data.at(6)+"\n\n"+data_vectors.at(0).at(3)+" million citizens");

			rights.set_markup("Civil Rights: "+data_vectors.at(6).at(0)+" ("+data_vectors.at(0).at(0)+")\nEconomy: "+data_vectors.at(6).at(1)+" ("+data_vectors.at(0).at(1)+")\nPolitical Freedom: "+data_vectors.at(6).at(2)+" ("+data_vectors.at(0).at(2)+")\n\nInfluence = "+all_data.at(66));

			Glib::ustring events_text;
			for(int i=0; i<data_vectors.at(2).size(); i++){
				events_text = events_text+"<b>"+data_vectors.at(2).at(i)+"</b>\n"+data_vectors.at(2).at(i+1)+"\n\n"; i++;}
			events_label.set_markup(events_text);

			string temp_string = data_vectors.at(3).at(6);
			temp_string.at(0) = toupper(temp_string.at(0));
			Glib::ustring description = all_data.at(2)+" is a "+data_vectors.at(3).at(0)+" nation, renowned for its "+data_vectors.at(3).at(1)+", and "+data_vectors.at(3).at(2)+" citizens.\n\n"+data_vectors.at(3).at(4)+" "+data_vectors.at(3).at(5)+"\n\n"+temp_string+", "+data_vectors.at(3).at(7)+", "+data_vectors.at(3).at(8)+", and "+data_vectors.at(3).at(9)+". "+data_vectors.at(3).at(10)+" "+nation+"\'s national animal is the "+data_vectors.at(3).at(11)+", which "+data_vectors.at(3).at(12)+", and its currency is the "+data_vectors.at(3).at(13)+".";
			description_label.set_label(description);

			set_title(nation+" | "+fun.get_time(2)+" hours until update");

			if(notebook.get_current_page() == 6){
				notebook.set_current_page(5);
				notebook.set_current_page(6);
			}
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
	if(page_num == 6){
		std::vector<Glib::ustring> nation_list = fun.read("./nation_list.txt");
		if(nation_list.size()>0){
			saves.clear_save_list();
			for(int j=0; j<nation_list.size(); j++){
				std::vector<Glib::ustring> previous_dates = fun.read("./"+nation_list.at(j)+"/datelog.txt");
				int n_dates = previous_dates.size();
				saves.append_nation(nation_list.at(j), fun.intstr(n_dates));
				for(int i=n_dates-1; i>-1; i--){
					saves.append_row();
					Glib::ustring pdate = fun.trim(previous_dates.at(i), 0, 4);
					if((nation == nation_list.at(j))&&(current_time == previous_dates.at(i)))
						saves.set_row("*"+pdate);
					else
						saves.set_row(pdate);
				}
			}
		}
	}
}

void gTest::goto_data(std::vector<Glib::ustring> nation_data){
	std::vector< std::vector<Glib::ustring> > last_vectors = fun.last_vectors_generate(fun.read("./"+nation_data.at(1)+"/"+nation_data.at(0)));
	stats.print_data(data_vectors, last_vectors, 1);
}


void Save_View::save_menu_print(){
	//if(nation_data.at(0).at(0) == '*')
	//	nation_data.at(0) = trim(nation_data.at(0), 1, 0);


	// Can not figure out how to get the gTest instance that contains the Save_View instance.
	//gtest.goto_data(nation_data);
}

