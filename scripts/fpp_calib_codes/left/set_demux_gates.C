#include <iostream>

Int_t set_demux_gates(){
  TString ss,sss,scut;
  char pltype[24] = {'v','v','v','u','u','u','v','v','v','u','u','u',,'u','u','v','v','x','x','u','u','u','v','v','v'};
  Int_t runnum = 2102;
  cout<<"Run number? "<<endl;
  cin>>runnum;
  cout<<"Processing run "<<runnum<<endl;
  TString froot;
    froot = Form("~/gmp/replay/Rootfiles/left_gmp_%d.root",runnum);
  //froot = Form("/work/halla/ledex/disk1/adaq/run_1763/run_1766/ledex_%d.root.save",runnum);
  //froot = Form("/work/halla/ledex/disk1/adaq/run_1763/run_1766/ledex_%d.root.save",runnum);
  f = new TFile(froot);
  TTree *t1 = (TTree*)f->Get("T");                  //Get loaded tree 
  TH1F *demux[24][35]; 
  // the histogram order is v1-3,u1-3,v4-6,u4-6,u7-8,v7-8,x1-2,u9-11,v9-11
  // This matchs the expected order in the database to the demux gates
  // In database there are assumed 35 wiregroups of 9 gates for each plane
  // Fill extra wiregroups for planes with < 35 wiregroups with zeroes 
  Int_t nwg_plane[24] = {20,20,20,22,22,22,20,20,20,22,22,22,30,30,30,30,31,31,35,35,35,35,35,35};
  Int_t nplane[24] = {1,2,3,1,2,3,4,5,6,4,5,6,7,8,7,8,1,2,9,10,11,9,10,11} ;
  Int_t pl,wg,k;
  Int_t hbinmax=200;
  Int_t hbinmin=0;
  Int_t hbinnum=200;
  // read demux gates
  Float_t demux_gates[24][35][9] ;
  //FILE *ff = fopen("db_L.fpp.dat","r");
  ifstream dbfile("gates.dat");
 pl = 0;
 wg = 0;
for( pl=0; pl<6; pl++) {
  cout << "Plane : " << pl+1 << " or " << pltype[pl] << nplane[pl] << endl ;
for( wg=0; wg<35; wg++) {
  dbfile >> demux_gates[pl][wg][0] >> demux_gates[pl][wg][1] >> demux_gates[pl][wg][2] >> demux_gates[pl][wg][3] >> demux_gates[pl][wg][4] >> demux_gates[pl][wg][5]  >> demux_gates[pl][wg][6] >> demux_gates[pl][wg][7] >> demux_gates[pl][wg][8] ;
  // cout << demux_gates[pl][wg][0] << demux_gates[pl][wg][1] << demux_gates[pl][wg][2] << demux_gates[pl][wg][3] << demux_gates[pl][wg][4] << demux_gates[pl][wg][5]  << demux_gates[pl][wg][6] << demux_gates[pl][wg][7] << demux_gates[pl][wg][8] << endl ;
 }  
}
 dbfile.close();
  //
  Int_t vpl=1;
  cout << "Which plane (1-6) ( type 99 to quit)?" << endl ;
  cin >> vpl;
  vpl = vpl -1;
  while (vpl <6) {  
  //
 // fill histograms
for( pl=vpl; pl<vpl+1; pl++) {
  cout << "Plane : " << pl+1 << " or " << pltype[pl] << nplane[pl] << endl ;
for( wg=0; wg<nwg_plane[pl]; wg++) {
     k=wg+1;  
    ss = Form("tdc%c%d_%d",pltype[pl],nplane[pl],k);
    demux[pl][wg] = new TH1F (ss,ss,hbinnum,hbinmin,hbinmax);
    sss = Form("L.fpp.%c%d.width",pltype[pl],nplane[pl]);
    scut = Form("L.fpp.%c%d.wire==%d",pltype[pl],nplane[pl],k);
    t1->Project(ss,sss,scut);
    cout << k << endl ;
  char temp[128];
  sprintf(temp,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f   \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
  //  cout << " Demux gates : " << temp << endl ;
}
}
//
// Create ps file
  TString tt,ttopen,ttclose;
  //  ttopen = Form("demux_pl%c%d_%d.ps[",pltype[vpl],nplane[vpl],runnum);
  //tt = Form("demux_pl%c%d_%d.ps",pltype[vpl],nplane[vpl],runnum);
  //ttclose = Form("demux_pl%c%d_%d.ps]",pltype[vpl],nplane[vpl],runnum);
  // create canvas and plot raw tdc histo and determine tzero offset
  TCanvas *d0 = new TCanvas("d0","d0",1000,600);
  d0->Divide(1,1);
 d0->Print(ttopen);
 Int_t max_bin,max_content;
for (pl=vpl;pl<vpl+1;pl++){
  //for (wg=0;wg<nwg_plane[pl];wg++){
  Int_t pickwg = 0;
  while (pickwg !=99) {
    cout << " Which wire group (1-" << nwg_plane[pl] << ")  ( type 99 to quit)?" << endl; 
    cin >> pickwg;
    if ( pickwg !=99) {
    wg = pickwg-1;
    if ( wg <0 ) wg = 0;
    if ( wg >=nwg_plane[pl]) wg =nwg_plane[pl]-1;
    d0->cd(1);
    demux[pl][wg]->Draw();
    max_bin = demux[pl][wg]->GetMaximumBin();
    max_content = demux[pl][wg]->GetBinContent(max_bin);
    d0->Modified();
    d0->Update();
    for (ng=0;ng<9;ng++) {
      TLine *tzline = new TLine(demux_gates[pl][wg][ng],0,demux_gates[pl][wg][ng],max_content);
     tzline->SetLineColor(2);
    tzline->Draw();
    d0->Modified();
    d0->Update();
    }
    char ans;
   ans = 'y';
   cout << " Are gates ok ( y/n) ?" << endl;
   cin >> ans ;
   Int_t cgate;
   Float_t nval;
   while ( ans == 'n') {
   cout << " Which gate to change ( 0-8 ) ?" << endl;
   cin >> cgate ;
   cout << " What value for gate " << cgate << " (present  channel number = " << demux_gates[pl][wg][cgate] << " ) ?" << endl;
   cin >> nval ;
   demux_gates[pl][wg][cgate] = nval ;
   d0 -> Clear();
    demux[pl][wg]->Draw();
    for (ng=0;ng<9;ng++) {
      TLine *tzline = new TLine(demux_gates[pl][wg][ng],0,demux_gates[pl][wg][ng],max_content);
     tzline->SetLineColor(2);
    tzline->Draw();
    d0->Modified();
    d0->Update();
    }
   cout << " Are gates ok?" << endl;
   cin >> ans ;
   }
    d0->Modified();
    d0->Update();
      d0->Print(tt);
}
  }
}
//d0->Print(ttclose);
 char fname[128];
 sprintf(fname,"new_gates.dat");
 cout << " Writing to file : " << fname << endl ;
 FILE *ff1 = fopen(fname,"w");
 for ( pl=0;pl<6;pl++ ) {
   for (wg=0;wg<35;wg ++) fprintf(ff1,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f\  \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
 }
fclose(ff1);
  cout << "Which plane (1-6) ( type 99 to quit)?" << endl ;
  cin >> vpl;
  vpl = vpl -1;
  }
//
//
  char fname[128];
  sprintf(fname,"new_gates.dat");
  cout << " Writing to file : " << fname << endl ;
FILE *ff1 = fopen(fname,"w");
for ( pl=0;pl<6;pl++ ) {
for (wg=0;wg<35;wg ++) fprintf(ff1,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f\  \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
}
fclose(ff1);

}
