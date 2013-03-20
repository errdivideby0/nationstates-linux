#include "saveview.h"

using namespace std;

std::vector<Glib::ustring> Save_View::nation_data;

Save_View::Save_View(){

	SaveModel = Gtk::TreeStore::create(save_columns);
	set_model(SaveModel);
	append_column("Date", save_columns.stat_date);

	Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem("Get Latest Data", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_nothing));
	save_menu.append(*item);
	item = Gtk::manage(new Gtk::MenuItem("Compare", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_print));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Load", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_load));
	save_menu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Delete", true));
	item->signal_activate().connect(sigc::mem_fun(*this, &Save_View::save_menu_delete));
	save_menu.append(*item);

	save_menu.accelerate(*this);
	save_menu.show_all();

	signal_button_press_event().connect(sigc::mem_fun(*this, &Save_View::on_button_press_event));

}

Save_View::~Save_View(){
}

bool Save_View::on_button_press_event(GdkEventButton* event){

	bool return_value = false;
	return_value = TreeView::on_button_press_event(event);

	nation_data.clear();
	selection = get_selection();
	iter = selection->get_selected();
	selected_row = *iter;

	if((event->type == GDK_BUTTON_PRESS) && (event->button == 3) && (selected_row->parent())){
		Glib::ustring ndate = selected_row[save_columns.stat_date];
		nation_data.push_back(ndate + ".txt");
		parentrow = selected_row->parent();
		parent_row = *parentrow;
		Glib::ustring nname = parent_row[save_columns.stat_date];
		nation_data.push_back(nname.substr(0, nname.find(' ')));
		save_menu.popup(event->button, event->time);
	}
	return return_value;
}

void Save_View::save_menu_load(){

}

void Save_View::save_menu_nothing(){
}

void Save_View::save_menu_delete(){
}

void Save_View::save_menu_get(std::vector< std::vector<Glib::ustring> > data_vectors){
	data_vectorers.clear();
	data_vectorers = data_vectors;
}

void Save_View::clear_save_list(){
	SaveModel->clear();
}

void Save_View::append_nation(Glib::ustring nation_pass, Glib::ustring n_dates){
	nation_row = *(SaveModel->append());
	nation_row[save_columns.stat_date] = nation_pass+" ("+n_dates+")";
}

void Save_View::append_row(){
	save_row = *(SaveModel->append(nation_row.children()));
}

void Save_View::set_row(Glib::ustring text){
	save_row[save_columns.stat_date] = text;
}

int Save_View::number_selected(){
	return get_selection()->count_selected_rows();
}

void Save_View::send_instance(){
}

void Save_View::save_menu_print(){
	//if(nation_data.at(0).at(0) == '*')
	//	nation_data.at(0) = trim(nation_data.at(0), 1, 0);


	// Can not figure out how to get the gTest instance that contains the Save_View instance.
	//gtest.goto_data(nation_data);
}
