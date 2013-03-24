#ifndef GTKMM_PLOTTER
#define GTKMM_PLOTTER

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>

class Census_Plot : public Gtk::DrawingArea
{
public:
	Census_Plot();
	virtual ~Census_Plot();
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

protected:
};

#endif
