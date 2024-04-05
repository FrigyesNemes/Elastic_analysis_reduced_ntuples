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

#include "TMath.h"

#include "TF1.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMinuit.h"
#include "TStyle.h"

#include "TVector.h"
#include "TMatrix.h"
#include "TMarker.h"
#include "TFitResult.h"

#include "TRandom3.h"
#include "TMultiGraph.h"

#include <iomanip>

class mytype
{
	int entries ;
	
	void mytpe() { entries = 0 ; }

	public:

	void add(int plus)  { entries += plus ; } 
	int get()  { return entries ; } 
} ;

int main()
{
	ifstream files("files.txt") ;
	string word ;
	
	map<int, mytype> mymap ;
	
	while(files >> word)
	{
		TFile *myfile = TFile::Open(("root://eostotem//eos/totem/data/offline/2018/450GeV/beta100/Totem1/version5/" + word).c_str()) ;
		
		TTree *ntuple = (TTree *)(myfile->Get("Events")) ;
		
		cout << word << " " << word.substr(4,6) << " events " << ntuple->GetEntries() << endl ;
		mymap[atoi(word.substr(4,6).c_str())].add(ntuple->GetEntries()) ;
		
		myfile->Close() ;
	}
	
	cout << endl ;
	
	for(map<int, mytype>::iterator it = mymap.begin() ; it != mymap.end() ; ++it)
	{
		cout << it->first << " " << it->second.get() << endl ;
	}
	
}
