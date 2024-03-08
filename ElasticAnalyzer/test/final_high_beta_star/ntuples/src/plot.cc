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

vector<string> plots_to_save4 ;
vector<string> plots_to_save5 ;
vector<string> plots_to_save6 ;


int main(int argc, char *argv[])
{
  if(argc != 4) 
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

  vector<int> runs = {324462, 324532, 324536, 324458} ;

  int run = runs[3] ;

  stringstream ss ;
  ss << run ;

  TFile *file = TFile::Open(("../output_LBRT_run_" + ss.str() + ".0_re_reco_Totem1.root").c_str()) ;

  TCanvas c ;

  for(int i = 0 ; i < plots_to_save.size() ; ++i)
  {
    string plot_to_save = plots_to_save[i] ;
  
    TH2F *histo = (TH2F *)file->Get(plot_to_save.c_str()) ;
    
    histo->GetXaxis()->SetTitle("x (mm)") ;
    histo->GetYaxis()->SetTitle("y (mm)") ;
    
    if(histo != NULL)
    {
      histo->Draw("colz") ;
      c.SetLogz() ;
 
      c.SetGridx() ;
      c.SetGridy() ;

      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save + ".pdf").c_str()) ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save + ".png").c_str()) ;
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
  
  const bool overlap = true ;
  
  TLine line(-20, -0.15, 20, 0.15) ;
  
  line.SetLineStyle(kDashed) ;

  for(int i = 0 ; i < plots_to_save2.size() ; ++i)
  {
    string plot_to_save2 = plots_to_save2[i] ;
    string plot_to_save3 = plots_to_save3[i] ;
  
    TH2F *histo2 = (TH2F *)file->Get(plot_to_save2.c_str()) ;
    TH2F *histo3 = (TH2F *)file->Get(plot_to_save3.c_str()) ;
    
    string title1(histo2->GetTitle()) ;
    string title2(histo3->GetTitle()) ;
    string title = title1 + " " + title2 ;
    
    histo2->SetTitle(title.c_str()) ;
    
    histo2->GetXaxis()->SetTitle("x (mm)") ;
    histo2->GetYaxis()->SetTitle("y (mm)") ;
    histo3->GetXaxis()->SetTitle("x (mm)") ;
    histo3->GetYaxis()->SetTitle("y (mm)") ;
    
    string chunk = plot_to_save2.substr(0,2) ;
    
    cout << "chunk "  << chunk << endl ;
    
    if(chunk.compare("dx") == 0) 
    {
      histo2->GetXaxis()->SetTitle("x (mm)") ;
      histo2->GetYaxis()->SetTitle("#Delta x (mm)") ;
      histo3->GetXaxis()->SetTitle("x (mm)") ;
      histo3->GetYaxis()->SetTitle("#Delta x (mm)") ;
    }
    else if(chunk.compare("dy") == 0) 
    {
      histo2->GetXaxis()->SetTitle("y (mm)") ;
      histo2->GetYaxis()->SetTitle("#Delta y (mm)") ;
      histo3->GetXaxis()->SetTitle("y (mm)") ;
      histo3->GetYaxis()->SetTitle("#Delta y (mm)") ;
    }
    
    
    if(histo2 != NULL)
    {
      histo2->SetMarkerColor(kRed) ;
      histo3->SetMarkerColor(kBlue) ;

      histo2->SetMarkerStyle(20) ;
      histo3->SetMarkerStyle(20) ;

      histo2->SetMarkerSize(0.6) ;
      histo3->SetMarkerSize(0.4) ;

      histo2->Draw("scat") ;
      if(overlap) histo3->Draw("same scat") ;
      
      if(chunk.compare("dy") == 0) line.Draw("same") ;

      // c.SetLogz() ;
 
      c.SetGridx() ;
      c.SetGridy() ;
      
      string postfix = "" ;
      if(overlap) postfix = "_overlap" ;

      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save2 + postfix + ".pdf").c_str()) ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save2 + postfix +  ".root").c_str()) ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save2 + postfix +  ".png").c_str()) ;
    }
    else
    {
      cout << "Histogram does not exist in root file" << plot_to_save2 << endl ;
    }
  
  }


  const string filename3(argv[3]) ;  

  ifstream plots_to_save_file3(filename3.c_str()) ;

  while(plots_to_save_file3 >> word)
  {
    plots_to_save4.push_back(word) ;

    plots_to_save_file3 >> word ;

    plots_to_save5.push_back(word) ;

    plots_to_save_file3 >> word ;

    plots_to_save6.push_back(word) ;
  }
  
  plots_to_save_file3.close() ;
  gStyle->SetLineScalePS(.3) ;

  for(int i = 0 ; i < plots_to_save4.size() ; ++i)
  {
    string plot_to_save4 = plots_to_save4[i] ;
    string plot_to_save5 = plots_to_save5[i] ;
    string plot_to_save6 = plots_to_save6[i] ;
  
    TH2F *histo4 = (TH2F *)file->Get(plot_to_save4.c_str()) ;
    TH2F *histo5 = (TH2F *)file->Get(plot_to_save5.c_str()) ;
    TH2F *histo6 = (TH2F *)file->Get(plot_to_save6.c_str()) ;
    
    if(histo4 != NULL)
    {

      histo4->GetXaxis()->SetTitle("x (mm)") ;
      histo4->GetYaxis()->SetTitle("#Delta x (mm)") ;

      histo5->GetXaxis()->SetTitle("x (mm)") ;
      histo5->GetYaxis()->SetTitle("#Delta x (mm)") ;

      histo6->GetXaxis()->SetTitle("x (mm)") ;
      histo6->GetYaxis()->SetTitle("#Delta x (mm)") ;
      
      string chunk = plot_to_save4.substr(0,2) ;

      if(chunk.compare("dy") == 0) 
      {
      histo4->GetXaxis()->SetTitle("y (mm)") ;
      histo4->GetYaxis()->SetTitle("#Delta y (mm)") ;

      histo5->GetXaxis()->SetTitle("y (mm)") ;
      histo5->GetYaxis()->SetTitle("#Delta y (mm)") ;

      histo6->GetXaxis()->SetTitle("y (mm)") ;
      histo6->GetYaxis()->SetTitle("#Delta y (mm)") ;
      }
      

      histo4->SetMarkerColor(kRed) ;
      histo4->SetLineColor(kRed) ;
      histo4->SetLineWidth(0.5) ;
      histo5->SetMarkerColor(kBlue) ;
      histo6->SetMarkerColor(kGreen) ;

      histo4->SetMarkerStyle(20) ;
      histo5->SetMarkerStyle(20) ;
      histo6->SetMarkerStyle(20) ;

      histo4->SetMarkerSize(0.1) ;
      histo5->SetMarkerSize(0.1) ;
      histo6->SetMarkerSize(0.1) ;

      // histo5->Draw("same scat") ;
      histo6->Draw("colz") ;
      histo4->Draw("same scat") ;

      c.SetLogz() ;
 
      c.SetGridx() ;
      c.SetGridy() ;

      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save4 + ".pdf").c_str()) ;

      histo4->Draw("colz") ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save4 + "_v1.pdf").c_str()) ;

      histo5->Draw("colz") ;
      histo4->Draw("same scat") ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save4 + "_v2.pdf").c_str()) ;

      histo5->SetMarkerColor(kRed) ;
      histo5->SetLineColor(kRed) ;
      histo5->SetMarkerSize(0.2) ;
      histo6->Draw("colz") ;
      histo5->Draw("same colz") ;
      c.SaveAs(("fig/" + ss.str() + "/"  + plot_to_save4 + "_v3.pdf").c_str()) ;

    }
    else
    {
      cout << "Histogram does not exist in root file" << plot_to_save4 << endl ;
    }
  
  }
  
}
