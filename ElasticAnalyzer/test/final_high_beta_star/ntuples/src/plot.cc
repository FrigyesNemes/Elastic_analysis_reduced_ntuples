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


vector<string> plots_to_save ;
vector<string> plots_to_save2 ;
vector<string> plots_to_save3 ;


int main(int argc, char *argv[])
{
  if(argc != 3) 
  {
    cout << "Please provide the filename!" << endl ;
    exit(1) ;
  }
  
  const string filename(argv[1]) ;  

  ifstream plots_to_save_file(filename.c_str()) ;

  string word ;
  
  while(plots_to_save_file >> word)
  {
    plots_to_save.push_back(word) ;
  }
  
  plots_to_save_file.close() ;

  TFile *file = TFile::Open("../output_LBRT_run_324462.0_re_reco_Totem1.root") ;

  TCanvas c ;

  for(int i = 0 ; i < plots_to_save.size() ; ++i)
  {
    string plot_to_save = plots_to_save[i] ;
  
    TH2F *histo = (TH2F *)file->Get(plot_to_save.c_str()) ;
    
    if(histo != NULL)
    {
      histo->Draw("colz") ;
      c.SetLogz() ;
 
      c.SetGridx() ;
      c.SetGridy() ;

      c.SaveAs(("../fig/2024_02_29_plots_for_meeting/" + plot_to_save + ".pdf").c_str()) ;
    }
    else
    {
      cout << "Histogram does not exist in root file" << plot_to_save << endl ;
    }
  
  }


  const string filename2(argv[2]) ;  

  ifstream plots_to_save_file2(filename2.c_str()) ;

  while(plots_to_save_file2 >> word)
  {
    plots_to_save2.push_back(word) ;

    plots_to_save_file2 >> word ;

    plots_to_save3.push_back(word) ;
  }
  
  plots_to_save_file2.close() ;
  gStyle->SetLineScalePS(.3) ;

  for(int i = 0 ; i < plots_to_save2.size() ; ++i)
  {
    string plot_to_save2 = plots_to_save2[i] ;
    string plot_to_save3 = plots_to_save3[i] ;
  
    TH2F *histo2 = (TH2F *)file->Get(plot_to_save2.c_str()) ;
    TH2F *histo3 = (TH2F *)file->Get(plot_to_save3.c_str()) ;
    
    if(histo2 != NULL)
    {
      histo2->SetMarkerColor(kRed) ;
      histo3->SetMarkerColor(kBlue) ;

      histo2->SetMarkerStyle(20) ;
      histo3->SetMarkerStyle(20) ;

      histo2->SetMarkerSize(0.4) ;
      histo3->SetMarkerSize(0.1) ;

      histo2->Draw("scat") ;
      histo3->Draw("same scat") ;

      c.SetLogz() ;
 
      c.SetGridx() ;
      c.SetGridy() ;

      c.SaveAs(("../fig/2024_02_29_plots_for_meeting/" + plot_to_save2 + ".pdf").c_str()) ;
      c.SaveAs(("../fig/2024_02_29_plots_for_meeting/" + plot_to_save2 + ".png").c_str()) ;
    }
    else
    {
      cout << "Histogram does not exist in root file" << plot_to_save2 << endl ;
    }
  
  }

  
}
