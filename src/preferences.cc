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

#include "preferences.h"
#include <cstdlib>
#include <gtkmm.h>
#include <iostream>
#include <fstream>

using namespace std;

Preferences_Window::Preferences_Window(): main_box(Gtk::ORIENTATION_VERTICAL), apply_button("Apply"), close_button("Close"){
	set_border_width(5);
	set_default_size(400, 400);
	set_title("Preferences");
	add(main_box);
	main_box.pack_start(pref_notebook);
	pref_notebook.append_page(general_tab,"General");
	pref_notebook.append_page(test_two,"????");
	general_tab.pack_start(general_tab_vbox_one);
	general_tab.pack_start(general_tab_vbox_two);
	general_tab_vbox_one.pack_start(info_box_check);
	info_box_check.set_label("Hide Information/Flag box");
	general_tab_vbox_one.pack_start(baked_potato);
	baked_potato.set_label("baked potato");
	general_tab_vbox_one.pack_start(not_a_potato);
	not_a_potato.set_label("not a potato");
	signal_show().connect(sigc::mem_fun(*this, &Preferences_Window::set_pref));
	info_box_check.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_info_box_check_clicked));
	baked_potato.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_baked_potato_clicked));
	not_a_potato.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_not_a_potato_clicked));
	main_box.pack_start(bbox_one, Gtk::PACK_SHRINK);
	bbox_one.set_border_width(5);
	bbox_one.set_layout(Gtk::BUTTONBOX_END);
	bbox_one.pack_start(apply_button, Gtk::PACK_SHRINK);
	bbox_one.pack_start(close_button, Gtk::PACK_SHRINK);
	close_button.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_close_button));
	apply_button.signal_clicked().connect(sigc::mem_fun(*this, &Preferences_Window::on_apply_button));
	show_all_children();
}

void Preferences_Window::set_pref(){
	pref_settings = fun.read("./settings.conf");
	if (pref_settings.size() == 0){
		for(int i=0;i<3;i++)
			pref_settings.push_back("");
	}
	cout<<pref_settings.size()<<"\n";
	for(int i=0;i<pref_settings.size();i++)
		cout<<pref_settings.at(i)<<"\n";
	if (pref_settings.at(0) == "Hide Info/Flag box: false")
		info_box_check.set_active(true);
	if (pref_settings.at(1) == "true")
		baked_potato.set_active(true);
	if (pref_settings.at(2) == "true")
		not_a_potato.set_active(true);
}

void Preferences_Window::on_info_box_check_clicked(){
	if (info_box_check.get_active()){
		pref_settings.at(0) = "Info/Flag box: true";
	}
	else{
		pref_settings.at(0) = "Info/Flag box: false";
	}
}

void Preferences_Window::on_baked_potato_clicked(){
	if (baked_potato.get_active()){
		pref_settings.at(1) = "true";
	}
	else{
		pref_settings.at(1) = "false";
	}
}

void Preferences_Window::on_not_a_potato_clicked(){
	if (not_a_potato.get_active()){
		pref_settings.at(2) = "true";
	}
	else{
		pref_settings.at(2) = "false";
	}
}

void Preferences_Window::on_apply_button(){
	ofstream save;
	save.open("./settings.conf");
	for(int i=0;i<pref_settings.size();i++){
		cout<<pref_settings.at(i)<<"\n";
		save<<pref_settings.at(i)<<"\n";
	}
	save.close();
}

void Preferences_Window::on_close_button(){
	hide();
}


