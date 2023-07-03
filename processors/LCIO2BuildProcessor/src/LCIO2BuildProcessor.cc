#include "LCIO2BuildProcessor.hh"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <map>

// ---- LCIO Headers
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/MCParticle.h>
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/SimCalorimeterHitImpl.h"
#include <IMPL/LCFlagImpl.h>
#include "UTIL/LCTypedVector.h"
#include "UTIL/BitField64.h"

// ----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

#ifdef MARLIN_USE_AIDA
#include <marlin/AIDAProcessor.h>
#include <AIDA/IHistogramFactory.h>
#include <AIDA/ICloud1D.h>
#endif // MARLIN_USE_AIDA

//ROOT
#include <TH1.h>
#include <TH2.h>
#include "TF1.h"
#include <TString.h>
#include <TFile.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TSpectrum.h>
#include "TMath.h"
#include "TROOT.h"
#include <TTree.h>
#include <TInterpreter.h>


#include <fstream>
#include <sstream>

//  //From Adrian
#include <vector>
#include <algorithm>


using namespace lcio ;
using namespace marlin ;
using namespace std;

namespace CALICE
{

  LCIO2BuildProcessor aLCIO2BuildProcessor;

  LCIO2BuildProcessor::LCIO2BuildProcessor() : Processor("LCIO2BuildProcessor")
  {

    vector<string> calorimInpCollectionsExample;
    registerProcessorParameter("Input_Collections",
                               "vector of collections for input",
                               _calorimInpCollections,
                               calorimInpCollectionsExample);


    string outFileNameExample;
    registerProcessorParameter("OutputBuildFile",
                               "Name to identify output file",
                               _outFileName,
                               outFileNameExample);
    
    vector<string> siThicknessesExample;
    registerProcessorParameter("SiThicknesses",
                               "vector of Silicon thicknesses per layer",
                               _siThicknesses,
                               siThicknessesExample);
    
    vector<string> FixedPosZExample;
    registerProcessorParameter("FixedPosZ",
                               "vector of z layer positions",
                               _FixedPosZ,
                               FixedPosZExample);

    float Z0Example = 7.26; // For TB2022-03_CONF2
    registerProcessorParameter("Z0",
                               "Position of first slab",
                               _Z0,
                               Z0Example);

    vector<string> GeV2MIPExample;
    registerProcessorParameter("GeV2MIP",
                               "vector of conversion factors",
                               _GeV2MIP,
                               GeV2MIPExample);

    bool ConversionGeV2MIPExample;
    registerProcessorParameter("ConversionGeV2MIP",
                               "Do conversion to MIP",
                               _ConversionGeV2MIP,
                               ConversionGeV2MIPExample);
    
    vector<string> MapFilenamesExample = {"/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/processors/LCIO2BuildProcessor/mapping/fev10_chip_channel_x_y_mapping.txt", "/home/llr/ilc/jimenez/Projects/Simulations/SiWECAL-Sim/processors/LCIO2BuildProcessor/mapping/fev11_cob_rotate_chip_channel_x_y_mapping.txt"};
    registerProcessorParameter("MappingFiles",
                               "Files mapping hit position with cell chan and chip",
                               _MapFilenames,
                               MapFilenamesExample);
    
    vector<int> SlabMapIndicesExample = {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1};
    registerProcessorParameter("SlabMapIndices",
                               "vector indices of maps (from MappingFiles parameters) to be used per slab",
                               _SlabMapIndices,
                               SlabMapIndicesExample);    


    float HalfCenterGapExample = 1.05; // This dead space should be included in the generation (3.8 - 5.5/2)
    registerProcessorParameter("HalfCenterGap",
                               "Half size of gap between wafers",
                               _HalfCenterGap,
                               HalfCenterGapExample);
    
    // registerProcessorParameter("hitType",
    //                            "Hit type (SimCalorimeterHit or CalorimeterHit)",
    //                            _hitType,
    //                            "SimCalorimeterHit");
    
  }

  /************************************************************************************/

  void LCIO2BuildProcessor::init()
  {
    streamlog_out(DEBUG) << "init called" << std::endl ;

    printParameters();

    _nRun = 0 ;
    _nEvt = 0 ;
    _rootout = new TFile(_outFileName.c_str(), "RECREATE");
    _treeout = new TTree("ecal", "From SLCIO");
    _treeout->Branch("event", &event);
    _treeout->Branch("spill", &spill);
    _treeout->Branch("cycle", &cycle);
    _treeout->Branch("bcid", &bcid);
    _treeout->Branch("bcid_first_sca_full", &bcid_first_sca_full);
    _treeout->Branch("bcid_merge_end", &bcid_merge_end);
    _treeout->Branch("id_run", &id_run);
    _treeout->Branch("id_dat", &id_dat);
    _treeout->Branch("nhit_slab", &nhit_slab);
    _treeout->Branch("nhit_chip", &nhit_chip);
    _treeout->Branch("nhit_chan", &nhit_chan);
    _treeout->Branch("nhit_len", &nhit_len);

    // _treeout->Branch("sum_hg", &sum_hg);
    _treeout->Branch("sum_energy", &sum_energy);
    _treeout->Branch("sum_energy_lg", &sum_energy_lg);

    _treeout->Branch("hit_slab", &hit_slab);
    _treeout->Branch("hit_chip", &hit_chip);
    _treeout->Branch("hit_chan", &hit_chan);
    _treeout->Branch("hit_sca", &hit_sca);
    _treeout->Branch("hit_adc_high", &hit_adc_high);
    _treeout->Branch("hit_adc_low", &hit_adc_low);
    _treeout->Branch("hit_n_scas_filled", &hit_n_scas_filled);
    _treeout->Branch("hit_isHit", &hit_isHit);
    _treeout->Branch("hit_isMasked", &hit_isMasked);
    _treeout->Branch("hit_isCommissioned", &hit_isCommissioned);

    _treeout->Branch("hit_energy", &hit_energy);
    _treeout->Branch("hit_energy_w", &hit_energy_w);
    _treeout->Branch("hit_energy_lg", &hit_energy_lg);
    _treeout->Branch("hit_x", &hit_x);
    _treeout->Branch("hit_y", &hit_y);
    _treeout->Branch("hit_z", &hit_z);

    for (int ilayer = 0; ilayer < _FixedPosZ.size(); ilayer++)
      _FixedPosZ_float.push_back(stof(_FixedPosZ[ilayer]));

    for (int ilayer = 0; ilayer < _GeV2MIP.size(); ilayer++)
      _GeV2MIP_float.push_back(stof(_GeV2MIP[ilayer]));

    // _FixedPosZ = {6.225,  21.225,  36.15,  51.15,  66.06,  81.06,  96.06,\
    //               111.15, 126.15, 141.15, 156.15, 171.06, 186.06, 201.06,\
    //               216.06};
    // ILD mode
    // _FixedPosZ = {6.225,  21.225,  36.15,  51.15,  66.06,  81.06,  96.06,\
    //               111.15, 126.15, 141.15, 156.15, 171.06, 186.06, 201.06,\
    //               216.06, 216., 231., 246., 261., 276., 291., 306., 321., 336.};
    _slabSpacing = 15.;
    _deltaZ = 2.;
    _printType = true;

    // Mapping (should be done with nice dict...)
    // vector<vector<vector<float>>> _maps(_MapFilenames.size(), vector<vector<float>>(1024, vector<float>(2)));
    // Initializing to zeros, needs to be done in a smarter way
    vector<float> zeros = {0., 0.};
    vector<vector<float>> this_map;
    for (int imap = 0; imap < _MapFilenames.size(); imap++) {
      for (int icell = 0; icell < 1024; icell++) this_map.push_back(zeros);
      _maps.push_back(this_map); 
    }
    fstream map_file;
    string space_delimiter = " ";
    size_t pos;
    vector<string> words;

    for (int imap = 0; imap < _MapFilenames.size(); imap++) {
      map_file.open(_MapFilenames[imap], ios::in);
      vector<vector<float>> this_map;
      if (map_file.is_open()){   
        string line;
        getline(map_file, line); //skip the first line
        int iline = 0;
        while(getline(map_file, line)){ 
          auto start = 0U;
          auto end = line.find(space_delimiter);
          while (end != std::string::npos) {
              words.push_back(line.substr(start, end - start));
              start = end + space_delimiter.length();
              end = line.find(space_delimiter, start);
          }
          words.push_back(line.substr(start));
          // There's a sign flip in coordinates in mapping (hence the -1*)
          _maps[imap][64 * stoi(words[0]) + stoi(words[3])][0] = -1 * stof(words[4]);
          _maps[imap][64 * stoi(words[0]) + stoi(words[3])][1] = -1 * stof(words[5]);
          words.clear();
        }

        map_file.close();
      }
    }
  }


  /************************************************************************************/
  void LCIO2BuildProcessor::processRunHeader( LCRunHeader* run)
  {
    _nRun++ ;
  }

  void LCIO2BuildProcessor::processEvent( LCEvent * evt )
  {
    
    int evtNumber = evt->getEventNumber();
    if ((evtNumber % 1000) == 0)
    streamlog_out(DEBUG) << " \n ---------> Event: "<<evtNumber<<"!Collections!! <-------------\n" << std::endl;

    //Get the list of collections in the event
    const std::vector<std::string> *cnames = evt->getCollectionNames();
    
    std::vector<float> this_energyCont;
    // int this_nMC;
    for(unsigned int icol = 0; icol < _calorimInpCollections.size(); icol++)
    {
      if( std::find(cnames->begin(), cnames->end(), _calorimInpCollections.at(icol)) != cnames->end() )
      {
        string _inputColName = _calorimInpCollections.at(icol);

        LCCollection *inputCalorimCollection = 0;
        try
        {
          inputCalorimCollection = evt->getCollection(_inputColName);
        }
        catch (EVENT::DataNotAvailableException &e)
        {
          streamlog_out(WARNING)<< "missing collection "
          <<_inputColName<<endl<<e.what()<<endl;
        }
        if (inputCalorimCollection != 0){

          int noHits = inputCalorimCollection->getNumberOfElements();

          event = evtNumber;
          spill = -1;
          cycle = -1;
          bcid = -1;
          bcid_first_sca_full = -999;
          bcid_merge_end = -1;
          id_run = -1;
          id_dat = -1;
          nhit_len = noHits;

          sum_energy_lg = 0.;
          sum_energy = 0.;

          vector<float> slabs_hit;
          vector<int> chans_hit, chips_hit;
          int i_slab;
          float gap_hit_x, gap_hit_y;

          // Hits loop
          for (int i = 0; i < noHits; i++)
          {
            // // Following line works, the pushbacks
            // SimCalorimeterHit *aHit = dynamic_cast<SimCalorimeterHit*>(inputCalorimCollection->getElementAt(i));
            // //auto *aHit = hitCast(inputCalorimCollection->getElementAt(i));
            // // hitCast(*aHit);

            // This is in DigiLCIO2Build
            // CalorimeterHit *aHit = dynamic_cast<CalorimeterHit*>(inputCalorimCollection->getElementAt(i));
            CalorimeterHit *aHit = dynamic_cast<CalorimeterHit*>(inputCalorimCollection->getElementAt(i));

            hit_sca.push_back(-1);
            hit_adc_high.push_back(-1);
            hit_adc_low.push_back(-1);
            hit_n_scas_filled.push_back(1);
            hit_isHit.push_back(1);
            hit_isMasked.push_back(0);
            hit_isCommissioned.push_back(1);

            i_slab = (int)((aHit->getPosition()[2] - _Z0) / _slabSpacing);
            
            hit_slab.push_back(i_slab);

            if (_ConversionGeV2MIP) {
              hit_energy.push_back(aHit->getEnergy() / _GeV2MIP_float[i_slab]);
              sum_energy += aHit->getEnergy() / _GeV2MIP_float[i_slab];
            }
            else {
              hit_energy.push_back(aHit->getEnergy());
              sum_energy += aHit->getEnergy();
            }
            hit_energy_lg.push_back(aHit->getEnergy());
            hit_x.push_back(aHit->getPosition()[0]);
            hit_y.push_back(aHit->getPosition()[1]);
            hit_z.push_back(aHit->getPosition()[2]);
    
            // hit_chip hit_chan
            if (aHit->getPosition()[0] > 0) gap_hit_x = aHit->getPosition()[0] + _HalfCenterGap;
            else gap_hit_x = aHit->getPosition()[0] - _HalfCenterGap;
            if (aHit->getPosition()[1] > 0) gap_hit_y = aHit->getPosition()[1] + _HalfCenterGap;
            else gap_hit_y = aHit->getPosition()[1] - _HalfCenterGap;

            for (int icell = 0; icell < 1024; icell++) {
              float in_x = fabs(_maps[_SlabMapIndices[i_slab]][icell][0] - gap_hit_x);
              float in_y = fabs(_maps[_SlabMapIndices[i_slab]][icell][1] - gap_hit_y);
              if (in_x < 0.1 && in_y < 0.1) {
                hit_chip.push_back(icell/64);
                hit_chan.push_back(icell%64);
              }
              
            }

            // ** Note ** //
            // I didn't find a straightforward way to fill hit_slab,
            // without providing the list of z layer pos (_FixedPosZ) as
            // input in the steering, which is not convenient because
            // per conf/geometry, z points can be different...
            // ** End Note ** //
            
            // Shorter implementation if perfect equality between hit_z and _FixedPosZ
            // auto slab_index = std::find(_FixedPosZ.begin(), _FixedPosZ.end(), aHit->getPosition()[2]);
            // if (slab_index != _FixedPosZ.end()) hit_slab.push_back(std::distance(_FixedPosZ.begin(), slab_index));
            
          }//end loop over SimCalorimeterHits

          sum_energy_lg = sum_energy;
          
          vector<float>::iterator it1;
          // nhit_slab
          slabs_hit = hit_z;
          sort(slabs_hit.begin(), slabs_hit.end());
          it1 = unique(slabs_hit.begin(), slabs_hit.end());
          slabs_hit.resize(std::distance(slabs_hit.begin(), it1));
          nhit_slab = slabs_hit.size();
          
          // nhit_chan
          vector<int>::iterator it2;
          chans_hit = hit_chan;
          sort(chans_hit.begin(), chans_hit.end());
          it2 = unique(chans_hit.begin(), chans_hit.end());
          chans_hit.resize(std::distance(chans_hit.begin(), it2));
          nhit_chan = chans_hit.size();
          // nhit_chip
          vector<int>::iterator it3;
          chips_hit = hit_chip;
          sort(chips_hit.begin(), chips_hit.end());
          it3 = unique(chips_hit.begin(), chips_hit.end());
          chips_hit.resize(distance(chips_hit.begin(), it3));
          nhit_chip = chips_hit.size();
        
        }//end if col
      }//end if find col names

          
    }//end for loop

  _treeout->Fill();
  // Clear vectors
  hit_slab.clear();
  hit_chip.clear();
  hit_chan.clear();
  hit_sca.clear();
  hit_adc_high.clear();
  hit_adc_low.clear();
  hit_n_scas_filled.clear();
  hit_isHit.clear();
  hit_isMasked.clear();
  hit_isCommissioned.clear();

  hit_energy.clear();
  hit_energy_lg.clear();
  hit_x.clear();
  hit_y.clear();
  hit_z.clear();
  //-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !

  streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber()
  << "   in run:  " << evt->getRunNumber() << std::endl ;


  _nEvt ++ ;

  }

  /************************************************************************************/

  void LCIO2BuildProcessor::check( LCEvent * evt ) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
  }

  /************************************************************************************/
  void LCIO2BuildProcessor::end(){

    _treeout->Write();
    _rootout->Write("", TObject::kOverwrite);
    _rootout->Close();
    
    std::cout << "LCIO2BuildProcessor::end()  " << name()
      << " processed " << _nEvt << " events in " << _nRun << " runs "
      << "FLAG"
      << std::endl ;

  }

  /************************************************************************************/
  // void LCIO2BuildProcessor::hitCast(){
  // }
  
  // SimCalorimeterHit LCIO2BuildProcessor::hitCast(SimCalorimeterHit* aHit){
  //   return dynamic_cast<SimCalorimeterHit*>aHit;
  // }

  // SimCalorimeterHit LCIO2BuildProcessor::hitCast(SimCalorimeterHit* aHit){
  //   return dynamic_cast<SimCalorimeterHit*>aHit;
  // }

  // EVENT::CalorimeterHit* LCIO2BuildProcessor::hitCast(EVENT::CalorimeterHit* aHit){
  //   return dynamic_cast<CalorimeterHit*>aHit;
  // }

  /************************************************************************************/
  void LCIO2BuildProcessor::printParameters(){
    std::cerr << "============= LCIO2Build Processor =================" <<std::endl;
    std::cerr << "Converting Simulation Hits to build ROOT file " <<std::endl;
    std::cerr << "Input Collection name : "; for(unsigned int i = 0; i < _calorimInpCollections.size(); i++) std::cerr << _calorimInpCollections.at(i) << " ";
    std::cerr << std::endl;
    //std::cerr << "Output Collection name : "; for(unsigned int i = 0; i < _calorimOutCollections.size(); i++) std::cerr << _calorimOutCollections.at(i) << " ";
    std::cerr << std::endl;
    //std::cerr << "MIP: " << _MIPvalue << " GeV" <<std::endl;
    std::cerr << "=======================================================" <<std::endl;
    return;

  }

}