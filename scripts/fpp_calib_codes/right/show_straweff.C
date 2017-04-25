#include <iostream>

Int_t show_straweff(){
  TString ss,sss,scut;
  Int_t runnum=4275;
  cout<<"Run number? "<<endl;
  cin>>runnum;
  cout<<"Processing run "<<runnum<<endl;
  TString froot;
  froot = Form("../root_align/ledex/ledex_%d.root",runnum);
  TChain eff("T");
  //    runnum=2094;
//  froot = Form("/work/halla/ledex/disk1/adaq/run_1763/run_1766/ledex_%d.root",runnum);
   eff.Add(froot);
//   runnum=2890;
//  froot = Form("/work/halla/ledex/disk1/adaq/run_1763/run_1766/ledex_%d.root",runnum);
//  eff.Add(froot);
  //  runnum=1785;
  //froot = Form("/work/halla/ledex/disk1/adaq/run_1763/run_1766/ledex_%d.root",runnum);
  //eff.Add(froot);
  //  f = new TFile(froot);
  //TTree *t1 = (TTree*)f->Get("T");                  //Get loaded tree 
  TH1F *strgood[24]; 
  TH1F *strbad[24]; 
  // the histogram order is v1-3,u1-3,v4-6,u4-6,u7-8,v7-8,x1-2,u9-11,v9-11
  Int_t nwg_plane[24] = {20,20,20,22,22,22,20,20,20,22,22,22,30,30,30,30,31,31,35,35,35,35,35,35};
  char pltype[24] = {'v','v','v','u','u','u','v','v','v','u','u','u','u','u','v','v','x','x','u','u','u','v','v','v'};
  Int_t nplane[24] = {1,2,3,1,2,3,4,5,6,4,5,6,7,8,7,8,1,2,9,10,11,9,10,11} ;
  Int_t div_pl[24] = {5,5,5,6,6,6,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7} ;
  Int_t pl,wg,k;
  Int_t hbinmax=300;
  Int_t hbinmin=1;
  Int_t hbinnum=300;
for( pl=0; pl<24; pl++) {
  cout << "Plane : " << pl+1 << endl ;
    ss = Form("strawgood%c%d",pltype[pl],nplane[pl]);
    strgood[pl] = new TH1F (ss,ss,hbinnum,hbinmin,hbinmax);
    sss = Form("L.fpp.%c%d.strwgood",pltype[pl],nplane[pl]);
    scut = Form("L.fpp.%c%d.efficiency==%d&&L.fpp.ntrackfu==1&&L.fpp.ntrackfv==1&&L.fpp.ntrackru==1&&L.fpp.ntrackrv==1",pltype[pl],nplane[pl],1);
    eff.Project(ss,sss,scut);
    ss = Form("strawbad%c%d",pltype[pl],nplane[pl]);
    strbad[pl] = new TH1F (ss,ss,hbinnum,hbinmin,hbinmax);
    scut = Form("L.fpp.%c%d.efficiency==%d&&L.fpp.ntrackfu==1&&L.fpp.ntrackfv==1&&L.fpp.ntrackru==1&&L.fpp.ntrackrv==1",pltype[pl],nplane[pl],0);
    sss = Form("L.fpp.%c%d.strwbad",pltype[pl],nplane[pl]);
    eff.Project(ss,sss,scut);
}
  TString tt,ttopen,ttclose;
  ttopen = Form("straweff_%d.ps[",runnum);
  tt = Form("straweff_%d.ps",runnum);
  ttclose = Form("straweff_%d.ps]",runnum);
  TCanvas *d0 = new TCanvas("d0","d0",1000,600);
  Int_t test,ii;
  Float_t badcnts,goodcnts,tot,heff[300],straw[300];
 d0->Print(ttopen);
  d0->Divide(1,2);
for (pl=0;pl<24;pl++){
     if ( pl+1 < 17 ||  pl+1 > 18) {
    d0->cd(1);
    strgood[pl]->Draw();
    d0->Modified();
    d0->Update();
    strbad[pl]->SetLineColor(2);
    strbad[pl]->Draw("same");
    d0->Modified();
    d0->Update();
    ss = Form("eff_%c%d",pltype[pl],nplane[pl]);
    TH1F *h1 = new TH1F(ss,"Efficiency vs straw",hbinmax,0,hbinmax);
    for (ii=1;ii<hbinmax+1;ii++) { 
      straw[ii-1] = ii;
      badcnts = strbad[pl]->GetBinContent(ii);
      goodcnts = strgood[pl]->GetBinContent(ii);
      tot=badcnts+goodcnts;
      //      cout << "ii = " << ii << "badcnts = " << badcnts << " " << goodcnts << " " << straw[ii-1]   << endl ;
      heff[ii-1] = 0. ;
	if (tot != 0) heff[ii-1] = goodcnts/tot ;
    }
    for (ii=0;ii<hbinmax;ii++) h1->Fill(straw[ii],heff[ii]);
     d0->cd(2);
   h1->Draw();
    d0->Modified();
    d0->Update();    
   d0->Print(tt);
  }

}
d0->Print(ttclose);
}
