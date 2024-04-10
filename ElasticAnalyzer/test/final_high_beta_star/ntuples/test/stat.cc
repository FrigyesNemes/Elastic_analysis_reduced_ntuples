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

void stat_1()
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
	
	files.close() ;
	
	cout << endl ;
	
	for(map<int, mytype>::iterator it = mymap.begin() ; it != mymap.end() ; ++it)
	{
		cout << it->first << " " << it->second.get() << endl ;
	}
}

void stat_2(string diagonal) 
{
	ifstream runs("/afs/cern.ch/work/f/fnemes/main_workspace_github_ssh_4/Projects/TOTEM_Projects/Physics_projects/Physics_analysis/Proton_proton_scattering/Elastic_scattering/Projects/2023/E_CM_900_GeV_beta_star_100_m/General_settings/List_of_runs.txt") ;

	string word ;
	
	while(runs >> word)
	{
		string actual_filename = "/afs/cern.ch/work/f/fnemes/tmp/pp/E_CM_900_GeV_beta_star_100_m/Analysis_output_files/7291/Diagonals/DIAGONAL_" + diagonal + "/All_root_files_to_define_cuts_run_" + word + "/Generic.root" ;
      if(access(actual_filename.c_str(), F_OK) != 0)
		{
			cout << "Problem 1 " << word << endl ;
			continue ;
		}
		

		TFile *myfile = TFile::Open(actual_filename.c_str()) ;
	
		TH2D *histo1 = (TH2D *)(myfile->Get("P0000_PlotsCollection_x_mm_y_mm_near_left")) ;
		TH1D *histo2 = (TH1D *)(myfile->Get("P0057_PlotsCollection_dN_dt_GeV2")) ;
		
		if(histo1 != NULL)
		{
			cout << word << " " << histo1->GetEntries() << " " << histo2->GetEntries() << endl ;
		}
		else
		{	
			cout << "Problem 2 " << word << endl ;
		}

		myfile->Close() ;
	}
}

int main()
{
	// stat_1() ;
	// stat_2("LEFT_BOTTOM_RIGHT_TOP") ;
	stat_2("LEFT_TOP_RIGHT_BOTTOM") ;
}
