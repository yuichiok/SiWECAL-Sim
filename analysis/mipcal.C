// int FevType[15] =
//     {13,13,                      // FEV13
//      19,19,                      // COB
//      0.320, 0.320, 0.320,       // 3
//      0.500, 0.500, 0.500, 0.500, // 4
//      0.320, 0.320, 0.320, 0.320  // 4
//     };
double ThSi[15] =
    {0.650, 0.650,              // 2
     0.500, 0.500,              // 2
     0.320, 0.320, 0.320,       // 3
     0.500, 0.500, 0.500, 0.500, // 4
     0.320, 0.320, 0.320, 0.320  // 4
    };
double ThW[15] = {
    2.8, 2.8, 2.8, 2.8, 2.8, 2.8,2.8, 2.8, // 8
    4.2,4.2,4.2,4.2,4.2,4.2,4.2} ;             // 7
double X0W =   3.504;                      // mm
double X0Si = 93.70;                        // mm

double mc255[15] = {
    4.9, 5.4,
    4.3, 4.1,
    2.4, 1.8, 1.8,
    3.96, 4.0, 3.8, 3.6,
    2.1, 2.1, 2.05, 2.3
};
double mc282[15] = {
    1.2, 1.33,
    1.28, 1.17,
    1.1, 0.95, 0.97,
    1.2, 1.2, 0.97, 1.2,
    0.96, 0.94, 1.04, 1.07
};
double mcmc[15] = {
    2.2e-4, 2.2e-4,                 // 650
    1.7e-4, 1.7e-4,                 // 320
    1.1e-4, 1.1e-4, 1.1e-4,         // 500
    1.7e-4, 1.7e-4, 1.7e-4, 1.7e-4, // 320
    1.1e-4, 1.1e-4, 1.1e-4, 1.1e-4  // 500
};
//        Double mcmc[15] = {
//    };
double mipcal(int slab, int run) {
    if( slab>=0 && slab <15){
        if( run == 255 ) return mc255[slab];
        else if( run == 282 ) return mc282[slab];
        else if( run == 0 ) return mcmc[slab];
    }
    return 1;
}

double slabSiTh(int slab){
    if( slab>=0 && slab <15){
        return ThSi[slab];
    }
    return 1;
}    

double slabWTh(int slab){
    if( slab>=0 && slab <15){
        return ThW[slab];
    }
    return 1;
}    

double slabSampFr(int slab){
    if( slab>=0 && slab <15){
        double f = (ThSi[slab]/X0Si)/(ThW[slab]/X0W + ThSi[slab]/X0Si);
        return f;
    }
    return 1;
}
