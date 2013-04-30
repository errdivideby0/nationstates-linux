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

	const char * strchar (Glib::ustring String);
	std::string lowercase(std::string string);
	double strouble(Glib::ustring String);
	Glib::ustring doubstr(double number);
	int count_lines(Glib::ustring file);
	Glib::ustring trim(Glib::ustring the_string, int from_start, int from_end);
	std::vector<Glib::ustring> read(Glib::ustring file);
	Glib::ustring read_single_line(const char * file, int line);
	void curl_grab(Glib::ustring filed, Glib::ustring url);
	int get_nation_data(Glib::ustring nation);
	Glib::ustring get_time(int mode, bool gmt);
	bool check_for_new_data(std::vector<Glib::ustring> comparor, std::vector<Glib::ustring> comparee);
	std::vector<Glib::ustring> load_data(Glib::ustring nation, Glib::ustring save, int bas_cen_man_de_evt_ev_de_bu_ec_fr);
	std::vector<Glib::ustring> convert_times(std::vector<Glib::ustring> times, Glib::ustring format);
	void save_data(Glib::ustring current_time, Glib::ustring nation);
	std::vector<Glib::ustring> print_node(const xmlpp::Node* node, std::vector<Glib::ustring> new_data_first);

private:
	std::vector< std::vector<Glib::ustring> > convert_data(std::vector<Glib::ustring> all_data, Glib::ustring nation);

};

#endif
