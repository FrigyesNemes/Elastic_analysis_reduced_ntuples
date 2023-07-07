#include <iostream>

#include "TFile.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TStyle.h"

using namespace std;

TCanvas c ;

TFile *myfile = NULL ;

void plot(string name1, string name2, bool flip = false)
{
	string name = "23_25";

	TH2F *hist1 = (TH2F *)myfile->Get(name1.c_str()) ;
	TH2F *hist2 = (TH2F *)myfile->Get(name2.c_str()) ;

	hist1->SetMarkerStyle(6) ;
	hist2->SetMarkerStyle(6) ;

	hist1->SetMarkerColor(kRed) ;
	hist2->SetMarkerColor(kBlue) ;

	hist1->Draw("") ;
	hist1->GetXaxis()->SetRangeUser(2, 20) ;

	if(flip) 	hist1->GetYaxis()->SetRangeUser(3, 14) ;
	else 	hist1->GetYaxis()->SetRangeUser(-10, 0) ;

	hist1->GetXaxis()->SetTitle("x (mm)") ;
	hist1->GetYaxis()->SetTitle("y (mm)") ;


	hist2->Draw("sames") ;
	c.Update() ;

	TPaveStats *pave = (TPaveStats *)hist1->FindObject("stats") ;
	pave->SetTextColor(kRed) ;

	TPaveStats *pave2 = (TPaveStats *)hist2->FindObject("stats") ;
	pave2->SetTextColor(kBlue) ;
	pave2->SetY1(-7) ;
	pave2->SetY2(-4) ;

	c.SaveAs(("plots/2023_07_06_degree_8/overlaps2/newoverlap_" + name1 + ".png").c_str());
	c.SaveAs(("plots/2023_07_06_degree_8/overlaps2/newoverlap_" + name1 + ".pdf").c_str());
}


main()
{

	c.SetGridx() ;
	c.SetGridy() ;

	gStyle->SetLineScalePS(.3) ;
	// gStyle->SetOptStat("neo") ;

	cout << "hello" << endl ;
	myfile = new TFile("output.root", "READ") ;
	
	cout << "myfile " << myfile << endl ;
	
	plot("xy_3_if_3_4", "xy_4_if_3_4", true) ;
	plot("xy_3_if_3_5", "xy_5_if_3_5") ;

	plot("xy_23_if_23_24", "xy_24_if_23_24", true) ;
	plot("xy_23_if_23_25", "xy_25_if_23_25") ;

	plot("xy_103_if_103_104", "xy_104_if_103_104", true) ;
	plot("xy_103_if_103_105", "xy_105_if_103_105") ;

	plot("xy_123_if_123_124", "xy_124_if_123_124", true) ;
	plot("xy_123_if_123_125", "xy_125_if_123_125") ;

}
