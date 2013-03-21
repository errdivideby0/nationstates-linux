#include "gtest.h"
#include <cstdlib>
#include <iostream>
#include <exception>
#include <gtkmm.h>
#include <string>

using namespace std;

gTest::gTest(): main_box(Gtk::ORIENTATION_HORIZONTAL), next_button("Next"){

	set_border_width(0);
	set_default_size(1040, 620);

	scrolled_save.add(saves);
	scrolled_save.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	scrolled_events.add(event_box);
	scrolled_events.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	add(main_box);
	//main_box.pack_start(big_box);

	main_box.pack_start(scrolled_stats);
	main_box.pack_start(right_box);

	scrolled_stats.add(stats);
	scrolled_stats.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	scrolled_stats.set_size_request(430, 570);

	right_box.set_size_request(590, 570);
	right_box.pack_start(v_header, Gtk::PACK_SHRINK);
	right_box.pack_start(notebook, Gtk::PACK_EXPAND_WIDGET);
	right_box.pack_start(input_box, Gtk::PACK_SHRINK);

	v_header.set_size_request(590, 100);
	v_header.pack_start(header_upper_box);
	v_header.pack_start(header_box);

	notebook.append_page(description_box, "Description");
	notebook.append_page(Info3, "Issues");
 	notebook.append_page(Info4, "Region");
	notebook.append_page(Info5, "Compare");
	notebook.append_page(Info6, "Census Graph");
	notebook.append_page(scrolled_events, "Events");
	notebook.append_page(save_box, "Saved Data");

	save_box.pack_start(save_buttons);
	save_box.pack_start(scrolled_save);

	header_upper_box.set_border_width(4);
	header_upper_box.pack_start(nation_label);

	header_box.pack_start(flag);
	header_box.pack_start(nation_box);
	header_box.set_border_width(8);

	flag.set_size_request(115, 100);
	nation_box.set_size_request(475, 100);

	nation_label.set_selectable(true);
	nation_label.set_markup("<b><big>Nation</big></b>");
	nation_label.set_justify(Gtk::JUSTIFY_CENTER);

	fullname.set_selectable(true);
	fullname.set_markup("\"Motto\"\nCategory");
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
	nation_box.set_border_width(8);

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

void gTest::on_button_next(){

	string nationer = nation_input.get_text().lowercase();
	nationer[0] = toupper(nationer[0]);

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

			std::vector<Glib::ustring> all_data;
			all_data = fun.print_node(parser.get_document()->get_root_node(), all_data);

			data_vectors.clear();
			data_vectors = fun.vectors_generate(all_data, nation);

			std::vector< std::vector<Glib::ustring> > last_vectors;
			if(fun.number_of_sets(nation)>1)
				last_vectors = fun.last_vectors_generate(fun.load_data(current_time, nation));

			if(fun.read("./"+nation+"/datelog.txt").size()>1)
				stats.print_data(data_vectors, last_vectors, 1);
			else
				stats.print_data(data_vectors, last_vectors, 0);

			fun.save_data(all_data, current_time, nation);

			fun.curl_grab("./"+nation+"/flag.jpg", all_data.at(25));
			flag.set("./"+nation+"/flag.jpg");

			nation_label		.set_markup("<b><big>"+all_data.at(2)+"</big></b>");
			fullname			.set_markup(fun.make_fullname_text(all_data, data_vectors));
			rights				.set_markup(fun.make_rights_text(all_data, data_vectors));
			description_label	.set_label(fun.make_description_text(all_data, data_vectors, nation));
			events_label		.set_markup(fun.make_events_text(data_vectors));

			set_title(nation+" | roughly "+fun.get_time(2)+" hours until update");

			// This is to force a refresh (the on_notebook_switch_page function) but I do not know a better way of doing so.
			force_notebook_refresh();
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

/* Use this to detect which rows are expanded
bool Gtk::TreeView::row_expanded 	( 	const TreeModel::Path&  	path	)
Returns true if the node pointed to by path is expanded in tree_view.
Parameters
    path	A Gtk::TreePath to test expansion state.
*/

void gTest::on_notebook_switch_page(Gtk::Widget*, guint page_num){
	if(page_num == 6){
		std::vector<Glib::ustring> nation_list = fun.read("./nation_list.txt");
		if(nation_list.size()>0){
			saves.clear_save_list();
			for(int j=0; j<nation_list.size(); j++){
				std::vector<Glib::ustring> previous_dates = fun.read("./"+nation_list.at(j)+"/datelog.txt");
				int n_dates = previous_dates.size();
				saves.append_nation(nation_list.at(j), std::to_string(n_dates));
				for(int i=n_dates-1; i>-1; i--){
					saves.append_row();
					Glib::ustring pdate = fun.trim(previous_dates.at(i), 0, 4);
					if((nation == nation_list.at(j))&&(current_time == previous_dates.at(i))){
						saves.set_row("*"+pdate);
						saves.expand_category();
					}
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

void gTest::goto_load(std::vector<Glib::ustring> nation_data){
	nation = nation_data.at(1);
	current_time = nation_data.at(0);
	std::vector<Glib::ustring> all_data = fun.read("./"+nation+"/"+current_time);
	std::vector< std::vector<Glib::ustring> > last_vectors = fun.last_vectors_generate(all_data);
	data_vectors.clear();
	data_vectors = fun.vectors_generate(all_data, nation);

	stats.print_data(data_vectors, last_vectors, 0);
	nation_label		.set_markup("<b><big>"+all_data.at(2)+"</big></b>");
	fullname			.set_markup(fun.make_fullname_text(all_data, data_vectors));
	rights				.set_markup(fun.make_rights_text(all_data, data_vectors));
	description_label	.set_label(fun.make_description_text(all_data, data_vectors, nation));
	events_label		.set_markup(fun.make_events_text(data_vectors));

	set_title(nation+" | roughly "+fun.get_time(2)+" hours until update");
	force_notebook_refresh();
}

void gTest::goto_delete_all(Glib::ustring nationer){
	//delete nationer folder, and from nation_log
}

void gTest::goto_get_all(Glib::ustring nationer){
	Glib::ustring currenter_time = fun.get_time(0)+"-"+fun.get_time(1)+".txt";

	fun.get_nation_data(nationer);
	xmlpp::DomParser parser;
	parser.parse_file("./nation.xml");

	std::vector<Glib::ustring> all_data;
	all_data = fun.print_node(parser.get_document()->get_root_node(), all_data);
	fun.save_data(all_data, currenter_time, nationer);
	force_notebook_refresh();

}

void gTest::force_notebook_refresh(){
	if(notebook.get_current_page() == 6){
		notebook.set_current_page(5);
		notebook.set_current_page(6);
	}
}
