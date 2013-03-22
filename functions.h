#ifndef GTKMM_FUNCS
#define GTKMM_FUNCS

#include <gtkmm.h>
#include <libxml++/libxml++.h>

class functions{

public:

const char * strchar (std::string String);
double strouble(std::string String);
std::string doubstr(double number);
int count_lines(std::string file);
Glib::ustring trim(Glib::ustring the_string, int from_start, int from_end);
std::vector<Glib::ustring> read(std::string file);
void curl_grab(Glib::ustring filed, Glib::ustring url);
int get_nation_data(Glib::ustring nation);
Glib::ustring get_time(int mode, bool gmt);
int number_of_sets(Glib::ustring nation);
bool check_for_new_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee);

Glib::ustring make_fullname_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors);
Glib::ustring make_rights_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors);
Glib::ustring make_description_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors, Glib::ustring nation);
Glib::ustring make_events_text(std::vector< std::vector<Glib::ustring> > data_vectors);

std::vector<Glib::ustring> load_data(Glib::ustring current_time, Glib::ustring nation);
void save_data(std::vector<Glib::ustring> all_data, Glib::ustring current_time, Glib::ustring nation);
void set_error(Gtk::Window* errorPopup, Glib::ustring title, Glib::ustring error_message);
Gtk::Window* error_setup();
std::vector<Glib::ustring> print_node(const xmlpp::Node* node, std::vector<Glib::ustring> new_data_first);
std::vector<std::vector<Glib::ustring> > vectors_generate(std::vector<Glib::ustring> all_data, Glib::ustring nation);
std::vector<std::vector<Glib::ustring> > last_vectors_generate(std::vector<Glib::ustring> last_data);

};

#endif
