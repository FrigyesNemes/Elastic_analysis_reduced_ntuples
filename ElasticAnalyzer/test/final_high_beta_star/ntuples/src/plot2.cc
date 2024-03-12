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

const string directory="/afs/cern.ch/work/f/fnemes/main_workspace/2023/Elastic_analysis/E_CM_900_GeV_beta_star_100_m/Lxplus9/CMSSW_10_6_17/src/ElasticAnalysis/ElasticAnalyzer/test/final_high_beta_star/ntuples/" ;

int draw(int run, string histoname1, string histoname2)
{

  stringstream ss ;
  ss << run ;

  TFile *myfile = TFile::Open((directory + "output_LBRT_run_" + ss.str() + ".0_re_reco_Totem1.root").c_str()) ;

  TCanvas c;

  gStyle->SetLineScalePS(.3) ;

  c.SetGridx() ;	
  c.SetGridy() ;	
  // c.SetLogz() ;  

  TH2F *temp1 = (TH2F *)myfile->Get(histoname1.c_str()) ;
  TH2F *temp2 = (TH2F *)myfile->Get(histoname2.c_str()) ;

  temp2->SetLineColor(kRed) ;
  temp2->SetMarkerColor(kRed) ;
  temp1->Draw("colz") ;
  temp2->Draw("same scatt") ;
  
  temp1->SetTitle(("run=" + ss.str() + "    compare " + histoname1 + " " + histoname2).c_str()) ;
  
  if(histoname1.substr(0,2).compare("dy") == 0)
  {
    TLine *line = new TLine(-20, -2.0, 20, 2.3) ;
    line->Draw("same") ;
    line->SetLineStyle(kDashed) ;
    
    temp1->GetXaxis()->SetTitle("y (mm)") ;
    temp1->GetYaxis()->SetTitle("#Delta y (mm)") ;
  }
  else
  {
    temp1->GetXaxis()->SetTitle("x (mm)") ;
    temp1->GetYaxis()->SetTitle("#Delta x (mm)") ;
  }

  string ofilename = "fig/run_" + ss.str() + "_" + histoname1 + "_" + histoname2 + ".pdf" ;
  c.SaveAs(ofilename.c_str()) ;
  myfile->Close() ;
}

int main(int argc, char *argv[])
{
  if(argc != 1) 
  {
    cout << "Please provide the filename!" << endl ;
    exit(1) ;
  }
  
  vector<int> runs ;
  
  runs.push_back(324467) ;
  runs.push_back(324485) ;
  runs.push_back(324498) ;
  runs.push_back(324517) ;
  runs.push_back(324526) ;
  runs.push_back(324529) ;
  runs.push_back(324534) ;

  
  for(vector<int>::iterator it = runs.begin() ; it != runs.end() ; ++it)
  {
    draw(*it, "dx_4_24", "dx_5_25") ;
    draw(*it, "dy_4_24", "dy_5_25") ;
    draw(*it, "dx_104_124", "dx_105_125") ;
    draw(*it, "dy_104_124", "dy_105_125") ;
  }
}
