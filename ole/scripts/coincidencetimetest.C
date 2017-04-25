//Check if Signal is inside Gate
//Barak Schmookler, Feb 2016
void coincidencetimetest(Char_t *drawoption){

  gStyle->SetOptStat(0);

  TTree *tree = (TTree*)gDirectory->Get("T");


    TH1F *ht10 = new TH1F("ht10","S2l6_LEFT versus S2l6_RIGHT retimed",1000,0,2500);
    ht10->GetXaxis()->SetTitle("S2L6 TDC LHRS");ht10->GetXaxis()->CenterTitle();
//     ht10->GetYaxis()->SetTitle("S2L6 TDC RHRS retimed for LHRS");ht10->GetYaxis()->CenterTitle();
    ht10->SetMarkerStyle(3);ht10->SetMarkerSize(0.75);

    tree->Draw("L.s2.lt[5]-R.s2.lt[5]-DR.Retiming_L[0]>>ht10","((DR.evtypebits>>3)&1)",drawoption);


}
