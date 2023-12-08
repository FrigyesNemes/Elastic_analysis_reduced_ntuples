
void plots2()
{
	vector<string> filenames ;
	filenames.push_back("output_LBRT_run_324575.0_re_reco_ZeroBias") ;
	filenames.push_back("output_LBRT_run_324576.0_re_reco_ZeroBias") ;
	filenames.push_back("output_LBRT_run_324578.0_re_reco_ZeroBias") ;
	filenames.push_back("output_LBRT_run_324579.0_re_reco_ZeroBias") ;
	filenames.push_back("output_LBRT_run_324580.0_re_reco_ZeroBias") ;
	filenames.push_back("output_LBRT_run_324581.0_re_reco_ZeroBias") ;

	TCanvas c ;

	for(int i = 0 ; i < filenames.size() ; ++i)
	{
		TFile *myfile = TFile::Open((filenames[i] + ".root").c_str()) ;
		TH1D *hist0 = (TH1D *)myfile->Get("timestamps_zero_bias") ;
		TH1D *hist1 = (TH1D *)myfile->Get("pile_up_events_strict_LBRT") ;
		TH1D *hist2 = (TH1D *)myfile->Get("pile_up_events_strict_LTRB") ;
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
