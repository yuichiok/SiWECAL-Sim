#define N_ENERGIES 11

void graph_setup_add(TGraph *g, string title, Color_t color){
    g->SetTitle(title.c_str());
    g->SetLineColor(color);
    g->SetLineWidth(3);
    return;
}

void res_mol_plots(string filename, bool transformed = true){
    
    TFile *file = new TFile(filename.c_str(), "read");
    // TVectorD * energies = file->GetObject("energies");
    TVectorD *energies;
    TVectorD *energies_tr;
    
    TVectorD *res_nhit, *mu_nhit, *sig_nhit;
    TVectorD *res_nhit_masked, *mu_nhit_masked, *sig_nhit_masked;
    TVectorD *res_sume, *mu_sume, *sig_sume;
    TVectorD *res_sume_masked, *mu_sume_masked, *sig_sume_masked;
    TVectorD *res_weight, *mu_weight, *sig_weight;
    TVectorD *res_weight_masked, *mu_weight_masked, *sig_weight_masked;
    
    TVectorD *mol, *mol_masked, *mol_sig, *mol_sig_masked;


    TVectorD zeros(N_ENERGIES); for (int j = 0; j < N_ENERGIES; j++) zeros[j] = 0;
    
    file->GetObject("energies", energies);
    file->GetObject("energies_tr", energies_tr);
    
    file->GetObject("res_nhit", res_nhit);
    file->GetObject("res_nhit_masked", res_nhit_masked);
    file->GetObject("res_sume", res_sume);
    file->GetObject("res_sume_masked", res_sume_masked);
    file->GetObject("res_weight", res_weight);
    file->GetObject("res_weight_masked", res_weight_masked);
    
    file->GetObject("mu_nhit", mu_nhit);
    file->GetObject("mu_nhit_masked", mu_nhit_masked);
    file->GetObject("mu_sume", mu_sume);
    file->GetObject("mu_sume_masked", mu_sume_masked);
    file->GetObject("mu_weight", mu_weight);
    file->GetObject("mu_weight_masked", mu_weight_masked);
    
    file->GetObject("sig_nhit", sig_nhit);
    file->GetObject("sig_nhit_masked", sig_nhit_masked);
    file->GetObject("sig_sume", sig_sume);
    file->GetObject("sig_sume_masked", sig_sume_masked);
    file->GetObject("sig_weight", sig_weight);
    file->GetObject("sig_weight_masked", sig_weight_masked);
    
    file->GetObject("mol", mol);
    file->GetObject("mol_masked", mol_masked);
    file->GetObject("mol_sig", mol_sig);
    file->GetObject("mol_sig_masked", mol_sig_masked);
    
    // // string e_str = to_string((int)round(energies[i_energy])) + "GeV";
    // // Resolution
    
    TGraph *g_res_nhit = new TGraph(*energies_tr, *res_nhit);                 
    graph_setup_add(g_res_nhit, "N hits", kBlack);
    
    TGraph *g_res_nhit_masked = new TGraph(*energies_tr, *res_nhit_masked);   
    graph_setup_add(g_res_nhit_masked, "N hits (masked)", kBlue);
    
    TGraph *g_res_sume = new TGraph(*energies_tr, *res_sume);                 
    graph_setup_add(g_res_sume, "Sum energy", kRed);
    
    TGraph *g_res_sume_masked = new TGraph(*energies_tr, *res_sume_masked);   
    graph_setup_add(g_res_sume_masked, "Sum energy (masked)", kGreen);
    
    TGraph *g_res_weight = new TGraph(*energies_tr, *res_weight);                 
    graph_setup_add(g_res_weight, "Weighted sum energy", kViolet);
    
    TGraph *g_res_weight_masked = new TGraph(*energies_tr, *res_weight_masked);   
    graph_setup_add(g_res_weight_masked, "Weighted sum energy (masked)", kCyan);
    
    
    TMultiGraph *mg = new TMultiGraph();
    
    mg->Add(g_res_nhit);
    // mg->Add(g_res_nhit_masked);
    mg->Add(g_res_sume);
    // mg->Add(g_res_sume_masked);
    mg->Add(g_res_weight);
    // mg->Add(g_res_weight_masked);

    auto c = new TCanvas("c", "c", 800, 600);
    mg->Draw("AL");
    mg->SetTitle("Resolution vs energy");
    if (transformed) mg->GetXaxis()->SetTitle("1/#sqrt{E[GeV]}");
    else mg->GetXaxis()->SetTitle("E[GeV]");
    mg->GetYaxis()->SetTitle("#sigma/#mu");
    

    // Debugging tvectord access
    for (int j = 0; j < N_ENERGIES; j++) {cout << "This energy" << ((*energies_tr))[j] << endl;}

    // energies->Print();
    if (!transformed){
        mg->GetXaxis()->SetNdivisions(-502);
        int bin_index;
        for (int j = 0; j < N_ENERGIES; j++) {
            bin_index = mg->GetXaxis()->FindBin(((*energies_tr))[j]);
            if (j == 8 || j == 7 || j == 9 ) continue;
            // cout << "This energy: " << to_string(energies[j]) << endl;
            mg->GetXaxis()->SetBinLabel(bin_index, to_string((int)round(((*energies))[j])).c_str());
            }
        mg->GetXaxis()->LabelsOption("h");
        mg->GetXaxis()->SetLabelSize(0.05);
    }
    c->BuildLegend();
    c->SaveAs("resolution_conf11.pdf");
    c->SaveAs("resolution_conf11.root");
    
    // // Linearity

    TMultiGraph *mg_lin_nhit = new TMultiGraph();
    TMultiGraph *mg_lin_sume = new TMultiGraph();
    TMultiGraph *mg_lin_weight = new TMultiGraph();
    
    //
    TGraph *g_lin_nhit = new TGraph(*energies, *mu_nhit);                 
    graph_setup_add(g_lin_nhit, "n hits", kBlack);
    
    TGraph *g_lin_nhit_masked = new TGraph(*energies, *mu_nhit_masked);   
    graph_setup_add(g_lin_nhit_masked, "n hits (masked)", kBlue);
    
    TGraph *g_lin_sume = new TGraph(*energies, *mu_sume);                 
    graph_setup_add(g_lin_sume, "sum energy", kRed);
    
    TGraph *g_lin_sume_masked = new TGraph(*energies, *mu_sume_masked);   
    graph_setup_add(g_lin_sume_masked, "sum energy (masked)", kGreen);
    
    TGraph *g_lin_weight = new TGraph(*energies, *mu_weight);                 
    graph_setup_add(g_lin_weight, "weighted sum energy", kViolet);
    
    TGraph *g_lin_weight_masked = new TGraph(*energies, *mu_weight_masked);   
    graph_setup_add(g_lin_weight_masked, "weighted sum energy (weighted)", kCyan);
    // //


    mg_lin_nhit->Add(g_lin_nhit);
    // mg_lin_nhit->Add(g_lin_nhit_masked);
    mg_lin_sume->Add(g_lin_sume);
    // mg_lin_sume->Add(g_lin_sume_masked);
    mg_lin_weight->Add(g_lin_weight);
    // mg_lin_weight->Add(g_lin_weight_masked);

    auto c_lin_nhit = new TCanvas("c_lin_nhit", "c_lin_nhit", 800, 600);
    mg_lin_nhit->Draw("AL");
    mg_lin_nhit->SetTitle("Linearity (n hits)");
    mg_lin_nhit->GetXaxis()->SetTitle("E [GeV]");
    mg_lin_nhit->GetYaxis()->SetTitle("Number of hits");
    c_lin_nhit->BuildLegend();
    c_lin_nhit->SaveAs("lin_nhits_conf11.pdf");
    c_lin_nhit->SaveAs("lin_nhits_conf11.root");

    auto c_lin_sume = new TCanvas("c_lin_sume", "c_lin_sume", 800, 600);
    mg_lin_sume->Draw("AL");
    mg_lin_sume->SetTitle("Linearity (sum energy)");
    mg_lin_sume->GetXaxis()->SetTitle("E [GeV]");
    mg_lin_sume->GetYaxis()->SetTitle("Sum energy");
    c_lin_sume->BuildLegend();
    c_lin_sume->SaveAs("lin_sume_conf11.pdf");
    c_lin_sume->SaveAs("lin_sume_conf11.root");

    auto c_lin_weight = new TCanvas("c_lin_weight", "c_lin_weight", 800, 600);
    mg_lin_weight->Draw("AL");
    mg_lin_weight->SetTitle("Linearity (weighted sum energy)");
    mg_lin_weight->GetXaxis()->SetTitle("E [GeV]");
    mg_lin_weight->GetYaxis()->SetTitle("Weighted energy");
    c_lin_weight->BuildLegend();
    c_lin_weight->SaveAs("lin_weight_conf11.pdf");
    c_lin_weight->SaveAs("lin_weight_conf11.root");


    // Moliere
    // TGraphErrors *g_mol = new TGraphErrors(N_ENERGIES, energies_tr, mol, zeros, mol_sig);   
    TGraphErrors *g_mol = new TGraphErrors(*energies_tr, *mol, zeros, *mol_sig);   
    graph_setup_add(g_mol, "Moliere Radius", kBlue);
    
    // TGraphErrors *g_mol_masked = new TGraphErrors(N_ENERGIES, energies_tr, mol_masked, zeros, mol_sig_masked);   
    TGraphErrors *g_mol_masked = new TGraphErrors(*energies_tr, *mol_masked, zeros, *mol_sig_masked);   
    graph_setup_add(g_mol_masked, "Moliere Radius (masked)", kRed);

    TMultiGraph *mg_mol = new TMultiGraph();
    mg_mol->Add(g_mol);
    // mg_mol->Add(g_mol_masked);

    auto c2 = new TCanvas("c2", "c2", 800, 600);
    
    mg_mol->Draw("AL");
    mg_mol->SetTitle("Moliere radius vs energy");
    // mg_mol->GetXaxis()->SetTitle("Energy [GeV]");
    if (transformed) mg_mol->GetXaxis()->SetTitle("1/#sqrt{E[GeV]}");
    else mg_mol->GetXaxis()->SetTitle("E[GeV]");
    mg_mol->GetYaxis()->SetTitle("Radius (95%) [mm]");
    if (!transformed){
        mg_mol->GetXaxis()->SetNdivisions(-502);
        int bin_index;
        for (int j = 0; j < N_ENERGIES; j++) {
            bin_index = mg_mol->GetXaxis()->FindBin(((*energies_tr))[j]);
            if (j == 8 || j == 7 || j == 9 ) continue;
            mg_mol->GetXaxis()->SetBinLabel(bin_index, to_string((int)round(((*energies))[j])).c_str());
            }
        mg_mol->GetXaxis()->LabelsOption("h");
        mg_mol->GetXaxis()->SetLabelSize(0.05);
    }
    
    c2->BuildLegend();
    c2->SaveAs("moliere_conf11.pdf");
    c2->SaveAs("moliere_conf11.root");

}