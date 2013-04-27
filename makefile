CC=g++
CFLAGS=-O3 `pkg-config --cflags --libs gtkmm-3.0 libxml++-2.6 libcurl` -std=c++11
A=src/

OBJECTS = main.o gtest.o functions.o nationview.o treeview.o saveview.o ploter.o popup.o add.o

all : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o bin/nationstates-linux

main.o: $(A)main.cc $(A)gtest.cc
	$(CC) $(CFLAGS) -c $(A)main.cc

gtest.o: $(A)gtest.cc $(A)functions.cc $(A)nationview.cc $(A)saveview.cc $(A)treeview.cc $(A)ploter.cc $(A)add.cc
	$(CC) $(CFLAGS) -c $(A)gtest.cc

functions.o: $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)functions.cc

nationview.o: $(A)nationview.cc $(A)gtest.cc $(A)saveview.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)nationview.cc

treeview.o: $(A)treeview.cc $(A)gtest.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)treeview.cc

saveview.o: $(A)saveview.cc $(A)gtest.cc $(A)functions.cc $(A)popup.cc
	$(CC) $(CFLAGS) -c $(A)saveview.cc

ploter.o: $(A)ploter.cc $(A)gtest.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)ploter.cc

popup.o: $(A)popup.cc $(A)saveview.cc
	$(CC) $(CFLAGS) -c $(A)popup.cc

add.o: $(A)add.cc $(A)gtest.cc
	$(CC) $(CFLAGS) -c $(A)add.cc

clean:
	rm *.o
