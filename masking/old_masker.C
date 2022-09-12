#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void masker(){
  
  TFile *in_file = new TFile("/data_ilc/flc/jimenez/simulations/TB2022-03/CONF1/build/ECAL_QGSP_BERT_conf1_e-_1.0GeV_100kevt_shaped_build.root");
  TFile *out_file = new TFile("ECAL_QGSP_BERT_conf1_e-_1.0GeV_100kevt_shaped_build_masked.root", "RECREATE");

  TTree *t = (TTree*) in_file->Get("SLCIOConverted");
  int entries = t->GetEntries();

  //Mapping part
  // Read mapping
  vector<vector<double>> map;
  int row = 0;
  int col;
  ifstream map_file("fev10_chip_channel_x_y_mapping.txt");
  string line;
  getline(map_file, line); // skip line 0
  while (getline(map_file, line)){
    istringstream iss(line);
    vector<double> this_vector;
    double this_val;
    while (iss >> this_val) this_vector.push_back(this_val);
    map.push_back(this_vector);
    row++;
  }
  // Print mapping
  //for (int i = 0; i < 1024; i++) for (int j = 0; j < 6; j++) cout << map[i][j] << " ";

  // Find unique x and y values
  vector<double>::iterator it;
  vector<double> unique_x, unique_y;
  unique_x.push_back(map[0][4]);
  unique_y.push_back(map[0][5]);
  for (int i = 1; i < 1024; i++){
    it = find(unique_x.begin(), unique_x.end(), map[i][4]);
    if (it == unique_x.end()) unique_x.push_back(map[i][4]);
    it = find(unique_y.begin(), unique_y.end(), map[i][5]);
    if (it == unique_y.end()) unique_y.push_back(map[i][5]);
  }
  // Print unique values
  cout << "Unique x"   << endl; for (auto i: unique_x) cout << i << " ";
  cout << "\nUnique y" << endl; for (auto i: unique_y) cout << i << " ";

  // vector<double> simPosXY;
  // for (double val = -88.0; val < 88.0; val += 5.5) simPosXY.push_back(val);
  
  // // Masking part
  // for (int i = 0; i < entries; i++){
  // For each mask entry, the hit condition should be:
  // find in sim and mask such that mask_x - 1.7 or mask -1.8 then set to 0
  //   continue;
  // }

  in_file->Close();

}
