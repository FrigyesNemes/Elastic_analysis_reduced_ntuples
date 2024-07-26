
void plots2()
{
	vector<string> filenames ;
	filenames.push_back("output_run_324457_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324458_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324460_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324461_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324462_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324464_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324465_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324467_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324469_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324471_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324485_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324498_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324509_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324513_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324517_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324518_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324519_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324520_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324524_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324526_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324527_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324528_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324529_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324530_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324531_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324532_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324534_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324535_re_reco_ZeroBias") ;
	filenames.push_back("output_run_324536_re_reco_ZeroBias") ;

	TCanvas c ;
	
	long unsigned int delta = (1539287481 - 1539453733) ;
	
   TH2D *hist_2d = new TH2D("hist_2d", "hist_2d", 100, 15392874e2 , 15394550e2, 100, 0, 0.3) ;	
	hist_2d->Draw("") ;
	
	for(int i = 0 ; i < filenames.size() ; ++i)
	{

		TFile *myfile = NULL ;	
		
		bool use_my_sources = true ;

		if(use_my_sources) myfile = TFile::Open(("plots/sources/" + filenames[i] + "_new.root").c_str()) ;
		else myfile = TFile::Open(("/eos/cms/store/group/phys_diffraction/fnemes/E_CM_900_GeV/Beta_star_100_m/ZeroBias/" + filenames[i] + ".root").c_str()) ;	
		
		TH1D *hist0 = (TH1D *)myfile->Get("timestamps_zero_bias") ;
		TH1D *hist1 = (TH1D *)myfile->Get("pile_up_events_LBRT_2RP") ;
		TH1D *hist2 = (TH1D *)myfile->Get("pile_up_events_LTRB_2RP") ;

		bool create_my_sources = false ;
		
		if(create_my_sources)
		{
			TFile *output = new TFile(("plots/sources/" + filenames[i] + "_new.root").c_str(), "UPDATE") ;
			output->cd() ;

			hist0->Write() ;
			hist1->Write() ;
			hist2->Write() ;

			output->Write() ;		
			output->Close() ;
			delete output ;
		}

		hist1->Divide(hist0) ;
		hist2->Divide(hist0) ;
		
		if((filenames[i].compare("output_run_324461_re_reco_ZeroBias") == 0) || (filenames[i].compare("output_run_324462_re_reco_ZeroBias") == 0) || (filenames[i].compare("output_run_324532_re_reco_ZeroBias") == 0) || (filenames[i].compare("output_run_324536_re_reco_ZeroBias") == 0))
		{
			hist1->SetLineColor(kBlue) ;
			hist2->SetLineColor(kRed) ;
			
			cout << filenames[i] << " " << hist1->GetBinContent(50) << " " << hist2->GetBinContent(50) << "\t " << 100.0 * (hist2->GetBinContent(50) - hist1->GetBinContent(50)) << endl ;			

		} else if((filenames[i].compare("output_run_324457_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324458_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324460_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324464_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324465_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324467_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324469_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324471_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324485_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324509_re_reco_ZeroBias") == 0) ||
		(filenames[i].compare("output_run_324513_re_reco_ZeroBias") == 0) 
		)
		{
			hist1->SetLineColor(kGreen) ;
			hist2->SetLineColor(kGreen) ;
		}
		else
		{
			hist1->SetLineColor(kBlack) ;
			hist2->SetLineColor(kBlack) ;
		}
		
		
		c.cd() ;

		if(i==0)
		{
			// hist1->GetYaxis()->SetRangeUser(0, 0.2) ;
			// hist1->Draw("same") ;
			// hist2->Draw("same") ;
		}
		else
		{
			hist1->Draw("same") ;
			hist2->Draw("same") ;
		}
		
		string suffix = "" ;
		if(use_my_sources) suffix = "_own" ;
		
		bool not_create_summary_figure = false ;
		
		if(not_create_summary_figure)
		{
			hist1->SaveAs(("plots/" + filenames[i] + "_result_LBRT_hist"+ suffix + ".root").c_str()) ;
			hist2->SaveAs(("plots/" + filenames[i] + "_result_LTRB_hist"+ suffix + ".root").c_str()) ;
			myfile->Close() ;
		}
	}

	c.SaveAs(("plots/result.root")) ;
	c.SaveAs(("plots/result.pdf")) ;
}
