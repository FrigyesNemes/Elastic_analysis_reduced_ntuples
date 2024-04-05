all: stat 

stat: stat.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		stat.cc -o stat
