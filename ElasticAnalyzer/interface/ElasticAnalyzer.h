
#include "TROOT.h"
#include "TFile.h"
#include "TMinuit.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TFitResult.h"
#include "TPaveStats.h"
#include "TF1.h"
#include "TMath.h"

using namespace std;

struct RP_struct_type
{
  unsigned int rpDecId ;

	Bool_t		validity ;

	Double_t	x ;
	Double_t	y ;

	Double_t	thx ;
	Double_t	thy ;

	UInt_t		uPlanesOn ;
	UInt_t		vPlanesOn ;
  
  RP_struct_type() ;
  void clear_variables() ;
  
} ;

RP_struct_type::RP_struct_type()
{
  clear_variables() ;
}

void RP_struct_type::clear_variables()
{
  rpDecId = 0 ;
  validity = kFALSE ;

  x = 0 ;
  y = 0 ;

  thx = 0 ;
  thy = 0 ;

  uPlanesOn = 0 ;
  vPlanesOn = 0 ;
  
}

class THorizontal_and_vertical_xy_pairs_to_match
{
  public:

  double hor_x, hor_y ;
  double ver_x, ver_y ;

  THorizontal_and_vertical_xy_pairs_to_match(double, double, double, double) ;
}  ;

map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>> map_of_THorizontal_and_vertical_xy_pairs_to_match ;

THorizontal_and_vertical_xy_pairs_to_match::THorizontal_and_vertical_xy_pairs_to_match(double hor_x, double hor_y, double ver_x, double ver_y) : hor_x(hor_x), hor_y(hor_y), ver_x(ver_x), ver_y(ver_y)
{
}

TH2F *test_hist_ver = new TH2F("test_hist_ver", "test_hist_ver" , 100, -20.0, 20.0, 100, -20.0, 20.0);
TH2F *test_hist_hor = new TH2F("test_hist_hor", "test_hist_hor" , 100, -20.0, 20.0, 100, -20.0, 20.0);

vector<THorizontal_and_vertical_xy_pairs_to_match *> *points = NULL ;
TH1F *chi2_contribution_hist = new TH1F("chi2_contribution_hist", "chi2_contribution_hist", 1000, 0, 100) ;
map<string, TH1F *> map_of_hists ;
string actual_detector_combination = "" ;

void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
  double chi2 = 0 ;

  double a = par[0] ;
  double b = par[1] ;
  double alpha = par[2] ;

  bool realistic = false ;

  double ex = 66e-3 ;
  double ey = 66e-3  ;

  if(realistic)
  {
    const double correction_from_expected_value = sqrt(12) ;
    ex = ex / correction_from_expected_value ;
    ey = ey / correction_from_expected_value ;
  }

	map_of_hists[actual_detector_combination]->Reset() ;

  for(unsigned int i = 0 ; i < points->size() ; ++i)
  {
    double hor_x = (cos(alpha) * (*points)[i]->hor_x) - (sin(alpha) * (*points)[i]->hor_y) + a ;
    double hor_y = (sin(alpha) * (*points)[i]->hor_x) + (cos(alpha) * (*points)[i]->hor_y) + b ;

    double dx = (hor_x - (*points)[i]->ver_x) / ex ;
    double dy = (hor_y - (*points)[i]->ver_y) / ey  ;

    double chi2_contribution = (dx*dx) + (dy*dy) ;
    chi2 += chi2_contribution ;

    if(chi2_contribution > 1e4) test_hist_hor->Fill((*points)[i]->hor_x, (*points)[i]->hor_y) ;
    if(chi2_contribution > 1e4) test_hist_ver->Fill((*points)[i]->ver_x, (*points)[i]->ver_y) ;

    chi2_contribution_hist->Fill(chi2_contribution) ;

    map_of_hists[actual_detector_combination]->Fill(chi2_contribution) ;
  }

  f = chi2 ;
}

void MinuitFit(string key)
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "a", 0, 0.1, -3, 3, ierflg);
  gMinuit2->mnparm(1, "b", 0, 0.1, -3, 3, ierflg);
  gMinuit2->mnparm(2, "alpha", 0.0, 0.1, -TMath::Pi()/16.0, TMath::Pi()/16.0, ierflg);

  arglist[0] = 3 ;

  gMinuit2->mnexcm("FIX", arglist, 1, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);

  arglist[0] = 3 ;

  gMinuit2->mnexcm("RELEASE", arglist, 1, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);

  double par[4] ;
  double pare[4] ;

	gMinuit2->GetParameter(0, par[0], pare[0]) ;
	gMinuit2->GetParameter(1, par[1], pare[1]) ;
	gMinuit2->GetParameter(2, par[2], pare[2]) ;

  cout << "Final parameters: x: " << key << "  " << par[0] << " y: " << par[1] << " alpha: " << par[2] << "  number_of_points: " << (*points).size() << endl ;
}
