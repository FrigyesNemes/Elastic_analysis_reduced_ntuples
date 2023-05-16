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

#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"

using namespace std;

struct RP_struct_type
{
	Bool_t		validity ;

	Double_t	x ;
	Double_t	y ;

	Double_t	thx ;
	Double_t	thy ;

	UInt_t		uPlanesOn ;
	UInt_t		vPlanesOn ;
} ;

class ElasticAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
  public:
  explicit ElasticAnalyzer(const edm::ParameterSet&);
  ~ElasticAnalyzer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:

  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;

  edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite>> tracksToken_;  //used to select what tracks to read from configuration file

  std::string outputFileName;  

  map<string, TH2F*> histosTH2F;

  RP_struct_type right_far ;
  RP_struct_type left_far ;

  RP_struct_type right_near ;
  RP_struct_type left_near ;

  TTree *tree ;
};

ElasticAnalyzer::ElasticAnalyzer(const edm::ParameterSet& iConfig) : tracksToken_(consumes<std::vector<CTPPSLocalTrackLite>>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
  outputFileName(iConfig.getParameter<std::string>("outputFileName"))
{
}


ElasticAnalyzer::~ElasticAnalyzer()
{
}

void ElasticAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  for(const auto& track : iEvent.get(tracksToken_) )
  {
    CTPPSDetId rpId(track.getRPId());
    unsigned int rpDecId = (100*rpId.arm()) + (10*rpId.station()) + (1*rpId.rp());

    if(rpDecId == 124)
    {
      right_far.x = track.getX() ;
      right_far.y = track.getY() ;
    }
    
    histosTH2F["scatter_plot_xy"]->Fill(right_far.x, right_far.y) ;
    
    tree->Fill() ;

    cout << rpDecId << " " << right_far.x << " " << right_far.y << endl ;
  }
}


void ElasticAnalyzer::beginJob()
{
  histosTH2F["scatter_plot_xy"] = new TH2F("scatter_plot_xy", "scatter_plot_xy" , 100, -40.0, 40.0, 100, -40.0, 40.0);
  
  tree = new TTree("TReducedNtuple", "TReducedNtuple") ;  

  tree->Branch("track_right_far_valid", &right_far.validity,  "track_right_far_valid/O") ;
  tree->Branch("track_right_far_x",     &right_far.x,         "track_right_far_x/D") ;
  tree->Branch("track_right_far_y",     &right_far.y,         "track_right_far_y/D") ;

  tree->Branch("track_left_far_valid",  &left_far.validity,   "track_left_far_valid/O") ;
  tree->Branch("track_left_far_x",      &left_far.x,          "track_left_far_x/D") ;
  tree->Branch("track_left_far_y",      &left_far.y,          "track_left_far_y/D") ;

  tree->Branch("track_right_near_valid",&right_near.validity, "track_right_near_valid/O") ;
  tree->Branch("track_right_near_x",    &right_near.x,        "track_right_near_x/D") ;
  tree->Branch("track_right_near_y",    &right_near.y,        "track_right_near_y/D") ;

  tree->Branch("track_left_near_valid", &left_near.validity,  "track_left_near_valid/O") ;
  tree->Branch("track_left_near_x",     &left_near.x,         "track_left_near_x/D") ;
  tree->Branch("track_left_near_y",     &left_near.y,         "track_left_near_y/D") ;

}

void ElasticAnalyzer::endJob()
{
  TFile * f_out = TFile::Open(outputFileName.c_str(), "RECREATE");

  // for (const auto &p : histosTH2F) p.second->Write(p.first.c_str());

  tree->Write() ;

  delete f_out;

}

void ElasticAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {

  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(ElasticAnalyzer);
