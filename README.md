To compile on archlinux install the packages:

	pacman -Sy libxml++ gtkmm3 curl

Then use the build command:

	g++ main.cc treeview.cc saveview.cc gtest.cc functions.cc -o Nationstates `pkg-config --cflags --libs gtkmm-3.0 libxml++-2.6 libcurl` -std=c++11

Run with ./Nationstates


