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

#ifndef GTKMM_PLOTTER
#define GTKMM_PLOTTER

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include "functions.h"

class Census_Plot : public Gtk::DrawingArea{
public:
	Census_Plot();
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

protected:
	functions fun;
	Glib::ustring to_precision(double num, int n);
};

#endif
