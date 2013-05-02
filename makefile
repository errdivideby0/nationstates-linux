CC=g++
CFLAGS=-O3
LDFLAGS=`pkg-config --cflags --libs glib-2.0 gtkmm-3.0 libxml++-2.6 libcurl` -std=c++11
A=src/

OBJECTS = main.o gtest.o functions.o nationview.o treeview.o saveview.o ploter.o popup.o add.o del.o preferences.o about.o

all : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o bin/nationstates-linux $(LDFLAGS)

main.o: $(A)main.cc $(A)gtest.cc
	$(CC) $(CFLAGS) -c $(A)main.cc $(LDFLAGS)

gtest.o: $(A)gtest.cc $(A)functions.cc $(A)nationview.cc $(A)saveview.cc $(A)treeview.cc $(A)ploter.cc $(A)add.cc $(A)preferences.cc $(A)about.cc
	$(CC) $(CFLAGS) -c $(A)gtest.cc $(LDFLAGS)

functions.o: $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)functions.cc $(LDFLAGS)

nationview.o: $(A)nationview.cc $(A)gtest.cc $(A)saveview.cc $(A)del.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)nationview.cc $(LDFLAGS)

treeview.o: $(A)treeview.cc $(A)gtest.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)treeview.cc $(LDFLAGS)

saveview.o: $(A)saveview.cc $(A)gtest.cc $(A)functions.cc $(A)popup.cc
	$(CC) $(CFLAGS) -c $(A)saveview.cc $(LDFLAGS)

ploter.o: $(A)ploter.cc $(A)gtest.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)ploter.cc $(LDFLAGS)

popup.o: $(A)popup.cc $(A)saveview.cc
	$(CC) $(CFLAGS) -c $(A)popup.cc $(LDFLAGS)

add.o: $(A)add.cc $(A)gtest.cc
	$(CC) $(CFLAGS) -c $(A)add.cc $(LDFLAGS)

del.o: $(A)del.cc $(A)gtest.cc
	$(CC) $(CFLAGS) -c $(A)del.cc $(LDFLAGS)

preferences.o: $(A)preferences.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)preferences.cc $(LDFLAGS)

about.o: $(A)about.cc
	$(CC) $(CFLAGS) -c $(A)about.cc $(LDFLAGS)

clean:
	rm *.o
