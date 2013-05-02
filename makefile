CC=g++
CFLAGS=-O3
LDFLAGS=`pkg-config --cflags --libs glib-2.0 gtkmm-3.0 libxml++-2.6 libcurl` -std=c++11
A=src/

OBJECTS = main.o nationstates.o functions.o nationview.o statview.o saveview.o ploter.o rename.o add.o del.o preferences.o about.o

all : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o bin/nationstates-linux $(LDFLAGS)

main.o: $(A)main.cc $(A)nationstates.cc
	$(CC) $(CFLAGS) -c $(A)main.cc $(LDFLAGS)

nationstates.o: $(A)nationstates.cc $(A)functions.cc $(A)nationview.cc $(A)saveview.cc $(A)statview.cc $(A)ploter.cc $(A)add.cc $(A)preferences.cc $(A)about.cc
	$(CC) $(CFLAGS) -c $(A)nationstates.cc $(LDFLAGS)

functions.o: $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)functions.cc $(LDFLAGS)

nationview.o: $(A)nationview.cc $(A)nationstates.cc $(A)saveview.cc $(A)del.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)nationview.cc $(LDFLAGS)

statview.o: $(A)statview.cc $(A)nationstates.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)statview.cc $(LDFLAGS)

saveview.o: $(A)saveview.cc $(A)nationstates.cc $(A)functions.cc $(A)rename.cc
	$(CC) $(CFLAGS) -c $(A)saveview.cc $(LDFLAGS)

ploter.o: $(A)ploter.cc $(A)nationstates.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)ploter.cc $(LDFLAGS)

rename.o: $(A)rename.cc $(A)saveview.cc
	$(CC) $(CFLAGS) -c $(A)rename.cc $(LDFLAGS)

add.o: $(A)add.cc $(A)nationstates.cc
	$(CC) $(CFLAGS) -c $(A)add.cc $(LDFLAGS)

del.o: $(A)del.cc $(A)nationstates.cc
	$(CC) $(CFLAGS) -c $(A)del.cc $(LDFLAGS)

preferences.o: $(A)preferences.cc $(A)functions.cc
	$(CC) $(CFLAGS) -c $(A)preferences.cc $(LDFLAGS)

about.o: $(A)about.cc
	$(CC) $(CFLAGS) -c $(A)about.cc $(LDFLAGS)

clean:
	rm *.o
