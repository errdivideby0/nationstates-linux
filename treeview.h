#ifndef GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_3
#define GTKMM_EXAMPLE_TREEVIEW_WITHPOPUP_3

#include <gtkmm.h>
#include "functions.h"

class Tree_View : public Gtk::TreeView{

public:
	Tree_View();
	virtual ~Tree_View();

	Glib::ustring get_name_at(int place);
	void print_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee, int print_mode);
	void clear_stat_list();

protected:

	void append_category_row(Glib::ustring category_name);
	void append_stat_row();
	void set_stat_row(Glib::ustring text, Glib::ustring text2, Glib::ustring text3, Glib::ustring text4);
	void expand_stat_list();

	class ModelColumns : public Gtk::TreeModel::ColumnRecord{
		public:

		ModelColumns(){
			add(stat_name); add(stat_value); add(stat_value2); add(stat_update);
		}

		Gtk::TreeModelColumn<Glib::ustring> stat_name;
		Gtk::TreeModelColumn<Glib::ustring> stat_value;
		Gtk::TreeModelColumn<Glib::ustring> stat_value2;
		Gtk::TreeModelColumn<Glib::ustring> stat_update;
	};
	ModelColumns stat_columns;

	Gtk::TreeView TreeView;
	Glib::RefPtr<Gtk::TreeStore> TreeModel;
	Gtk::TreeModel::Row category_row, stat_row;
	static std::vector<Glib::ustring> names;
	functions fun;

};

#endif
