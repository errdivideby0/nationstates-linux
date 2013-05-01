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

#include "about.h"
#include <cstdlib>
#include <gtkmm.h>
#include <iostream>
#include <fstream>

using namespace std;

About_Page::About_Page(): main_box(Gtk::ORIENTATION_VERTICAL), close_button("Close"){
	set_border_width(5);
	set_default_size(150, 0);
	set_title("About");
	add(main_box);
	main_box.pack_start(vbox_one);
	vbox_one.pack_start(label_one);
	vbox_one.set_size_request(1,1);
	label_one.set_label("Nationstates-linux is free software: you can redistribute it and/or modify\n"
						"it under the terms of the GNU General Public License as published by\n"
						"the Free Software Foundation, either version 3 of the License, or\n"
						"(at your option) any later version.\n"
						"\n"
						"Nationstates-linux is distributed in the hope that it will be useful,\n"
						"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
						"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
						"GNU General Public License for more details.\n");
	//label_one.set_line_wrap(true);
	//vbox_one.pack_start(label_two, Gtk::PACK_SHRINK);
	//label_two.set_markup(<a href=poloure.github.io/nationstates-linux/>Nationstates-linux page</a>);
	main_box.pack_start(bbox_one, Gtk::PACK_SHRINK);
	bbox_one.set_layout(Gtk::BUTTONBOX_END);
	bbox_one.pack_start(close_button);
	bbox_one.set_border_width(5);
	close_button.signal_clicked().connect(sigc::mem_fun(*this, &About_Page::on_close_button));
	show_all_children();
	}

void About_Page::on_close_button(){
	hide();
}
