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

#include "ploter.h"
#include "gtest.h"

#include <cairomm/context.h>
#include <cairomm/matrix.h>
#include <cairomm/surface.h>

Census_Plot::Census_Plot(){
}

Census_Plot::~Census_Plot(){
}

bool Census_Plot::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	int xs, ys, ysb, xsb;
	xs = width / 10;
	xsb = width/12;
	ys = height - (height / 10);
	ysb = height/13;

	cr->set_line_width(2.0);

	cr->set_source_rgba(0.0, 0.0, 0.0, 0.78);
	cr->move_to(xs, ys);
	cr->line_to(width - xs, ys);
	cr->move_to(xs, ys);
	cr->line_to(xs, height - ys);
	cr->stroke();
	cr->set_line_width(1.0);
	cr->set_source_rgba(0.0, 0.0, 0.0, 0.5);
	for(int i=1; i<11; i++){
		cr->move_to(xs -2 , ys - (i*ysb) + 0.5);
		cr->line_to(width -xs, ys - (i*ysb) + 0.5);
	}
	cr->stroke();

	std::vector<Glib::ustring> stat_vector = gTest::instance().get_stat_vector();
	if(stat_vector.size()>0){
		cr->set_source_rgba(0.0, 0.0, 0.0, 0.95);
		Glib::RefPtr<Pango::Layout> date_label = create_pango_layout("date");

		int text_width;
		int text_height;

		date_label->get_pixel_size(text_width, text_height);
		cr->move_to(width/2 - (text_width/2), height - ysb/2 - (text_height/2));
		date_label->show_in_cairo_context(cr);


		Glib::RefPtr<Pango::Layout> name_label = create_pango_layout(stat_vector.at(0));
		name_label->get_pixel_size(text_width, text_height);
		cr->move_to(width/2 - (text_width/2), ysb/2 + (text_height/2));
		name_label->show_in_cairo_context(cr);


		Glib::RefPtr<Pango::Layout> layout_vertical = create_pango_layout(stat_vector.at(1));
		layout_vertical->get_pixel_size(text_width, text_height);
		cr->move_to(xsb/2 - text_height, height/2 + text_width/2);

		Cairo::Matrix rotater = Cairo::identity_matrix();
		rotater.rotate(4.71238898);
		cr->set_matrix(rotater);
		layout_vertical->show_in_cairo_context(cr);
	}

	return true;
}
