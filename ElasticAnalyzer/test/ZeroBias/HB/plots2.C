
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

	for(int i = 0 ; i < filenames.size() ; ++i)
	{
		TFile *myfile = TFile::Open((filenames[i] + ".root").c_str()) ;
		TH1D *hist0 = (TH1D *)myfile->Get("timestamps_zero_bias") ;
		TH1D *hist1 = (TH1D *)myfile->Get("pile_up_events_LBRT") ;
		TH1D *hist2 = (TH1D *)myfile->Get("pile_up_events_LTRB") ;
		hist1->Divide(hist0) ;
		hist2->Divide(hist0) ;
		
		c.cd() ;

		if(i==0)
		{
			hist1->GetYaxis()->SetRangeUser(0, 0.2) ;
			hist1->Draw() ;
			hist2->Draw("same") ;
		}
		else
		{
			hist1->Draw("same") ;
			hist2->Draw("same") ;
		}
		
		
		hist1->SaveAs(("plots/" + filenames[i] + "_result_LBRT_hist.root").c_str()) ;
		hist2->SaveAs(("plots/" + filenames[i] + "_result_LTRB_hist.root").c_str()) ;
		myfile->Close() ;
	}

	c.SaveAs(("plots/result.root")) ;
	c.SaveAs(("plots/result.pdf")) ;
}
