#include <iostream>
#include <vector>

Float_t map_pointX[16][64];
Float_t map_pointY[16][64];

void plotmap (string mask_filename="raw_siwecal_90134_masked_channels.txt") {
    
  TString map_fev10="fev10_chip_channel_x_y_mapping.txt";
  TString map_cob  ="fev11_cob_rotate_chip_channel_x_y_mapping.txt";
  std::ifstream reading_file(map_fev10);
  if(!reading_file){
    cout<<" dameyo - damedame"<<endl;
  }
  for(int i=0; i<16; i++) {
    for(int j=0; j<64; j++) {
      map_pointX[i][j] = -1000.;
      map_pointY[i][j] = -1000.;
    }
  }

  Int_t tmp_chip = 0,tmp_channel = 0;
  Float_t tmp_x0 = 0 ,tmp_y0 = 0 , tmp_x = 0 , tmp_y = 0 ;
  TString tmpst;
  reading_file >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst >> tmpst ;
  while(reading_file){
    reading_file >> tmp_chip >> tmp_x0 >> tmp_y0 >> tmp_channel >> tmp_x >> tmp_y ;
    map_pointX[tmp_chip][tmp_channel] = -tmp_x ;
    map_pointY[tmp_chip][tmp_channel] = -tmp_y ;
  }

  // Read the mask

  int N_LAYER(15), N_CHIP(16), N_CHAN(64);
  ifstream f;
  f.open(mask_filename);
  string line;
  getline(f, line); // Get rid of first line
  int is_masked;
  vector<vector<vector<int>>> mask(N_LAYER, vector<vector<int>>(N_CHIP, vector<int>(N_CHAN)));
  vector<int> this_layer_chip;
  int this_chip, this_layer;
  for (int i_layer = 0; i_layer < N_LAYER; i_layer++) {
    for (int i_chip = 0; i_chip < N_CHIP; i_chip++) {
      getline(f, line);
      istringstream stm(line);
      stm >> this_layer >> this_chip; 
      if (!((i_layer == this_layer) && (i_chip == this_chip))) {cout << "Chip/layer not coinciding!" << endl; break;}
      while(stm >> is_masked) this_layer_chip.push_back(is_masked);
      mask[i_layer][i_chip] = this_layer_chip;
      this_layer_chip.clear();
    }
  }

  // Plot the map
  TH2F* mapxy =new TH2F("mapxy","map-xy; x; y",32,-90,90,32,-90,90);
  TH2F* mapxy_chip =new TH2F("mapxy_chip","map-xy; x; y",32,-90,90,32,-90,90);
  int fev10_layer = 4;
  for( int i_chip=0; i_chip < N_CHIP; i_chip++) {
    for( int i_chan=0; i_chan < N_CHAN; i_chan++) {
      if (mask[fev10_layer][i_chip][i_chan] == 1) {
        mapxy->Fill(map_pointX[i_chip][i_chan],map_pointY[i_chip][i_chan],i_chan);
      }
      mapxy_chip->Fill(map_pointX[i_chip][i_chan],map_pointY[i_chip][i_chan],i_chip+1);
    }
  }

  gStyle->SetOptStat(0);
  TCanvas* c1 = new TCanvas("c1","c1",800,800);
  mapxy_chip->Draw("colz");
  mapxy->Draw("textsame");




}