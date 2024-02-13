OBJ = createdb
sourcefile = main.cc UserDefine.hh

MYSQLCFLAGS  = $(shell mysql_config --cflags)
MYSQLLIBS    = $(shell mysql_config --libs)

CFLAGS = -g -Wall -O2

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(MYSQLCFLAGS) -o $@ $(filter %.cc ,$(sourcefile)) $(MYSQLLIBS)

clean:
	rm -f *~ *.o  $(OBJ)
