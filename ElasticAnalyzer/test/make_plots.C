
void make_plots()
{
	vector<string> filenames ;

  filenames.push_back("RP_correlation") ;
  filenames.push_back("RP_covariance") ;
  filenames.push_back("diff_x_vs_dx_124_104_vs_104") ;
  filenames.push_back("diff_x_vs_dx_124_104_vs_124") ;
  filenames.push_back("diff_x_vs_dx_125_105_vs_105") ;
  filenames.push_back("diff_x_vs_dx_125_105_vs_125") ;
  filenames.push_back("diff_x_vs_dx_24_4_vs_24") ;
  filenames.push_back("diff_x_vs_dx_24_4_vs_4") ;
  filenames.push_back("diff_x_vs_dx_25_5_vs_25") ;
  filenames.push_back("diff_x_vs_dx_25_5_vs_5") ;
  filenames.push_back("diff_y_vs_dy_124_104_vs_104") ;
  filenames.push_back("diff_y_vs_dy_124_104_vs_124") ;
  filenames.push_back("diff_y_vs_dy_125_105_vs_105") ;
  filenames.push_back("diff_y_vs_dy_125_105_vs_125") ;
  filenames.push_back("diff_y_vs_dy_24_4_vs_24") ;
  filenames.push_back("diff_y_vs_dy_24_4_vs_4") ;
  filenames.push_back("diff_y_vs_dy_25_5_vs_25") ;
  filenames.push_back("diff_y_vs_dy_25_5_vs_5") ;
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
  filenames.push_back("scatter_plot_xy") ;
  filenames.push_back("scatter_plot_xy_103") ;
  filenames.push_back("scatter_plot_xy_104") ;
  filenames.push_back("scatter_plot_xy_105") ;
  filenames.push_back("scatter_plot_xy_123") ;
  filenames.push_back("scatter_plot_xy_124") ;
  filenames.push_back("scatter_plot_xy_125") ;
  filenames.push_back("scatter_plot_xy_23") ;
  filenames.push_back("scatter_plot_xy_24") ;
  filenames.push_back("scatter_plot_xy_25") ;
  filenames.push_back("scatter_plot_xy_3") ;
  filenames.push_back("scatter_plot_xy_4") ;
  filenames.push_back("scatter_plot_xy_5") ;
  filenames.push_back("th_x_local_vs_RP") ;
  filenames.push_back("th_y_local_vs_RP") ;
  filenames.push_back("xy_103_if_103_104") ;
  filenames.push_back("xy_103_if_103_105") ;
  filenames.push_back("xy_104_if_103_104") ;
  filenames.push_back("xy_105_if_103_105") ;
  filenames.push_back("xy_123_if_123_124") ;
  filenames.push_back("xy_123_if_123_125") ;
  filenames.push_back("xy_124_if_123_124") ;
  filenames.push_back("xy_125_if_123_125") ;
  filenames.push_back("xy_23_if_23_24") ;
  filenames.push_back("xy_23_if_23_25") ;
  filenames.push_back("xy_24_if_23_24") ;
  filenames.push_back("xy_25_if_23_25") ;
  filenames.push_back("xy_3_if_3_4") ;
  filenames.push_back("xy_3_if_3_5") ;
  filenames.push_back("xy_4_if_3_4") ;
  filenames.push_back("xy_5_if_3_5") ;

	TFile *myfile = new TFile("output.root", "READ") ;

	TCanvas c ;
  
  c.SetLogz() ;
  // c.SetGridx() ;
  // c.SetGridy() ;
	
	for(int i = 0 ; i < filenames.size() ; ++i)
	{
		TH2F *hist1 = (TH2F *)myfile->Get(filenames[i].c_str()) ;
		hist1->Draw("colz") ;
	
		c.SaveAs(("plots/" + filenames[i] + ".pdf").c_str()) ;
	}

}
