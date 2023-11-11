#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include "math.h"

using namespace std;

#include "stdlib.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMinuit.h"
#include "TStyle.h"
#include "TFitResult.h"

string filename1 = "result_version5_Totem1.txt" ;
string filename2 = "result_version1_ZeroBias.txt" ;


void test_other(int rp_to_be_tested, long int event_number_to_test, bool &success, double &other_x, double &other_y)
{

	success = false ;

	ifstream input_file1(filename2.c_str()) ;

	string word1 ;
	string word2 ;
	string word3 ;
	string word4 ;
	string word5 ;
	string word6 ;
	string word7 ;
	string word8 ;

	long int event_number = 0 ;
	int  track_index = 0 ;
	int rp_index = 0 ;
	
	double x,y ;
	
	while(input_file1 >> word1 >> word2 >> event_number >> word3 >> track_index >> word4 >> rp_index >> word5 >> word6 >> x >> word7 >> word8 >> y) 
	{
		if(word3.compare("track_index") != 0)
		{
			cout << "Error" << endl ;
			exit(1) ;
		}
		
		if((rp_index == rp_to_be_tested) && (event_number_to_test == event_number))
		{
		 // cout << "rp2 found " <<  endl ;
		 other_x = x ;
		 other_y = y ;
		 
		 success = true ;
		}
	}
	
	input_file1.close() ;
}

void test(int rp_to_be_tested)
{
	ifstream input_file1(filename1.c_str()) ;

	string word1 ;
	string word2 ;
	string word3 ;
	string word4 ;
	string word5 ;
	string word6 ;
	string word7 ;
	string word8 ;

	long int event_number = 0 ;
	int  track_index = 0 ;
	int rp_index = 0 ;
	
	double x,y ;
	
	while(input_file1 >> word1 >> word2 >> event_number >> word3 >> track_index >> word4 >> rp_index >> word5 >> word6 >> x >> word7 >> word8 >> y) 
	{
		if(word3.compare("track_index") != 0)
		{
			cout << "Error" << endl ;
			exit(1) ;
		}
		
		if(rp_index == rp_to_be_tested)
		{
		 // cout << "rp found " <<  endl ;
		 double other_x, other_y ;
		 bool success = false ;
		 test_other(rp_to_be_tested, event_number, success, other_x, other_y) ;
		 
		 if(success) cout << "Comparison possible: " << rp_to_be_tested << " " << event_number << " " << x << " " << other_x << " " << y << " " << other_y << endl ;
		 // cout << endl ;
		}
	}
	cout << endl ;
}

int main()
{
	test(3) ;
	test(4) ;
	test(5) ;

	test(23) ;
	test(24) ;
	test(25) ;

	test(103) ;
	test(104) ;
	test(105) ;

	test(123) ;
	test(124) ;
	test(125) ;
	
	cout << "Success" << endl ;	
	exit(0) ;
}	
