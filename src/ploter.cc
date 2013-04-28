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

#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <cairomm/context.h>
#include <cairomm/matrix.h>
#include <cairomm/surface.h>

using namespace std;

std::vector<Glib::ustring> Census_Plot::unit_vector;

std::string Census_Plot::to_precision(double num, int n) {
    if(num == 0)
      return "0";

    double d = std::ceil(std::log10(num < 0 ? -num : num));
    int power = n - (int)d;
    double magnitude = std::pow(10., power);
    long shifted = ::round(num*magnitude);

    std::ostringstream oss;
    oss << shifted/magnitude;
    return oss.str();
}

Census_Plot::Census_Plot(){
	unit_vector = fun.read("./name-store/unit_names.txt");

}

bool Census_Plot::on_draw(const Cairo::RefPtr<Cairo::Context>& cr){
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	int text_width, text_height;
	double xs = width/20.0;
	double xsb = width/12.0;
	double ys = height/18.0;
	double ysb = (height - (2*ys))/10.0;
	int red_colour[] = {51, 170, 153, 255, 255};
	int green_colour[] = {181, 102, 204, 187, 68};
	int blue_colour[] = {229, 204, 0, 51, 68};
	int kint = rand() % 5;
	int kint_store = kint;
	int kint_pan;

	std::vector<Glib::ustring> stat_vector = gTest::instance().get_stat_vector();
	int n_lines = stat_vector.size()/3;
	if(stat_vector.size()>1){
		cr->set_source_rgba(0.0, 0.0, 0.0, 0.95);
		Glib::RefPtr<Pango::Layout> date_label = create_pango_layout("Time");

		date_label->get_pixel_size(text_width, text_height);
		cr->move_to(1.5*xs + ((width-(5.5*xs))/2) - (text_width/2), height - ysb/3.5 - (text_height/2));
		date_label->show_in_cairo_context(cr);

		Glib::RefPtr<Pango::Layout> name_label;

		std::vector<double> values_vector = gTest::instance().get_value_vector();
		if(values_vector.size()>1){
			double split = (values_vector.size()+0.00)/n_lines;
			double step_width = ((width-(5.5*xs))/(split-1));
			int larger = 0;
			double miner = values_vector.at(0);
			double maxer = values_vector.at(0);
			for(int i=0; i<split*n_lines; i++){
				if(values_vector.at(i)>maxer)
					maxer = values_vector.at(i);
				else if(values_vector.at(i)<miner)
					miner = values_vector.at(i);
			}
			if((maxer==0)&&(miner==0)){
				maxer = 8;
				miner = -8;
			}
			if(abs(maxer)>abs(miner))
				larger = 1;

			cr->set_line_width(2.0);
			cr->set_source_rgba(0.0, 0.0, 0.0, 0.95);
			if(miner>=0){
				cr->move_to(1.5*xs, height- ys);
				cr->line_to(width - 4*xs, height - ys);
			}
			else if(maxer<=0){
				cr->move_to(1.5*xs, ys);
				cr->line_to(width - 4*xs, ys);
			}
			else{
				cr->move_to(1.5*xs, ys + 0.5*(height - 2*ys));
				cr->line_to(width - 4*xs, ys + 0.5*(height - 2*ys));
			}
			cr->move_to(1.5*xs, ys);
			cr->line_to(1.5*xs, height - ys);
			cr->stroke();

			// Print the axis lines and then labels
			Glib::RefPtr<Pango::Layout> y_label;
			cr->set_line_width(1.0);
			for(int i=0; i<11; i++){
				cr->set_source_rgba(0.0, 0.0, 0.0, 0.25);
				cr->move_to(1.5*xs -2 , (height - ys) - (i*ysb) + 0.5);
				cr->line_to(width - 4*xs, (height - ys) - (i*ysb) + 0.5);
				cr->stroke();
				cr->set_source_rgba(0.0, 0.0, 0.0, 0.95);
				if(miner>=0){
					if(((maxer/10)*i)<1)
						y_label = create_pango_layout(to_precision(((maxer/10)*i), 2));
					else
						y_label = create_pango_layout(to_precision(((maxer/10)*i), 3));
				}
				else if(maxer<=0)
					y_label = create_pango_layout(to_precision((miner - (miner/10)*i), 3));
				else if(larger == 1)
					y_label = create_pango_layout(to_precision((-maxer + (maxer/5)*i), 3));
				else
					y_label = create_pango_layout(to_precision((miner - (miner/5)*i), 3));
				y_label->get_pixel_size(text_width, text_height);
				cr->move_to(1.5*xs -6 -text_width, (height - ys) - (i*ysb) + 0.5 - text_height/2);
				y_label->show_in_cairo_context(cr);
				cr->stroke();
			}
		//// draw lines
			cr->set_line_width(2.0);
			for(int k=0; k<n_lines; k++){
				kint = kint_store + k;
				while(kint>4)
					kint = kint-5;
				cr->set_source_rgba(red_colour[kint]/256.0, green_colour[kint]/256.0, blue_colour[kint]/256.0, 1);
				for(int i=0; i<split-1; i++){
					if(miner>=0){
						cr->move_to(1.5*xs +(i*step_width), height - ys - (values_vector.at((k*split)+i) * (height - 2*ys) / maxer) +0.5);
						cr->curve_to(1.5*xs +(i*step_width) + step_width/2, height - ys - (values_vector.at((k*split)+i) * (height - 2*ys) / maxer) +0.5,
						1.5*xs +((i+1)*step_width) - step_width/2, height -ys - (values_vector.at((k*split)+i+1) * (height - 2*ys) / maxer) +0.5,
						1.5*xs +((i+1)*step_width), height -ys - (values_vector.at((k*split)+i+1) * (height - 2*ys) / maxer) +0.5);
					}
					else if(maxer<=0){
						cr->move_to(1.5*xs +(i*step_width), ys + (values_vector.at((k*split)+i) * (height - 2*ys) / miner));
						cr->curve_to(1.5*xs +(i*step_width) + step_width/2, ys + (values_vector.at((k*split)+i) * (height - 2*ys) / miner),
						1.5*xs +((i+1)*step_width) - step_width/2, ys + (values_vector.at((k*split)+i+1) * (height - 2*ys) / miner),
						1.5*xs +((i+1)*step_width), ys + (values_vector.at((k*split)+i+1) * (height - 2*ys) / miner));
					}
					else if(larger == 1){
						cr->move_to(1.5*xs +(i*step_width), height/2 - (values_vector.at((k*split)+i) * (height - 2*ys) / (2*maxer)) +0.5);
						cr->curve_to(1.5*xs +(i*step_width) + step_width/2, height/2 - (values_vector.at((k*split)+i) * (height - 2*ys) / (2*maxer)) +0.5,
						1.5*xs +((i+1)*step_width) - step_width/2, height/2 - (values_vector.at((k*split)+i+1) * (height - 2*ys) / (2*maxer)) +0.5,
						1.5*xs +((i+1)*step_width), height/2 - (values_vector.at((k*split)+i+1) * (height - 2*ys) / (2*maxer)) +0.5);
					}
					else{
						cr->move_to(1.5*xs +(i*step_width), height/2 + (values_vector.at((k*split)+i) * (height - 2*ys) / (2*miner)));
						cr->curve_to(1.5*xs +(i*step_width) + step_width/2, height/2 + (values_vector.at((k*split)+i) * (height - 2*ys) / (2*miner)),
						1.5*xs +((i+1)*step_width) - step_width/2, height/2 + (values_vector.at((k*split)+i+1) * (height - 2*ys) / (2*miner)),
						1.5*xs +((i+1)*step_width), height/2 + (values_vector.at((k*split)+i+1) * (height - 2*ys) / (2*miner)));
					}
				}
				cr->stroke();
			}
			double width_end = 1.5*xs +((split-1)*step_width) + 4;
			for(int k=0; k<n_lines; k++){
				kint_pan = kint_store + k;
				while(kint_pan>4)
					kint_pan = kint_pan-5;
				cr->set_source_rgba(red_colour[kint_pan]/256.0, green_colour[kint_pan]/256.0, blue_colour[kint_pan]/256.0, 1);
				if(stat_vector.at(k*3).find('<')!=-1)
					stat_vector.at(k*3) = stat_vector.at(k*3).substr(3, stat_vector.at(k*3).length() - 7);
				name_label = create_pango_layout(stat_vector.at(k*3));
				name_label->get_pixel_size(text_width, text_height);
				if(miner>=0)
					cr->move_to(width_end, height -ys - (values_vector.at((k*split)+split-1) * (height - 2*ys) / maxer) +0.5 - (text_height/2));
				else if(maxer<=0)
					cr->move_to(width_end, ys + (values_vector.at((k*split)+split-1) * (height - 2*ys) / miner) - (text_height/2));
				else if(larger==1)
					cr->move_to(width_end, height/2 - (values_vector.at((k*split)+split-1) * (height - 2*ys) / (2*maxer)) +0.5 - (text_height/2));
				else
					cr->move_to(width_end, height/2 + (values_vector.at((k*split)+split-1) * (height - 2*ys) / (2*miner)) - (text_height/2));
				name_label->show_in_cairo_context(cr);
				cr->stroke();
			}
		}
	}
	return true;
}
