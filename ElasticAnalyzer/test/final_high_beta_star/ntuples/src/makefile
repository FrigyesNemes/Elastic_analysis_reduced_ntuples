all: plot 

plot: plot.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot.cc -o plot

