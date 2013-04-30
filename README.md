If you used the program before May 2013 and have saved data that you would like to update to the newest data format,
in the bin folder type (backup first!)
	
	g++ -O3 convert.cc -o convert `pkg-config --cflags --libs glib-2.0` -std=c++11
	./convert

This will convert all your nations' saved data to the new format.


Build instructions

Archlinux

	sudo pacman -Sy libxml++ gtkmm3 glib2 curl base-devel
	make
	cd bin
	./nationstates-linux

Ubuntu/Xubuntu

	sudo apt-get install libcurl4-openssl-dev libgtkmm-3.0-dev libglib2.0-dev libxml++2.6-dev
	make
	cd bin
	./nationstates-linux
