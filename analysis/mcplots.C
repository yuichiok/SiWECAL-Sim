//# TFile *_file0 = TFile::Open("data/ECAL_QGSP_BERT_conf1_e-_3.0GeV_100kevt_build.root")
# TFile *_file0 = TFile::Open("/data/flc/jimenez/simulations/TB2022-03/CONF0/build/ECAL_QGSP_BERT_conf0_60anglePos1_e-_3.0GeV_100kevt_shaped_build.root")
.L mipcal.C;                    // mipcalibration functions
                                // and sampling fraction
.L mcmap.C;                     // convertion z -> slab for MC
                                // slab(z)
TTree* ecal = SLCIOConverted;   // to have the same tree name as in data 
// Shower hit distribution in mips 
ecal->Draw("hit_energy/mipcal(slab(hit_z),0):slab(hit_z)>>h(15,-0.5,14.5,100,0,10)","hit_energy/mipcal(slab(hit_z),0)<10", "ZCOL")

// Shower profile in mips (cut at 0.5 mips)
ecal->Draw("hit_energy/mipcal(slab(hit_z),0):slab(hit_z)>>hp(15,-0.5,14.5)", "hit_energy>0.5*mipcal(slab(hit_z),0)", "PROF")
// Energy resolution
ecal->Draw("Sum$(hit_energy/mipcal(slab(hit_z),0)/slabSampFr(slab(hit_z)))", "hit_energy>0.5*mipcal(slab(hit_z),0)", "")                                                                                  


    
