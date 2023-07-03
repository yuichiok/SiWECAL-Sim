#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVectorD.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include <iostream>
#include <numeric>


using namespace std;

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

  // initialize original index locations
  vector<size_t> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

  return idx;
}

#define N_ENERGIES 11

void get_res(int &nhit,     int &nhit_masked,
             float &sume,   float &sume_masked,
             float &weight, float &weight_masked,
             vector<float> * hit_energy, vector<int> *hit_slab, vector<int> *hit_isMasked,
            //  double (&W_thicknesses)[15]) {
             TVectorD W_thicknesses) {
    
    for (int j = 0; j < hit_energy->size(); j++) {
            if (hit_isMasked->at(j) == 0) {
                nhit_masked += 1;
                sume_masked += hit_energy->at(j);
                weight_masked += hit_energy->at(j) * W_thicknesses[hit_slab->at(j)];
            }
            nhit += 1;
            sume += hit_energy->at(j);
            weight += hit_energy->at(j) * W_thicknesses[hit_slab->at(j)];
        }
    return;
}

float moliere(vector<float> * hit_energy,
              vector<float> * hit_x, vector<float> * hit_y, vector<float> * hit_z,
              vector<int> * hit_isMasked, bool masked=false, float containment = 0.95) {

    vector<float> hit_rs;
    vector<float> hit_es;
    float mol_rad = 0.;
    float sume = 0.;
    float wx = 0.; float wy = 0.; float wz = 0.;
    float r;

    for (int j = 0; j < hit_energy->size(); j++){
        if (masked && hit_isMasked->at(j) == 1) continue;
        sume += hit_energy->at(j);
        wx += hit_x->at(j) * hit_energy->at(j);
        wy += hit_y->at(j) * hit_energy->at(j);
        wz += hit_z->at(j) * hit_energy->at(j);
    }

    float bary_x = wx / sume;  
    float bary_y = wy / sume;  

    for (int j = 0; j < hit_energy->size(); j++){
        if (masked && hit_isMasked->at(j) == 1) continue;
        r = pow(pow((hit_x->at(j) - bary_x) , 2) + pow((hit_y->at(j) - bary_y), 2), 0.5);
        hit_rs.push_back(r);
    }
    for (auto k: sort_indexes(hit_rs)) hit_es.push_back(hit_energy->at(k));
    
    sort(hit_rs.begin(), hit_rs.end());

    float mol_e = 0.;
    int mol_i;
    
    for (mol_i = 0; mol_i < hit_rs.size(); mol_i++) {
        mol_e += hit_es.at(mol_i);
        if (mol_e >= containment * sume) break; 
    }
    
    return hit_rs.at(mol_i-1);      

}

void fit_res(TH1 * h, double &mu, double &sig, double &res){
    h->Fit("gaus", "q");
    mu = h->GetFunction("gaus")->GetParameter("Mean");
    sig = h->GetFunction("gaus")->GetParameter("Sigma");
    res = h->GetFunction("gaus")->GetParameter("Sigma") / h->GetFunction("gaus")->GetParameter("Mean");
    return;
}

void single_res (string filename) {
    // gROOT->ProcessLine(".L resolutions.C");
    // gSystem->Load("resolutions.C");
    // gROOT->LoadMacro("resolutions.C");
    
    double W[15] = {4.2, 4.2, 4.2, 4.2, 4.2, 4.2, 4.2, 4.2, 5.6, 5.6, 5.6, 5.6, 5.6, 5.6, 5.6}; 
    TVectorD W_thicknesses(15, W);
    
    double mu_nhit, mu_nhit_masked, mu_sume, mu_sume_masked, mu_weight, mu_weight_masked;
    double sig_nhit, sig_nhit_masked, sig_sume, sig_sume_masked, sig_weight, sig_weight_masked;
    double res_nhit, res_nhit_masked, res_sume, res_sume_masked, res_weight, res_weight_masked;

    double mol, mol_masked;
    double mol_sig, mol_sig_masked;
    
    // Output filename
    TFile f("single_resolution_result.root", "recreate");

    TFile *file = new TFile(filename.c_str(), "read");
    TTree *tree = (TTree*)file->Get("ecal");

    Long64_t nentries = tree->GetEntries();

    vector<float> *hit_energy = 0;
    vector<float> *hit_x = 0;
    vector<float> *hit_y = 0;
    vector<float> *hit_z = 0;
    vector<int> *hit_isMasked = 0;
    vector<int> *hit_slab = 0;
    
    TBranch *bhit_energy = 0;
    TBranch *bhit_x = 0;
    TBranch *bhit_y = 0;
    TBranch *bhit_z = 0;
    TBranch *bhit_isMasked = 0;
    TBranch *bhit_slab= 0;


    tree->SetBranchAddress("hit_energy", &hit_energy, &bhit_energy);
    tree->SetBranchAddress("hit_x", &hit_x, &bhit_x);
    tree->SetBranchAddress("hit_y", &hit_y, &bhit_y);
    tree->SetBranchAddress("hit_z", &hit_z, &bhit_z);
    tree->SetBranchAddress("hit_isMasked", &hit_isMasked, &bhit_isMasked);
    tree->SetBranchAddress("hit_slab", &hit_slab, &bhit_slab);

    // Resolution histos
    
    TH1I *h_nhit = new TH1I("NumHits", "Number of hits", 4000, 0, 4000);
    TH1I *h_nhit_masked = new TH1I("NumHitsMask", "Number of hits, masked", 4000, 0, 4000);
    TH1F *h_sume = new TH1F("SumEnergy", "Sum Energy", 500, 0, 15000);
    TH1F *h_sume_masked = new TH1F("SumEnergyMask", "Sum Energy, masked", 500, 0, 15000);
    TH1F *h_weight = new TH1F("WSumEnergy", "W Sum Energy", 500, 0, 70000);
    TH1F *h_weight_masked = new TH1F("WSumEnergyMask", "W Sum Energy, masked", 500, 0, 70000);
    
    // Moliere histos
    TH1F *h_mol = new TH1F("Radius95", "Radius containing 95% of energy", 100, 0., 100.);
    TH1F *h_mol_masked = new TH1F("Radius95Masked", "Radius containing 95% of energy Masked", 100, 0., 100.);

    for (int i_event = 0; i_event < nentries; i_event++) {
            
        // Resolution
        int nhit = 0;      int nhit_masked = 0;
        float sume = 0;    float sume_masked = 0;
        float weight = 0;  float weight_masked = 0;

        tree->GetEntry(i_event);
        // cout << "Event " << to_string(i_event) << endl;
        // if (i_event ==1000) break;
        get_res(nhit, nhit_masked, sume, sume_masked,
                weight, weight_masked,
                hit_energy, hit_slab, hit_isMasked,
                W_thicknesses);

        h_nhit->Fill(nhit);         h_nhit_masked->Fill(nhit_masked);
        h_sume->Fill(sume);         h_sume_masked->Fill(sume_masked);
        h_weight->Fill(weight);     h_weight_masked->Fill(weight_masked);
        
        // Fill Moliere radii histograms
        h_mol->Fill(moliere(hit_energy, hit_x, hit_y, hit_z, hit_isMasked));
        h_mol_masked->Fill(moliere(hit_energy, hit_x, hit_y, hit_z, hit_isMasked, true));
        
        hit_isMasked->clear();
        hit_energy->clear();
        hit_slab->clear();

    }

    fit_res(h_nhit, mu_nhit, sig_nhit, res_nhit);
    fit_res(h_nhit_masked, mu_nhit_masked, sig_nhit_masked, res_nhit_masked);
    fit_res(h_sume, mu_sume, sig_sume, res_sume);
    fit_res(h_sume_masked, mu_sume_masked, sig_sume_masked, res_sume_masked);
    fit_res(h_weight, mu_weight, sig_weight, res_weight);
    fit_res(h_weight_masked, mu_weight_masked, sig_weight_masked, res_weight_masked);


    h_mol->Fit("gaus", "q");
    mol = h_mol->GetFunction("gaus")->GetParameter("Mean");
    mol_sig = h_mol->GetFunction("gaus")->GetParameter("Sigma")/nentries;
    
    h_mol_masked->Fit("gaus", "q");
    mol_masked = h_mol_masked->GetFunction("gaus")->GetParameter("Mean");
    mol_sig_masked = h_mol_masked->GetFunction("gaus")->GetParameter("Sigma")/nentries;

    f.WriteTObject(h_nhit);    f.WriteTObject(h_nhit_masked);
    f.WriteTObject(h_sume);    f.WriteTObject(h_sume_masked);
    f.WriteTObject(h_weight);  f.WriteTObject(h_weight_masked);
    f.WriteTObject(h_mol);     f.WriteTObject(h_mol_masked);

    f.Close();

    cout << "Resolution\t\t\t(masked)\n";
    cout << "Number of hits:" << endl; 
    cout << "mu\t=\t" << mu_nhit << "\t" << mu_nhit_masked << endl;
    cout << "sigma\t=\t" << sig_nhit << "\t" << sig_nhit_masked  << endl;
    cout << "sig/mu\t=\t" << sig_nhit/mu_nhit << "\t" << sig_nhit_masked/mu_nhit_masked  << endl;
    cout << endl;

    cout << "Sum energy:" << endl; 
    cout << "mu\t=\t" << mu_sume << "\t" << mu_sume_masked << endl;
    cout << "sigma\t=\t" << sig_sume << "\t" << sig_sume_masked << endl;
    cout << "sig/mu\t=\t" << sig_sume/mu_sume << "\t" << sig_sume_masked/mu_sume_masked << endl;
    cout << endl;

    cout << "Weighted sum energy:" << endl; 
    cout << "mu\t=\t" << mu_weight << "\t" << mu_weight_masked << endl;
    cout << "sigma\t=\t" << sig_weight << "\t" << sig_weight_masked << endl;
    cout << "sig/mu\t=\t" << sig_weight/mu_weight << "\t" << sig_weight_masked/mu_weight_masked << endl;

}