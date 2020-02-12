OBJ = createdb
sourcefile = main.cc UserDefine.hh

MYSQLCFLAGS  = $(shell mysql_config --cflags)
MYSQLLIBS    = $(shell mysql_config --libs)

CFLAGS = -g -Wall -O2

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(MYSQLCFLAGS) $(MYSQLLIBS) -o $@ $(filter %.cc ,$(sourcefile))

clean:
	rm -f *~ *.o  $(OBJ)
