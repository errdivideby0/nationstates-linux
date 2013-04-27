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

#ifndef GTKMM_FUNCS
#define GTKMM_FUNCS

#include <gtkmm.h>
#include <libxml++/libxml++.h>

class functions{

public:

const char * strchar (std::string String);
std::string lowercase(std::string string);
double strouble(std::string String);
std::string doubstr(double number);
int count_lines(std::string file);
Glib::ustring trim(Glib::ustring the_string, int from_start, int from_end);
std::vector<Glib::ustring> read(std::string file);
std::string read_single(const char * file, Glib::ustring term);
void curl_grab(Glib::ustring filed, Glib::ustring url);
int get_nation_data(Glib::ustring nation);
Glib::ustring get_time(int mode, bool gmt);
bool check_for_new_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee);
std::vector<Glib::ustring> get_deaths(const char * latest_deaths, Glib::ustring nation);

Glib::ustring make_fullname_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors);
Glib::ustring make_rights_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors);
Glib::ustring make_description_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors, Glib::ustring nation);
Glib::ustring make_events_text(std::vector< std::vector<Glib::ustring> > data_vectors);

void save_data(std::vector<Glib::ustring> all_data, Glib::ustring current_time, Glib::ustring nation);
void set_error(Gtk::Window* errorPopup, Glib::ustring title, Glib::ustring error_message);
Gtk::Window* error_setup();
std::vector<Glib::ustring> print_node(const xmlpp::Node* node, std::vector<Glib::ustring> new_data_first);
std::vector<std::vector<Glib::ustring> > vectors_generate(std::vector<Glib::ustring> all_data, Glib::ustring nation);
std::vector<std::vector<Glib::ustring> > last_vectors_generate(std::vector<Glib::ustring> last_data);

};

#endif
