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

#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
 
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "ElasticAnalysis/ElasticAnalyzer/interface/ElasticAnalyzer.h"

using namespace std;

vector<int> RP_numbers = {3, 4, 5, 23, 24, 25, 103, 104, 105, 123, 124, 125} ;

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
  virtual void TestDetectorPair(map<unsigned int, RP_struct_type>::iterator, map<unsigned int, RP_struct_type>::iterator, unsigned int, unsigned int, double=0, double=0, double sigma=0) ;
  virtual void TestDetectorPair2(map<unsigned int, RP_struct_type>::iterator, map<unsigned int, RP_struct_type>::iterator, unsigned int, unsigned int, double=0, double=0, double sigma=0) ;
  virtual void Minimize() ;
  void MinimizeHorizontalVerticalPair(vector<THorizontal_and_vertical_xy_pairs_to_match *> &);

  int verbosity;

  edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> tracksToken_;  //used to select what tracks to read from configuration file
  edm::EDGetTokenT<edm::DetSetVector<TotemRPUVPattern>> rpPatternToken_ ;

  std::string diagonal;  
  std::string outputFileName;  

  map<string, TH1F*> histosTH1F;
  map<string, TH2F*> histosTH2F;
  map<string, TProfile*> profiles;
  
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

  struct Distribution {
    enum Type { dtBox, dtGauss, dtGaussLimit } type_;
    double x_mean_, x_width_, x_min_, x_max_;
    double y_mean_, y_width_, y_min_, y_max_;

    Distribution(const edm::ParameterSet &);
  };
  
  Distribution *position_dist_;

  TTree *tree ;
};

ElasticAnalyzer::Distribution::Distribution(const edm::ParameterSet &ps) {
  // get type
  string typeName = ps.getParameter<string>("type");
  if (!typeName.compare("box"))
    type_ = dtBox;
  else if (!typeName.compare("gauss"))
    type_ = dtGauss;
  else if (!typeName.compare("gauss-limit"))
    type_ = dtGaussLimit;
  else
    throw cms::Exception("PPS") << "Unknown distribution type `" << typeName << "'.";

  x_mean_ = ps.getParameter<double>("x_mean");
  x_width_ = ps.getParameter<double>("x_width");
  x_min_ = ps.getParameter<double>("x_min");
  x_max_ = ps.getParameter<double>("x_max");

  y_mean_ = ps.getParameter<double>("y_mean");
  y_width_ = ps.getParameter<double>("y_width");
  y_min_ = ps.getParameter<double>("y_min");
  y_max_ = ps.getParameter<double>("y_max");
}


ElasticAnalyzer::ElasticAnalyzer(const edm::ParameterSet& iConfig) :  verbosity(iConfig.getUntrackedParameter<int>("verbosity")),
  tracksToken_(consumes<std::vector<CTPPSLocalTrackLite>>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
  rpPatternToken_(consumes<edm::DetSetVector<TotemRPUVPattern>>(iConfig.getParameter<edm::InputTag>("rpPatternTag"))),
  diagonal(iConfig.getParameter<std::string>("diagonal")), outputFileName(iConfig.getParameter<std::string>("outputFileName")) 
{
  position_dist_ = new Distribution(iConfig.getParameterSet("position_distribution")) ;
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

void ElasticAnalyzer::TestDetectorPair(map<unsigned int, RP_struct_type>::iterator it1, map<unsigned int, RP_struct_type>::iterator it2, unsigned int detector_1, unsigned int detector_2, double p0, double p1, double sigma)
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
      string name_x3 = "x_vs_dx_" + ss_1.str() + "_if_" + ss_1.str() + "_" + ss_2.str() ;
      string name_x4 = "y_vs_dy_" + ss_1.str() + "_if_" + ss_1.str() + "_" + ss_2.str() ;

      // cout << name_x3 << " " << name_y3 << endl ;

      histosTH2F[name_x2]->Fill(it1->second.x, it1->second.y) ;
      histosTH2F[name_y2]->Fill(it2->second.x, it2->second.y) ;

      double delta_x = (it2->second.x - it1->second.x) ;
      double y_extrapolated = (p1 * it1->second.x) + p0 ;

      double delta_from_cut = (y_extrapolated - delta_x) ;

      if(((p0 == 0) && (p1 == 0)) || (fabs(delta_from_cut / sigma) < 3.0))
      {
        histosTH2F[name_x3]->Fill(it1->second.x, it2->second.x - it1->second.x) ;
        profiles["profile_" + name_x3]->Fill(it1->second.x, it2->second.x - it1->second.x) ;
      }

      double sigma_2 = 0.025 ;
      double delta_x_2 = (it2->second.y - it1->second.y) ;
      double y_extrapolated_2 = (0.00358 * it1->second.y) + 0.0 ;
      double delta_from_cut_2 = (y_extrapolated_2 - delta_x_2) ;

      if(fabs(delta_from_cut_2 / sigma_2) < 3.0)
      {
        histosTH2F[name_x4]->Fill(it1->second.y, it2->second.y - it1->second.y) ;
        profiles["profile_" + name_x4]->Fill(it1->second.y, it2->second.y - it1->second.y) ;
      }
    }
  }
}

void ElasticAnalyzer::TestDetectorPair2(map<unsigned int, RP_struct_type>::iterator it1, map<unsigned int, RP_struct_type>::iterator it2, unsigned int detector_1, unsigned int detector_2, double p0, double p1, double sigma)
{
  if((it1->first == detector_1) && (it2->first == detector_2))
  {
    // cout << "Found " << detector_1 << " " << detector_2 << endl ;
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

  for (const auto &pv : iEvent.get(rpPatternToken_))
  {
    const CTPPSDetId detId(pv.detId());
    const unsigned int rpDecId = detId.arm() * 100 + detId.station() * 10 + detId.rp();

    // cout << "Now I am using patterns" << endl ;

    // require exactly 1 U and 1 V pattern, i.e. unique U-V association
    unsigned int n_U = 0, n_V = 0;
    unsigned int idx_U = 0, idx_V = 0;
    for (unsigned int pi = 0; pi < pv.size(); pi++)
    {
      const TotemRPUVPattern &pattern = pv[pi];

      switch (pattern.getProjection()) {
        case TotemRPUVPattern::projU:
          n_U++;
          idx_U = pi;
          break;

        case TotemRPUVPattern::projV:
          n_V++;
          idx_V = pi;
          break;

        default:
          break;
      }
    }

    if (n_U != 1 || n_V != 1)
      continue;

    // skip if patterns not reasonable
    if (!pv[idx_U].getFittable() || !pv[idx_V].getFittable())
      continue;

    int counter_u_v = 0 ;
    int counter = 0 ;
    // cout << endl ;
    
    string strip_orientation = "U-strip JUU" ;

    for (const auto &pattern : {pv[idx_U], pv[idx_V]})
    {

      // cout << endl ;

      for (const auto &hitsDetSet : pattern.getHits())
      {
        for (auto &hit : hitsDetSet)
        {

          // cout << strip_orientation << " position" << counter << " " << hit.getPosition() << endl ;
          counter++ ;
        }
      }
      strip_orientation = "V-strip VII" ;
    }

  }

  for(const auto& track : iEvent.get(tracksToken_))
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
      TestDetectorPair(it, it2, 3, 4, -0.0120793 ,-0.0087, 0.013) ;
      TestDetectorPair(it, it2, 3, 5,  0.0368000 ,-0.0130, 0.022) ;

      TestDetectorPair(it, it2, 23, 24,  0.037, -0.014, 0.022) ;
      TestDetectorPair(it, it2, 23, 25, -0.006, -0.01,  0.022) ;

      TestDetectorPair(it, it2, 103, 104, 0.05 ,-0.0079, 0.022) ;
      TestDetectorPair(it, it2, 103, 105, 0.05 ,-0.0079, 0.022) ;

      TestDetectorPair(it, it2, 123, 124, 0.05 ,-0.0079, 0.022) ;
      TestDetectorPair(it, it2, 123, 125, 0.05 ,-0.0079, 0.022) ;

      TestDetectorPair2(it, it2, 3, 23, 0.0 ,0.0, 0.0) ;
      TestDetectorPair2(it, it2, 4, 24, 0.0 ,0.0, 0.0) ;
      TestDetectorPair2(it, it2, 5, 25, 0.0 ,0.0, 0.0) ;
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
      string name7 = "x_vs_dx_" + ss1.str() + "_if_" + ss1.str() + "_" + ss2.str() ;
      string name8 = "y_vs_dy_" + ss1.str() + "_if_" + ss1.str() + "_" + ss2.str() ;

      histosTH2F[name1] = new TH2F(name1.c_str(), name1.c_str() , 100, -20.0, 20.0, 100, -20.0, 20.0);
      histosTH2F[name2] = new TH2F(name2.c_str(), name2.c_str() , 100, -20.0, 20.0, 100, -20.0, 20.0);
      histosTH2F[name7] = new TH2F(name7.c_str(), name7.c_str() , 100, -20.0, 20.0, 100, -0.2,  0.2);
      histosTH2F[name8] = new TH2F(name8.c_str(), name8.c_str() , 100, -20.0, 20.0, 100, -0.2,  0.2);

      histosTH2F[name7]->GetXaxis()->SetTitle("x (mm)") ;
      histosTH2F[name7]->GetYaxis()->SetTitle("#Deltax (mm)") ;

      histosTH2F[name8]->GetXaxis()->SetTitle("y (mm)") ;
      histosTH2F[name8]->GetYaxis()->SetTitle("#Deltay (mm)") ;

      profiles["profile_" + name7] = new TProfile(("profile_" + name7).c_str(), ("profile_" + name7).c_str() , 100, -20.0, 20.0);
      profiles["profile_" + name8] = new TProfile(("profile_" + name8).c_str(), ("profile_" + name8).c_str() , 100, -20.0, 20.0);

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

  for (const auto &p : profiles)
  {
    int lowest_bin = 0 ;
    int highest_bin = p.second->GetXaxis()->GetNbins() ;

    double min_position = p.second->GetBinCenter(0) ;
    double max_position = p.second->GetBinCenter(highest_bin) ;

    for(int i = 0 ; i < p.second->GetXaxis()->GetNbins() ; ++i)
    {
      ++lowest_bin ;

      if(p.second->GetBinError(i) > 0) break ;
    }

    for(int i = p.second->GetXaxis()->GetNbins() ; i > 0 ;  --i)
    {
      --highest_bin ;

      if(p.second->GetBinError(i) > 0) break ;
    }

    double lowest_bin_position = p.second->GetBinCenter(lowest_bin) ;
    double highest_bin_position = p.second->GetBinCenter(highest_bin) ;

    cout << "fit_limits " << lowest_bin_position << " " << highest_bin_position << endl ;
    if(lowest_bin_position > highest_bin_position) cout << "hello" << endl ;

    double length = (highest_bin_position - lowest_bin_position) ;
    double edge = length / 10.0 ;
    highest_bin_position -= 2.0 * edge ;
    lowest_bin_position += 2.0 * edge ;

    TFitResultPtr myfit = p.second->Fit("pol1", "S", "", lowest_bin_position, highest_bin_position) ;

    if(myfit->Prob() < 2e-2)
    {
      highest_bin_position -= edge ;
      lowest_bin_position += edge ;

      TFitResultPtr myfit = p.second->Fit("pol1", "S", "", lowest_bin_position, highest_bin_position) ;
    }

    p.second->SetMarkerStyle(20) ;
    p.second->GetFunction("pol1")->SetLineStyle(kDashed) ;
    
    map<string, Color_t> color_map ;
    
    color_map["profile_x_vs_dx_3_if_3_4"] = kRed ;
    color_map["profile_y_vs_dy_3_if_3_4"] = kRed ;
    color_map["profile_x_vs_dx_3_if_3_5"] = kBlue;
    color_map["profile_y_vs_dy_3_if_3_5"] = kBlue;
    color_map["profile_x_vs_dx_23_if_23_24"] = kGreen ;
    color_map["profile_y_vs_dy_23_if_23_24"] = kGreen ;
    color_map["profile_x_vs_dx_23_if_23_25"] = kMagenta ;
    color_map["profile_y_vs_dy_23_if_23_25"] = kMagenta ;

    color_map["profile_x_vs_dx_103_if_103_104"] = kRed ;
    color_map["profile_y_vs_dy_103_if_103_104"] = kRed ;
    color_map["profile_x_vs_dx_103_if_103_105"] = kBlue;
    color_map["profile_y_vs_dy_103_if_103_105"] = kBlue;
    color_map["profile_x_vs_dx_123_if_123_124"] = kGreen ;
    color_map["profile_y_vs_dy_123_if_123_124"] = kGreen ;
    color_map["profile_x_vs_dx_123_if_123_125"] = kMagenta ;
    color_map["profile_y_vs_dy_123_if_123_125"] = kMagenta ;
    
    p.second->SetMarkerColor(color_map[p.first]) ;
    p.second->GetFunction("pol1")->SetLineColor(color_map[p.first]) ;

    cout << "slopes " << p.first << " " << myfit->Parameter(1) << " +/- " << myfit->ParError(1) << endl ;

    // p.second->GetFunction("pol1")->SetRange(min_position, max_position) ;

    p.second->Write(p.first.c_str());
  }

  tree->Write() ;

  Minimize() ;

  for(map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>>:: iterator it = map_of_THorizontal_and_vertical_xy_pairs_to_match.begin() ; it != map_of_THorizontal_and_vertical_xy_pairs_to_match.end() ; ++it)
  {
    map_of_hists[it->first]->Write() ;
  }

  delete f_out;
  
}

TF1 *func = NULL ;
TF1 *func2 = NULL ;


void ElasticAnalyzer::MinimizeHorizontalVerticalPair(vector<THorizontal_and_vertical_xy_pairs_to_match *> &vector_of_corrd)
{
  points = &vector_of_corrd ;

  MinuitFit("") ;
}

void ElasticAnalyzer::Minimize()
{
  for(map<string, vector<THorizontal_and_vertical_xy_pairs_to_match *>>:: iterator it = map_of_THorizontal_and_vertical_xy_pairs_to_match.begin() ; it != map_of_THorizontal_and_vertical_xy_pairs_to_match.end() ; ++it)
  {
    cout << endl << endl << "################" << it->first << " " << it->second.size() << endl ;

    actual_detector_combination = it->first ;
    string name_of_hist = "chi2_contribution_" + actual_detector_combination ;
    map_of_hists[actual_detector_combination] = new TH1F(name_of_hist.c_str(), name_of_hist.c_str(), 10000, 0, 100) ;

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
