// -*- C++ -*-
//
// Package:    ElasticAnalysis/ElasticAnalyzer
// Class:      ElasticAnalyzer
//
/**\class ElasticAnalyzer ElasticAnalyzer.cc ElasticAnalysis/ElasticAnalyzer/plugins/ElasticAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Frigyes Janos Nemes
//         Created:  Fri, 12 May 2023 07:51:22 GMT
//
//


#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
 
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "TROOT.h"
#include "TFile.h"
#include "TMinuit.h"
#include "TRandom3.h"
#include "TTree.h"
#include "TH2F.h"

using namespace std;

vector<int> RP_numbers = {3, 4, 5, 23, 24, 25, 103, 104, 105, 123, 124, 125} ;

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

THorizontal_and_vertical_xy_pairs_to_match::THorizontal_and_vertical_xy_pairs_to_match(double hor_x, double hor_y, double ver_x, double ver_y) : hor_x(hor_x), hor_y(hor_y), ver_x(ver_x), ver_y(ver_y)
{
}

map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>> map_of_THorizontal_and_vertical_xy_pairs_to_match ;


class ElasticAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
  explicit ElasticAnalyzer(const edm::ParameterSet&);
  ~ElasticAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  virtual void clear_variables() ;
  virtual void addLabels() ;
  virtual void addHistos() ;
  virtual void addLabels(int, int) ;
  virtual void TestDetectorPair(map<unsigned int, RP_struct_type>::iterator, map<unsigned int, RP_struct_type>::iterator, unsigned int, unsigned int) ;
  virtual void Minimize() ;
  void MinimizeHorizontalVerticalPair(vector<THorizontal_and_vertical_xy_pairs_to_match *> &);

  int verbosity;

  edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> tracksToken_;  //used to select what tracks to read from configuration file

  std::string diagonal;  
  std::string outputFileName;  

  map<string, TH1F*> histosTH1F;
  map<string, TH2F*> histosTH2F;
  
  // Declaration of leaf types
  ULong64_t       event_info_timestamp;
  UInt_t          trigger_data_run_num;
  UInt_t          trigger_data_bunch_num;
  UInt_t          trigger_data_event_num;
  UInt_t          trigger_data_trigger_num;
  UInt_t          trigger_data_input_status_bits;

  RP_struct_type left_near ;
  RP_struct_type left_far ;
  RP_struct_type right_near ;
  RP_struct_type right_far ;

  RP_struct_type left_near_horizontal ;
  RP_struct_type left_far_horizontal ;
  RP_struct_type right_near_horizontal ;
  RP_struct_type right_far_horizontal ;

  TTree *tree ;
};

ElasticAnalyzer::ElasticAnalyzer(const edm::ParameterSet& iConfig) :  verbosity(iConfig.getUntrackedParameter<int>("verbosity")),
  tracksToken_(consumes<std::vector<CTPPSLocalTrackLite>>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
  diagonal(iConfig.getParameter<std::string>("diagonal")), outputFileName(iConfig.getParameter<std::string>("outputFileName"))
{
}

ElasticAnalyzer::~ElasticAnalyzer()
{
}

void ElasticAnalyzer::clear_variables() 
{
  event_info_timestamp = 0 ;
  trigger_data_run_num = 0 ;
  trigger_data_bunch_num = 0 ;
  trigger_data_event_num = 0 ;
  trigger_data_trigger_num = 0 ;
  trigger_data_input_status_bits = 0 ;

  left_near.clear_variables() ;
  left_far.clear_variables() ;
  right_near.clear_variables() ;
  right_far.clear_variables() ;

  left_near_horizontal.clear_variables() ;
  left_far_horizontal.clear_variables() ;
  right_near_horizontal.clear_variables() ;
  right_far_horizontal.clear_variables() ;

}

const double RP_distance_m = 7.0 ;
const double RP_distance_mm = RP_distance_m * 1e3 ;

const double dx_threshold_between_vertical_and_horizontal_mm = 0.2 ;
const double vertical_limit_mm = 0.4 ;

const double horizontal_boundary_mm = 20.0 ;
const double vertical_boundary_mm = 0.5 ;

TRandom3 myrand ;

void ElasticAnalyzer::TestDetectorPair(map<unsigned int, RP_struct_type>::iterator it1, map<unsigned int, RP_struct_type>::iterator it2, unsigned int detector_1, unsigned int detector_2)
{
  if((it1->first == detector_1) && (it2->first == detector_2))
  {
    stringstream ss_1 ;
    stringstream ss_2 ;

    ss_1 << it1->first ;
    ss_2 << it2->first ;

    string key_for_coords = ss_1.str() + "_" + ss_2.str() ;

    string name_x = "dx_" + ss_1.str() + "_" + ss_2.str() ;
    string name_y = "dy_" + ss_1.str() + "_" + ss_2.str() ;
    
    // cout << name_x << " " << name_y << endl ;

    histosTH2F[name_x]->Fill(it2->second.x, it2->second.x - it1->second.x) ;
    histosTH2F[name_y]->Fill(it2->second.y, it2->second.y - it1->second.y) ;

    histosTH1F[("h" + name_x).c_str()]->Fill(it2->second.x - it1->second.x) ;
    histosTH1F[("h" + name_y).c_str()]->Fill(it2->second.y - it1->second.y) ;

    if((fabs(it2->second.x - it1->second.x) < dx_threshold_between_vertical_and_horizontal_mm) && (fabs(it2->second.y - it1->second.y) < dx_threshold_between_vertical_and_horizontal_mm))
    {

      map_of_THorizontal_and_vertical_xy_pairs_to_match[key_for_coords].push_back(new THorizontal_and_vertical_xy_pairs_to_match(it1->second.x, it1->second.y, it2->second.x, it2->second.y)) ;
      
      cout << "to_be_saved " << key_for_coords << " " <<  it1->second.x << " " <<  it1->second.y << " " <<  it2->second.x << " " <<  it2->second.y << " " <<  endl ;

      string name_x2 = "xy_" + ss_1.str() + "_if_" + ss_1.str() + "_" + ss_2.str() ;
      string name_y2 = "xy_" + ss_2.str() + "_if_" + ss_1.str() + "_" + ss_2.str() ;

      // cout << name_x2 << " " << name_y2 << endl ;

      histosTH2F[name_x2]->Fill(it1->second.x, it1->second.y) ;
      histosTH2F[name_y2]->Fill(it2->second.x, it2->second.y) ;
    }
  }
}

void ElasticAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  EventNumber_t event_number = iEvent.id().event() ;
  RunNumber_t run_number = iEvent.run() ;


	if(verbosity > 0)
  {
    cout << "Eventinfo " << run_number << " " << event_number << " (" <<  iEvent.id() << ")" << endl ;
  }
  
  clear_variables() ;

  bool rp_valid_004 = false;
  bool rp_valid_005 = false;
  bool rp_valid_024 = false;
  bool rp_valid_025 = false;

  bool rp_valid_104 = false;
  bool rp_valid_105 = false;
  bool rp_valid_124 = false;
  bool rp_valid_125 = false;

  event_info_timestamp = iEvent.time().unixTime();
  trigger_data_run_num = run_number ;
  trigger_data_bunch_num = iEvent.bunchCrossing();
  trigger_data_event_num = event_number ;
  trigger_data_trigger_num = 0 ;
  trigger_data_input_status_bits = 0 ;

  std::uint32_t time_date_stamp = iEvent.time().unixTime();
  std::time_t temp = time_date_stamp;
  std::tm* t = std::gmtime(&temp);
  std::stringstream ss; // or if you're going to print, just input directly into the output stream
  ss << std::put_time(t, "%Y-%m-%d %I:%M:%S %p");
  std::string output = ss.str();

  cout << iEvent.time().unixTime() << " " << output << endl ;

  for(const auto& track : iEvent.get(tracksToken_) )
  {
    CTPPSDetId rpId(track.getRPId());
    unsigned int rpDecId = (100*rpId.arm()) + (10*rpId.station()) + (1*rpId.rp());
    
    // if(2,3,22,23)
    
    if((rpId.rp() == 2) || (rpId.rp() == 3))
    {
      if(verbosity > 1) cout << "a_horizontal " << rpDecId << " " << rpId.rp() << " " << rpId.arm() << " " << rpId.station() << endl ;
    }
    
    if(verbosity > 1) cout << "time: " << track.getTime() << endl ;

    if(rpDecId ==  23)
    {
      left_far_horizontal.validity = kTRUE ;
      left_far_horizontal.x = track.getX() ;
      left_far_horizontal.y = track.getY() ;
      left_far_horizontal.thx = track.getTx() ;
      left_far_horizontal.thy = track.getTy() ;
    }
    else if(rpDecId == 003)
    {
      left_near_horizontal.validity = kTRUE ;
      left_near_horizontal.x = track.getX() ;
      left_near_horizontal.y = track.getY() ;
      left_near_horizontal.thx = track.getTx() ;
      left_near_horizontal.thy = track.getTy() ;
    }
    else if(rpDecId == 103)
    {
      right_near_horizontal.validity = kTRUE ;
      right_near_horizontal.x = track.getX() ;
      right_near_horizontal.y = track.getY() ;
      right_near_horizontal.thx = track.getTx() ;
      right_near_horizontal.thy = track.getTy() ;
    }
    else if(rpDecId == 123)
    {
      right_far_horizontal.validity = kTRUE ;
      right_far_horizontal.x = track.getX() ;
      right_far_horizontal.y = track.getY() ;
      right_far_horizontal.thx = track.getTx() ;
      right_far_horizontal.thy = track.getTy() ;
    }
    else
    {
      if(verbosity > 1 && ((rpId.rp() == 2) || (rpId.rp() == 3))) cout << "Info: horizontal not in code" << rpDecId << endl ;
    }

    if(diagonal.compare("LBRT") == 0)
    {
      if(rpDecId ==  25)
      {
        rp_valid_025 = true;

        left_far.validity = kTRUE ;
        left_far.x = track.getX() ;
        left_far.y = track.getY() ;
        left_far.thx = track.getTx() ;
        left_far.thy = track.getTy() ;
      }
      else if(rpDecId ==   5)
      {
        rp_valid_005 = true;

        left_near.validity = kTRUE ;
        left_near.x = track.getX() ;
        left_near.y = track.getY() ;
        left_near.thx = track.getTx() ;
        left_near.thy = track.getTy() ;
      }
      else if(rpDecId == 104)
      {
        rp_valid_104 = true;

        right_near.validity = kTRUE ;
        right_near.x = track.getX() ;
        right_near.y = track.getY() ;
        right_near.thx = track.getTx() ;
        right_near.thy = track.getTy() ;
      }
      else if(rpDecId == 124)
      {
        rp_valid_124 = true;

        right_far.validity = kTRUE ;
        right_far.x = track.getX() ;
        right_far.y = track.getY() ;
        right_far.thx = track.getTx() ;
        right_far.thy = track.getTy() ;
      }
      else
      {
        if(verbosity > 1) cout << "Info: not in diagonal " << diagonal << " " << rpDecId << endl ;
      }
    }
    else if(diagonal.compare("LTRB") == 0)
    {
      if(rpDecId ==  24)
      {
        rp_valid_024 = true;

        left_far.validity = kTRUE ;
        left_far.x = track.getX() ;
        left_far.y = track.getY() ;
        left_far.thx = track.getTx() ;
        left_far.thy = track.getTy() ;
      }
      else if(rpDecId ==   4)
      {
        rp_valid_004 = true;

        left_near.validity = kTRUE ;
        left_near.x = track.getX() ;
        left_near.y = track.getY() ;
        left_near.thx = track.getTx() ;
        left_near.thy = track.getTy() ;
      }
      else if(rpDecId == 105)
      {
        rp_valid_105 = true;

        right_near.validity = kTRUE ;
        right_near.x = track.getX() ;
        right_near.y = track.getY() ;
        right_near.thx = track.getTx() ;
        right_near.thy = track.getTy() ;
      }
      else if(rpDecId == 125)
      {
        rp_valid_125 = true;

        right_far.validity = kTRUE ;
        right_far.x = track.getX() ;
        right_far.y = track.getY() ;
        right_far.thx = track.getTx() ;
        right_far.thy = track.getTy() ;
      }
      else
      {
        if(verbosity > 1)  cout << "Info: not in diagonal " << diagonal << " " << rpDecId << endl ;
      }
    }
    else
    {
      cout << "Unknown diagonal: " << diagonal << endl ;
      exit(1) ;
    }
  }
  
  if(rp_valid_025 && rp_valid_005 && rp_valid_104 && rp_valid_124)
  {
    if(verbosity > 1) cout << "left_bottom_right_top" << endl ;
    tree->Fill() ;  
  }

  if(rp_valid_024 && rp_valid_004 && rp_valid_105 && rp_valid_125)
  {
    if(verbosity > 1) cout << "left_top_right_bottom" << endl ;
    tree->Fill() ;  
  }

  map <int, int> map_RPids_occurance_in_event ;
  map<unsigned int, RP_struct_type> map_RPs ;

  for(const auto& track : iEvent.get(tracksToken_) )
  {
    CTPPSDetId rpId(track.getRPId());
    unsigned int rpDecId = (100*rpId.arm()) + (10*rpId.station()) + (1*rpId.rp());

    map_RPids_occurance_in_event[rpDecId]++;

    histosTH2F["scatter_plot_xy"]->Fill(track.getX(), track.getY()) ;

    stringstream ss ;
    ss << rpDecId ;

    histosTH2F[("scatter_plot_xy_" + ss.str()).c_str()]->Fill(track.getX(), track.getY()) ;

    RP_struct_type my_RP_struct ;

    my_RP_struct.validity = kTRUE ;
    my_RP_struct.rpDecId = rpDecId ;
    my_RP_struct.x = track.getX() ;
    my_RP_struct.y = track.getY() ;
    my_RP_struct.thx = track.getTx() ;
    my_RP_struct.thy = track.getTy() ;
    
    map_RPs[rpDecId] = my_RP_struct ;
    
  }

  for(map <int, int>::iterator it = map_RPids_occurance_in_event.begin() ; it != map_RPids_occurance_in_event.end() ; ++it)
  for(map <int, int>::iterator it2 = map_RPids_occurance_in_event.begin() ; it2 != map_RPids_occurance_in_event.end() ; ++it2)
  {
    histosTH2F["RP_correlation"]->Fill(it->first, it2->first) ;
  }

  bool simulation = false ;

  for(map<unsigned int, RP_struct_type>::iterator it = map_RPs.begin() ; it != map_RPs.end() ; ++it)
  for(map<unsigned int, RP_struct_type>::iterator it2 = it ; it2 != map_RPs.end() ; ++it2)
  {
    if(it == it2) continue ;

    unsigned int rpDecId1 = it->first ;
    unsigned int rpDecId2 = it2->first ;

    if(rpDecId1 > rpDecId2)
    {
      cout << "strange " << iEvent.id() << endl ;
      exit(1) ;
    }

    if((rpDecId1 == 4) && (rpDecId2 == 24))
    {
      histosTH2F["diff_x_vs_dx_24_4_vs_4"]->Fill(it->second.x, (it2->second.x - it->second.x)) ;
      histosTH2F["diff_x_vs_dx_24_4_vs_24"]->Fill(it2->second.x, (it2->second.x - it->second.x)) ;

      histosTH2F["diff_y_vs_dy_24_4_vs_4"]->Fill(it->second.y, (it2->second.y - it->second.y)) ;
      histosTH2F["diff_y_vs_dy_24_4_vs_24"]->Fill(it2->second.y, (it2->second.y - it->second.y)) ;
      
      double th_x = (it2->second.x - it->second.x) / RP_distance_mm ;
      double th_y = (it2->second.y - it->second.y) / RP_distance_mm ;

      histosTH2F["th_x_local_vs_RP"]->Fill(it->second.thx, th_x) ;
      histosTH2F["th_y_local_vs_RP"]->Fill(it->second.thy, th_y) ;
    }

    if((rpDecId1 == 5) && (rpDecId2 == 25))
    {
      histosTH2F["diff_x_vs_dx_25_5_vs_5"]->Fill(it->second.x, (it2->second.x - it->second.x)) ;
      histosTH2F["diff_x_vs_dx_25_5_vs_25"]->Fill(it2->second.x, (it2->second.x - it->second.x)) ;

      histosTH2F["diff_y_vs_dy_25_5_vs_5"]->Fill(it->second.y, (it2->second.y - it->second.y)) ;
      histosTH2F["diff_y_vs_dy_25_5_vs_25"]->Fill(it2->second.y, (it2->second.y - it->second.y)) ;
    }

    if((rpDecId1 == 104) && (rpDecId2 == 124))
    {
      histosTH2F["diff_x_vs_dx_124_104_vs_104"]->Fill(it->second.x, (it2->second.x - it->second.x)) ;
      histosTH2F["diff_x_vs_dx_124_104_vs_124"]->Fill(it2->second.x, (it2->second.x - it->second.x)) ;

      histosTH2F["diff_y_vs_dy_124_104_vs_104"]->Fill(it->second.y, (it2->second.y - it->second.y)) ;
      histosTH2F["diff_y_vs_dy_124_104_vs_124"]->Fill(it2->second.y, (it2->second.y - it->second.y)) ;
    }

    if((rpDecId1 == 105) && (rpDecId2 == 125))
    {
      histosTH2F["diff_x_vs_dx_125_105_vs_105"]->Fill(it->second.x, (it2->second.x - it->second.x)) ;
      histosTH2F["diff_x_vs_dx_125_105_vs_125"]->Fill(it2->second.x, (it2->second.x - it->second.x)) ;

      histosTH2F["diff_y_vs_dy_125_105_vs_105"]->Fill(it->second.y, (it2->second.y - it->second.y)) ;
      histosTH2F["diff_y_vs_dy_125_105_vs_125"]->Fill(it2->second.y, (it2->second.y - it->second.y)) ;
    }

    if(simulation)
    {
      RP_struct_type RP_hor, RP_ver ;

      RP_hor.validity = kTRUE ;
      RP_hor.rpDecId = 3 ;
      RP_hor.x = myrand.Uniform() ;
      RP_hor.y = myrand.Uniform() ;
      RP_hor.thx = myrand.Uniform() ;
      RP_hor.thy = myrand.Uniform() ;

      RP_ver.validity = kTRUE ;
      RP_ver.rpDecId = 4 ;
      RP_ver.x = myrand.Uniform() ;
      RP_ver.y = myrand.Uniform() ;
      RP_ver.thx = myrand.Uniform() ;
      RP_ver.thy = myrand.Uniform() ;
      
      map<unsigned int, RP_struct_type> mc_map ;
      mc_map[RP_hor.rpDecId] = RP_hor ;
      mc_map[RP_ver.rpDecId ] = RP_ver ;

      map<unsigned int, RP_struct_type>::iterator it3 = mc_map.begin() ;
      map<unsigned int, RP_struct_type>::iterator it4 = mc_map.begin() ;
      it4++ ;

      TestDetectorPair(it3, it4, 3, 4) ;
      
      cout << "Simulation" << endl ;
    }
    else
    {
      TestDetectorPair(it, it2, 3, 4) ;
      TestDetectorPair(it, it2, 3, 5) ;

      TestDetectorPair(it, it2, 23, 24) ;
      TestDetectorPair(it, it2, 23, 25) ;

      TestDetectorPair(it, it2, 103, 104) ;
      TestDetectorPair(it, it2, 103, 105) ;

      TestDetectorPair(it, it2, 123, 124) ;
      TestDetectorPair(it, it2, 123, 125) ;
    }
  }
}

void ElasticAnalyzer::addLabels(int first, int second)
{
  stringstream ss1, ss2 ;
  
  ss1 << first ;
  ss2 << second ;
  
  string name_x  = "dx_" + ss1.str() + "_" + ss2.str() ;
  string name_y  = "dy_" + ss1.str() + "_" + ss2.str() ;

  string name_xx = "x_{RP, " + ss2.str() + "} (mm)" ;
  string name_xy = "x_{RP, " + ss2.str() + "} - x_{RP, " + ss1.str() + "} (mm)" ;

  string name_yx = "y_{RP, " + ss2.str() + "} (mm)" ;
  string name_yy = "y_{RP, " + ss2.str() + "} - y_{RP, " + ss1.str() + "} (mm)" ;

  histosTH2F[name_x]->GetXaxis()->SetTitle(name_xx.c_str()) ;
  histosTH2F[name_x]->GetYaxis()->SetTitle(name_xy.c_str()) ;

  histosTH2F[name_y]->GetXaxis()->SetTitle(name_yx.c_str()) ;
  histosTH2F[name_y]->GetYaxis()->SetTitle(name_yy.c_str()) ;

}

void ElasticAnalyzer::addLabels()
{
  addLabels(3, 4) ;
  addLabels(3, 5) ;

  addLabels(23, 24) ;
  addLabels(23, 25) ;

  addLabels(103, 104) ;
  addLabels(103, 105) ;

  addLabels(123, 124) ;
  addLabels(123, 125) ;
}

void ElasticAnalyzer::addHistos()
{
  for(unsigned int i = 0 ; i < RP_numbers.size() ; ++i)
  {
    stringstream ss ;
    ss << RP_numbers[i] ;

    string name = "scatter_plot_xy_" + ss.str() ;

    histosTH2F[name] = new TH2F(name.c_str(), name.c_str(), 100, -40.0, 40.0, 100, -40.0, 40.0);
  }

  const int gap_between_horizontals_in_array = 3 ;

  for(unsigned int i = 0 ; i < RP_numbers.size() ; i += gap_between_horizontals_in_array)
  {
    for(unsigned int j = 0 ; j < 2 ; ++j)
    {
      unsigned int RP1 = RP_numbers[i] ;
      unsigned int RP2 = RP_numbers[i+j+1] ;
      
      stringstream ss1, ss2 ;

      ss1 << RP1 ;
      ss2 << RP2 ;

      string name1 = "xy_" + ss1.str() +"_if_" + ss1.str() + "_" + ss2.str() ;
      string name2 = "xy_" + ss2.str() +"_if_" + ss1.str() + "_" + ss2.str() ;

      histosTH2F[name1] = new TH2F(name1.c_str(), name1.c_str() , 100, -20.0, 20.0, 100, -20.0, 20.0);
      histosTH2F[name2] = new TH2F(name2.c_str(), name2.c_str() , 100, -20.0, 20.0, 100, -20.0, 20.0);

      string name3 = "dx_" + ss1.str() +"_" + ss2.str() ;
      string name4 = "dy_" + ss1.str() +"_" + ss2.str() ;

      string name5 = "h" + name3 ;
      string name6 = "h" + name4 ; 

      histosTH2F[name3] = new TH2F(name3.c_str(), name3.c_str() , 100, -20.0, 20.0, 100, -vertical_boundary_mm, vertical_boundary_mm);
      histosTH2F[name4] = new TH2F(name4.c_str(), name4.c_str() , 100, -20.0, 20.0, 100, -vertical_boundary_mm, vertical_boundary_mm);

      histosTH1F[name5] = new TH1F(name5.c_str(), name5.c_str() , 100, -vertical_boundary_mm, vertical_boundary_mm);
      histosTH1F[name6] = new TH1F(name6.c_str(), name6.c_str() , 100, -vertical_boundary_mm, vertical_boundary_mm);

    }
  }
}

void ElasticAnalyzer::beginJob()
{

  histosTH2F["RP_correlation"] = new TH2F("RP_correlation", "RP_correlation" , (125 - 3) + 1, 3, 125, (125 - 3) + 1, 3, 125);
  histosTH2F["RP_covariance"] = new TH2F("RP_covariance", "RP_covariance" , (125 - 3) + 1, 3, 125, (125 - 3) + 1, 3, 125);
  histosTH2F["scatter_plot_xy"] = new TH2F("scatter_plot_xy", "scatter_plot_xy" , 100, -40.0, 40.0, 100, -40.0, 40.0);

  histosTH2F["diff_x_vs_dx_24_4_vs_4"] = new TH2F("diff_x_vs_dx_24_4_vs_4", "diff_x_vs_dx_24_4_vs_4" , 100, -20.0, 20.0, 100, -20.0, 20.0);
  histosTH2F["diff_x_vs_dx_24_4_vs_24"] = new TH2F("diff_x_vs_dx_24_4_vs_24", "diff_x_vs_dx_24_4_vs_24" , 100, -20.0, 20.0, 100, -20.0, 20.0);

  histosTH2F["diff_x_vs_dx_25_5_vs_5"] = new TH2F("diff_x_vs_dx_25_5_vs_5", "diff_x_vs_dx_25_5_vs_5" , 100, -20.0, 20.0, 100, -20.0, 20.0);
  histosTH2F["diff_x_vs_dx_25_5_vs_25"] = new TH2F("diff_x_vs_dx_25_5_vs_25", "diff_x_vs_dx_25_5_vs_25" , 100, -20.0, 20.0, 100, -20.0, 20.0);

  histosTH2F["diff_x_vs_dx_124_104_vs_104"] = new TH2F("diff_x_vs_dx_124_104_vs_104", "diff_x_vs_dx_124_104_vs_104" , 100, -20.0, 20.0, 100, -20.0, 20.0);
  histosTH2F["diff_x_vs_dx_124_104_vs_124"] = new TH2F("diff_x_vs_dx_124_104_vs_124", "diff_x_vs_dx_124_104_vs_124" , 100, -20.0, 20.0, 100, -20.0, 20.0);

  histosTH2F["diff_x_vs_dx_125_105_vs_105"] = new TH2F("diff_x_vs_dx_125_105_vs_105", "diff_x_vs_dx_125_105_vs_105" , 100, -20.0, 20.0, 100, -20.0, 20.0);
  histosTH2F["diff_x_vs_dx_125_105_vs_125"] = new TH2F("diff_x_vs_dx_125_105_vs_125", "diff_x_vs_dx_125_105_vs_125" , 100, -20.0, 20.0, 100, -20.0, 20.0);

  histosTH2F["diff_y_vs_dy_24_4_vs_4"] = new TH2F("diff_y_vs_dy_24_4_vs_4", "diff_y_vs_dy_24_4_vs_4" , 100, -20.0, 20.0, 100, -2.0, 2.0);
  histosTH2F["diff_y_vs_dy_24_4_vs_24"] = new TH2F("diff_y_vs_dy_24_4_vs_24", "diff_y_vs_dy_24_4_vs_24" , 100, -20.0, 20.0, 100, -2.0, 2.0);

  histosTH2F["diff_y_vs_dy_25_5_vs_5"] = new TH2F("diff_y_vs_dy_25_5_vs_5", "diff_y_vs_dy_25_5_vs_5" , 100, -20.0, 20.0, 100, -2.0, 2.0);
  histosTH2F["diff_y_vs_dy_25_5_vs_25"] = new TH2F("diff_y_vs_dy_25_5_vs_25", "diff_y_vs_dy_25_5_vs_25" , 100, -20.0, 20.0, 100, -2.0, 2.0);

  histosTH2F["diff_y_vs_dy_124_104_vs_104"] = new TH2F("diff_y_vs_dy_124_104_vs_104", "diff_y_vs_dy_124_104_vs_104" , 100, -20.0, 20.0, 100, -2.0, 2.0);
  histosTH2F["diff_y_vs_dy_124_104_vs_124"] = new TH2F("diff_y_vs_dy_124_104_vs_124", "diff_y_vs_dy_124_104_vs_124" , 100, -20.0, 20.0, 100, -2.0, 2.0);

  histosTH2F["diff_y_vs_dy_125_105_vs_105"] = new TH2F("diff_y_vs_dy_125_105_vs_105", "diff_y_vs_dy_125_105_vs_105" , 100, -20.0, 20.0, 100, -2.0, 2.0);
  histosTH2F["diff_y_vs_dy_125_105_vs_125"] = new TH2F("diff_y_vs_dy_125_105_vs_125", "diff_y_vs_dy_125_105_vs_125" , 100, -20.0, 20.0, 100, -2.0, 2.0);

  histosTH2F["th_x_local_vs_RP"] = new TH2F("th_x_local_vs_RP", "th_x_local_vs_RP" , 100, -5.0e-4, 5.0e-4, 100, -5.0e-4, 5.0e-4);
  histosTH2F["th_y_local_vs_RP"] = new TH2F("th_y_local_vs_RP", "th_y_local_vs_RP" , 100, -5.0e-4, 5.0e-4, 100, -5.0e-4, 5.0e-4);

  addHistos() ;
  addLabels() ;
  
  tree = new TTree("TReducedNtuple", "TReducedNtuple") ;  

  tree->Branch("event_info_timestamp",                    &event_info_timestamp,                  "event_info_timestamp/l") ;
  tree->Branch("trigger_data_run_num",                    &trigger_data_run_num,                  "trigger_data_run_num/i") ;
  tree->Branch("trigger_data_bunch_num",                  &trigger_data_bunch_num,                "trigger_data_bunch_num/i") ;
  tree->Branch("trigger_data_event_num",                  &trigger_data_event_num,                "trigger_data_event_num/i") ;
  tree->Branch("trigger_data_trigger_num",                &trigger_data_trigger_num,              "trigger_data_trigger_num/i") ;
  tree->Branch("trigger_data_input_status_bits",          &trigger_data_input_status_bits,        "trigger_data_input_status_bits/i") ;

  tree->Branch("track_right_far_valid", &right_far.validity,  "track_right_far_valid/O") ;
  tree->Branch("track_right_far_x",     &right_far.x,         "track_right_far_x/D") ;
  tree->Branch("track_right_far_y",     &right_far.y,         "track_right_far_y/D") ;
  tree->Branch("track_right_far_thx",   &right_far.thx,        "track_right_far_thx/D") ;
  tree->Branch("track_right_far_thy",   &right_far.thy,        "track_right_far_thy/D") ;

  tree->Branch("track_left_far_valid",  &left_far.validity,   "track_left_far_valid/O") ;
  tree->Branch("track_left_far_x",      &left_far.x,          "track_left_far_x/D") ;
  tree->Branch("track_left_far_y",      &left_far.y,          "track_left_far_y/D") ;
  tree->Branch("track_left_far_thx",    &left_far.thx,        "track_left_far_thx/D") ;
  tree->Branch("track_left_far_thy",    &left_far.thy,        "track_left_far_thy/D") ;

  tree->Branch("track_right_near_valid",&right_near.validity, "track_right_near_valid/O") ;
  tree->Branch("track_right_near_x",    &right_near.x,        "track_right_near_x/D") ;
  tree->Branch("track_right_near_y",    &right_near.y,        "track_right_near_y/D") ;
  tree->Branch("track_right_near_thx",  &right_near.thx,      "track_right_near_thx/D") ;
  tree->Branch("track_right_near_thy",  &right_near.thy,      "track_right_near_thy/D") ;

  tree->Branch("track_left_near_valid", &left_near.validity,  "track_left_near_valid/O") ;
  tree->Branch("track_left_near_x",     &left_near.x,         "track_left_near_x/D") ;
  tree->Branch("track_left_near_y",     &left_near.y,         "track_left_near_y/D") ;
  tree->Branch("track_left_near_thx",   &left_near.thx,       "track_left_near_thx/D") ;
  tree->Branch("track_left_near_thy",   &left_near.thy,       "track_left_near_thy/D") ;

  tree->Branch("track_left_near_uPlanesOn",               &left_near.uPlanesOn,                   "track_left_near_uPlanesOn/i") ;
  tree->Branch("track_left_near_vPlanesOn",               &left_near.vPlanesOn,                   "track_left_near_vPlanesOn/i") ;
  tree->Branch("track_left_far_uPlanesOn",                &left_far.uPlanesOn,                    "track_left_far_uPlanesOn/i") ;
  tree->Branch("track_left_far_vPlanesOn",                &left_far.vPlanesOn,                    "track_left_far_vPlanesOn/i") ;
  tree->Branch("track_right_near_uPlanesOn",              &right_near.uPlanesOn,                  "track_right_near_uPlanesOn/i") ;
  tree->Branch("track_right_near_vPlanesOn",              &right_near.vPlanesOn,                  "track_right_near_vPlanesOn/i") ;
  tree->Branch("track_right_far_uPlanesOn",               &right_far.uPlanesOn,                   "track_right_far_uPlanesOn/i") ;
  tree->Branch("track_right_far_vPlanesOn",               &right_far.vPlanesOn,                   "track_right_far_vPlanesOn/i") ;
  tree->Branch("track_left_near_horizontal_valid",        &left_near_horizontal.validity,         "track_left_near_horizontal_valid/O") ;
  tree->Branch("track_left_near_horizontal_x",            &left_near_horizontal.x,                "track_left_near_horizontal_x/D") ;
  tree->Branch("track_left_near_horizontal_y",            &left_near_horizontal.y,                "track_left_near_horizontal_y/D") ;
  tree->Branch("track_left_near_horizontal_thx",          &left_near_horizontal.thx,              "track_left_near_horizontal_thx/D") ;
  tree->Branch("track_left_near_horizontal_thy",          &left_near_horizontal.thy,              "track_left_near_horizontal_thy/D") ;
  tree->Branch("track_left_far_horizontal_valid",         &left_far_horizontal.validity,          "track_left_far_horizontal_valid/O") ;
  tree->Branch("track_left_far_horizontal_x",             &left_far_horizontal.x,                 "track_left_far_horizontal_x/D") ;
  tree->Branch("track_left_far_horizontal_y",             &left_far_horizontal.y,                 "track_left_far_horizontal_y/D") ;
  tree->Branch("track_left_far_horizontal_thx",           &left_far_horizontal.thx,               "track_left_far_horizontal_thx/D") ;
  tree->Branch("track_left_far_horizontal_thy",           &left_far_horizontal.thy,               "track_left_far_horizontal_thy/D") ;
  tree->Branch("track_right_near_horizontal_valid",       &right_near_horizontal.validity,        "track_right_near_horizontal_valid/O") ;
  tree->Branch("track_right_near_horizontal_x",           &right_near_horizontal.x,               "track_right_near_horizontal_x/D") ;
  tree->Branch("track_right_near_horizontal_y",           &right_near_horizontal.y,               "track_right_near_horizontal_y/D") ;
  tree->Branch("track_right_near_horizontal_thx",         &right_near_horizontal.thx,             "track_right_near_horizontal_thx/D") ;
  tree->Branch("track_right_near_horizontal_thy",         &right_near_horizontal.thy,             "track_right_near_horizontal_thy/D") ;
  tree->Branch("track_right_far_horizontal_valid",        &right_far_horizontal.validity,         "track_right_far_horizontal_valid/O") ;
  tree->Branch("track_right_far_horizontal_x",            &right_far_horizontal.x,                "track_right_far_horizontal_x/D") ;
  tree->Branch("track_right_far_horizontal_y",            &right_far_horizontal.y,                "track_right_far_horizontal_y/D") ;
  tree->Branch("track_right_far_horizontal_thx",          &right_far_horizontal.thx,              "track_right_far_horizontal_thx/D") ;
  tree->Branch("track_right_far_horizontal_thy",          &right_far_horizontal.thy,              "track_right_far_horizontal_thy/D") ;

}

map<string, TH1F *> map_of_hists ;

void ElasticAnalyzer::endJob()
{
  TFile * f_out = TFile::Open(outputFileName.c_str(), "RECREATE");

  for (const auto &p : histosTH1F)
  {
    p.second->Write(p.first.c_str());
    cout << "  TH1_names.push_back(\"" << p.first.c_str() << "\") ;" << endl ;
  }
  
  cout << endl ;
  cout << endl ;

  for (const auto &p : histosTH2F)
  {
    p.second->Write(p.first.c_str());
    cout << "  filenames.push_back(\"" << p.first.c_str() << "\") ;" << endl ;
  }

  tree->Write() ;

  Minimize() ;

  for(map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>>:: iterator it = map_of_THorizontal_and_vertical_xy_pairs_to_match.begin() ; it != map_of_THorizontal_and_vertical_xy_pairs_to_match.end() ; ++it)
  {
    map_of_hists[it->first]->Write() ;
  }

  delete f_out;
  
}

vector<THorizontal_and_vertical_xy_pairs_to_match *> *points = NULL ;

string actual_detector_combination = "" ;

TH2F *test_hist_ver = new TH2F("test_hist_ver", "test_hist_ver" , 100, -20.0, 20.0, 100, -20.0, 20.0);
TH2F *test_hist_hor = new TH2F("test_hist_hor", "test_hist_hor" , 100, -20.0, 20.0, 100, -20.0, 20.0);

TH1F *chi2_contribution_hist = new TH1F("chi2_contribution_hist", "chi2_contribution_hist", 1000, 0, 100) ;


void fcn(Int_t &npar, double *gin, double &f, double *par, int iflag)
{
  double chi2 = 0 ;

  double a = par[0] ;
  double b = par[1] ;
  double alpha = par[2] ;

  const double ex = 60e-3 ;
  const double ey = 60e-3 ;

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


TF1 *func = NULL ;
TF1 *func2 = NULL ;


void MinuitFit()
{
  TMinuit *gMinuit2 = new TMinuit(10);
  gMinuit2->SetFCN(fcn);

  Double_t arglist[10];
  Int_t ierflg = 0 ;
  arglist[0] = 1 ;
  gMinuit2->mnexcm("SET ERR", arglist ,1,ierflg);

  gMinuit2->mnparm(0, "a", 0, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(1, "b", 0, 0.1, 0, 0, ierflg);
  gMinuit2->mnparm(2, "alpha", 0.0, 0.1, 0, 0, ierflg);

  arglist[0] = 3 ;

  // gMinuit2->mnexcm("FIX", arglist, 1, ierflg);

  arglist[0] = 0 ;
  arglist[1] = 3 ;
  arglist[2] = 1 ;

  gMinuit2->mnexcm("MIGRAD", arglist , 2, ierflg);
  
  cout << "number_of_points: " << (*points).size() << endl ;

  double par[4] ;
  double pare[4] ;

  {
	  gMinuit2->GetParameter(0, par[0], pare[0]) ;
    gMinuit2->GetParameter(1, par[1], pare[1]) ;
	  // gMinuit2->GetParameter(2, par[2], pare[2]) ;

  }
}


void ElasticAnalyzer::MinimizeHorizontalVerticalPair(vector<THorizontal_and_vertical_xy_pairs_to_match *> &vector_of_corrd)
{
  points = &vector_of_corrd ;

  MinuitFit() ;
}

void ElasticAnalyzer::Minimize()
{
  for(map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>>:: iterator it = map_of_THorizontal_and_vertical_xy_pairs_to_match.begin() ; it != map_of_THorizontal_and_vertical_xy_pairs_to_match.end() ; ++it)
  {
    cout << endl << endl << "################" << it->first << " " << it->second.size() << endl ;

    actual_detector_combination = it->first ;
    string name_of_hist = "chi2_contribution_" + actual_detector_combination ;
    map_of_hists[actual_detector_combination] = new TH1F(name_of_hist.c_str(), name_of_hist.c_str(), 1000, 0, 100) ;

    MinimizeHorizontalVerticalPair(it->second) ;
    
    test_hist_ver->SaveAs("test_hist_ver.root") ;
    test_hist_hor->SaveAs("test_hist_hor.root") ;

  }
  
}

void ElasticAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ElasticAnalyzer);
