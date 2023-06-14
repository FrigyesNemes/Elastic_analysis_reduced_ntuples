
void make_plots()
{
	TFile *myfile = new TFile("../output.root") ;

	TCanvas c ;
	
	TH1F *hist1 = (TH1F *)myfile->Get("dx_103_104") ;
	hist1->Draw("colz") ;

}
