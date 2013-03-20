#ifndef GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H
#define GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_H

#include <gtkmm.h>
#include "functions.h"

class Save_View : public Gtk::TreeView{

public:
	Save_View();
	virtual ~Save_View();

	void append_nation(Glib::ustring nation_pass, Glib::ustring n_dates);
	void clear_save_list();
	void append_row();
	void set_row(Glib::ustring text);
	int number_selected();
	void save_menu_get(std::vector< std::vector<Glib::ustring> > data_vectors);
	void send_instance();

protected:

	virtual bool on_button_press_event(GdkEventButton *ev);

	void save_menu_load();
	void save_menu_delete();
	void save_menu_nothing();
	void save_menu_print();

	class SaveColumns : public Gtk::TreeModel::ColumnRecord{
		public:

	 	SaveColumns(){
			add(stat_date);
		}

	 	 Gtk::TreeModelColumn<Glib::ustring> stat_date;
	};
	SaveColumns save_columns;

	std::vector< std::vector<Glib::ustring> > data_vectorers;
	static std::vector<Glib::ustring> nation_data;

	Glib::RefPtr<Gtk::TreeStore> SaveModel;
	Glib::RefPtr<Gtk::TreeSelection> selection;
	Gtk::TreeModel::Row nation_row, save_row, selected_row, parent_row;
	Gtk::TreeModel::iterator parentrow, iter;
	Gtk::Menu save_menu;
	functions fun;
};


#endif
