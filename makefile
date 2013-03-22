all:
	g++ -O3 main.cc treeview.cc saveview.cc gtest.cc functions.cc -o nationstates-linux `pkg-config --cflags --libs gtkmm-3.0 libxml++-2.6 libcurl` -std=c++11