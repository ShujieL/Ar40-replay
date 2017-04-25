{

const Int_t n=12;

//Double_t x[n]={1750,1800,1850,1875,1700,1775,1825,1900};
//Double_t y1[n]={0.85,0.87,0.93,0.94,0.87,0.84,0.90,0.96};
//Double_t y2[n]={0.91,0.97,0.98,0.98,0.83,0.95,0.98,0.99};

Double_t x[n]={1419,1420,1421,1423,1424,1425,1426,1427,1430,1435,1438,1440};
Double_t y1[n]={0.87,0.86,0.90,0.90,0.92,0.92,0.93,0.92,0.93,0.93,0.93,0.93};
Double_t y2[n]={0.86,0.85,0.91,0.94,0.95,0.96,0.97,0.96,0.96,0.96,0.97,0.97};

TGraph* gr= new TGraph(12,x,y1);
TGraph* gr1= new TGraph(12,x,y2);
 
TCanvas *c1= new TCanvas("c1","Fpp HV Scan",200,10,700,500);
//TF1 *f1 = new TF1("f1","pol1",-15,190);

c1->SetFillColor(0);
c1->SetGrid();
//c1->GetFrame()->SetFillColor(21);
c1->GetFrame()->SetBorderSize(12);
// gStyle->SetErrorX(dx);
// gStyle->SetErrory(dy);
//gr->GetXaxis()->SetTitle("HV(V)");
//gr->GetYaxis()->SetTitle("Chamber Efficiency");
gr->SetTitle("Fpp efficiency;Run Number;Chamber Efficiency");
gr->GetYaxis()->SetTitleOffset(1.2);
gr->SetMarkerColor(4);
gr1->SetMarkerColor(2);
gr->SetMarkerStyle(20);
gr1->SetMarkerStyle(20);
//gr->GetXaxis()->SetLimits(-20,200);
gr->SetMinimum(0.8);
gr->SetMaximum(1);
//f1->SetLineWidth(2);
gr->Draw("ALP");
//gr->Fit(f1,"R");
gr1->Draw("LP");

 
 c1->Update();

}
