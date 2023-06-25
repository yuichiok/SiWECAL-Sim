#include <iostream>
#include <vector>

void plotmap (string mask_filename="raw_siwecal_90134_masked_channels.txt") {
    
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

}