
void make_plots()
{
	vector<string> filenames ;
	
	filenames.push_back("dx_103_104") ;
	filenames.push_back("dx_103_104") ;
	filenames.push_back("dx_103_105") ;
	filenames.push_back("dx_123_124") ;
	filenames.push_back("dx_123_125") ;
	filenames.push_back("dx_23_24") ;
	filenames.push_back("dx_23_25") ;
	filenames.push_back("dx_3_4") ;
	filenames.push_back("dx_3_5") ;
	filenames.push_back("dy_103_104") ;
	filenames.push_back("dy_103_105") ;
	filenames.push_back("dy_123_124") ;
	filenames.push_back("dy_123_125") ;
	filenames.push_back("dy_23_24") ;
	filenames.push_back("dy_23_25") ;
	filenames.push_back("dy_3_4") ;
	filenames.push_back("dy_3_5") ;

	TFile *myfile = new TFile("output.root") ;

	TCanvas c ;
	
	for(int i = 0 ; i < filenames.size() ; ++i)
	{
		TH1F *hist1 = (TH1F *)myfile->Get(filenames[i].c_str()) ;
		hist1->Draw("colz") ;
	
		c.SaveAs(("plots/" + filenames[i] + ".pdf").c_str()) ;
	}

}
