TARGET = readtrigger.cgi readfileinfo.cgi rateone.cgi rateday.cgi
CXXFLAGS = -Wall -fPIC -O3 -g
LIBS = -lz
MYSQLLIBS = $(shell mysql_config --libs)
MYSQLINC = -I$(shell mysql_config --cflags)

all: $(TARGET)

%.o: %.c 
	gcc  $(CFLAGS) -c $< -o $@

%.o: %.cc 
	g++  $(CXXFLAGS) $(MYSQLINC) -c $< -o $@

%.o: %.cpp 
	g++  $(CXXFLAGS) $(MYSQLINC) -c $< -o $@


readtrigger.cgi: readtrigger.o
	g++ readtrigger.o $(LIBS) $(MYSQLLIBS) -o readtrigger.cgi

readfileinfo.cgi: readfileinfo.o
	g++ readfileinfo.o  $(LIBS) $(MYSQLLIBS) -o readfileinfo.cgi

rateone.cgi: rateone.o
	g++ rateone.o  $(LIBS) $(MYSQLLIBS) -o rateone.cgi

rateday.cgi: rateday.o
	g++ rateday.o  $(LIBS) $(MYSQLLIBS) -o rateday.cgi

clean:
	rm -f *.o
	rm -f $(TARGET)
