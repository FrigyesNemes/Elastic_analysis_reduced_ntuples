
vector<int> RP_numbers = {3, 4, 5, 23, 24, 25, 103, 104, 105, 123, 124, 125} ;

void plots()
{
  vector<double> runs = {324575, 324576, 324578, 324579, 324580, 324581} ;
  TCanvas c ;
  c.SetLogz() ;

  const int scenario_y_left_far_top = 1 ;
  const int scenario_y_right_far_top = 2 ;
  
  int scenario = scenario_y_right_far_top ;
  
  string histo_name = "" ;
  string histo2_name = "strips_u_v_positions" ;
  string histo3_name = "strips_v_per_plane_" ;

  string x_axis_name = "" ;
  string y_axis_name = "" ;
  
  if(scenario == scenario_y_left_far_top)
  {
    histo_name = "diff_y_vs_dy_24_4_vs_24" ;
    x_axis_name = "y_{mm, left far top}" ;
    y_axis_name = "#Delta y_{mm, left far top}" ;
  }
  else if(scenario == scenario_y_right_far_top)
  {
    histo_name = "diff_y_vs_dy_124_104_vs_124" ;
    x_axis_name = "y_{mm, right far top}" ;
    y_axis_name = "#Delta y_{mm, right far top}" ;
  }
  else
  {
    cout << "No such scneario!" << endl ;
    exit(1) ;
  }

  for(int i = 0 ; i < runs.size() ; ++i)
  {
    stringstream ss ;
    ss << runs[i] ;
    TFile *file = TFile::Open(("output_LBRT_run_" + ss.str() + ".0_re_reco_ZeroBias.root").c_str()) ;

    TH2D *hist = ((TH2D *)file->Get(histo_name.c_str())) ;
    hist->GetXaxis()->SetTitle(x_axis_name.c_str()) ;
    hist->GetYaxis()->SetTitle(y_axis_name.c_str()) ;
    hist->GetXaxis()->SetRangeUser(0, 20) ;
    
    hist->Draw("colz") ;
    
    c.SaveAs(("scripts/plots/c_" + histo_name + "_" + ss.str() + ".pdf").c_str()) ;

    TH2D *hist2 = ((TH2D *)file->Get("strips_u_v_positions")) ;
    hist2->GetXaxis()->SetTitle("position_{avg, u} (mm) ") ;
    hist2->GetYaxis()->SetTitle("position_{avg, v} (mm) ") ;
    hist2->Draw("colz") ;

    c.SaveAs(("scripts/plots/c_" + histo2_name + "_" + ss.str() + ".pdf").c_str()) ;

    double maximum = -1 ;
    TH1D *just_a_pointer = NULL ;
    
    for(int j = 0 ; j < RP_numbers.size() ; ++j)
    {
      stringstream ss2 ;
      ss2 << RP_numbers[j] ;

      TH1D *hist3 = ((TH1D *)file->Get((histo3_name + ss2.str()).c_str())) ;
      cout << hist3 << endl ;
      
      if(j == 0)
      {
        hist3->Draw("") ;
        just_a_pointer = hist3 ;
      }
      else hist3->Draw("same") ;
      
      if(maximum > hist3->GetMaximum()) maximum = hist3->GetMaximum() ;
      
    }

    just_a_pointer->GetYaxis()->SetRangeUser(0, maximum) ;
    c.SaveAs(("scripts/plots/c_" + histo3_name + "_" + ss.str() + ".pdf").c_str()) ;

    file->Close() ;
  }
  
  
}

