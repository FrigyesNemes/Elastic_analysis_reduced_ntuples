all: plot lsq millepede

plot: plot.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		plot.cc -o plot

lsq: lsq.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		lsq.cc -o lsq

millepede: millepede.cc
	g++ --std=c++11 -Wall `root-config --libs` -lMinuit `root-config --cflags` \
		millepede.cc -o millepede
