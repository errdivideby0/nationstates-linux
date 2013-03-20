#include "gtest.h"
#include <gtkmm.h>

int main (int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

	gTest gtest;

	return app->run(gtest);
}
