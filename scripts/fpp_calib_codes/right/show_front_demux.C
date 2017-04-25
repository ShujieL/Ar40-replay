#include <iostream>


Int_t show_front_demux(){
  // the histogram order is v1-3,u1-3,v4-6,u4-6,u7-8,v7-8,x1-2,u9-11,v9-11
  // This matchs the expected order in the database to the demux gates
  // In database there are assumed 35 wiregroups of 9 gates for each plane
  // Fill extra wiregroups for planes with < 35 wiregroups with zeroes 
  Int_t nwg_plane[24] = {20,20,20,22,22,22,20,20,20,22,22,22,30,30,30,30,31,31,35,35,35,35,35,35};
  Int_t nplane[24] = {1,2,3,1,2,3,4,5,6,4,5,6,7,8,7,8,1,2,9,10,11,9,10,11} ;
  char pltype[24] = {'v','v','v','u','u','u','v','v','v','u','u','u',,'u','u','v','v','x','x','u','u','u','v','v','v'};
  TString ss,sss,scut;
  Int_t runnum=4273;
  cout<<"Run number? "<<endl;
  cin>>runnum;
  cout<<"Processing Run "<<runnum<<endl;
  TString froot;
  froot = Form("~/xiaohui/GEp/rootfile/fppHVscan_%d.root",runnum);
  f = new TFile(froot);
  TTree *t1 = (TTree*)f->Get("T");                  //Get loaded tree 
    TH1F *demux[24][35];
    Int_t first_plane=1;
    Int_t last_plane=12;
    char chtype='front';
    for(Int_t j=first_plane-1; j<last_plane; j++) {
      for(Int_t i=0; i<nwg_plane[j]; i++) {
	cout << " Plane = " <<pltype[j] << nplane[j] << " wiregroup = " << i+1 << endl;
	ss = Form("demux_%c%d_%d",pltype[j],nplane[j],i+1);
	demux[j][i] = new TH1F (ss,ss,150,0,150);
	sss = Form("L.fpp.%c%d.width",pltype[j],nplane[j]);
	scut = Form("L.fpp.%c%d.wire==%d",pltype[j],nplane[j],i+1);
	t1->Project(ss,sss,scut);
      }
    }
    // read demux gates
    Float_t demux_gates[24][35][9] ;
    //FILE *ff = fopen("db_L.fpp.dat","r");
    ifstream dbfile("gates.dat");
    pl = 0;
    wg = 0;
    for( pl=0; pl<24; pl++) {
      //  cout << "Plane : " << pl+1 << " or " << pltype[pl] << nplane[pl] << endl ;
      for( wg=0; wg<35; wg++) {
	dbfile >> demux_gates[pl][wg][0] >> demux_gates[pl][wg][1] >> demux_gates[pl][wg][2] >> demux_gates[pl][wg][3] >> demux_gates[pl][wg][4] >> demux_gates[pl][wg][5]  >> demux_gates[pl][wg][6] >> demux_gates[pl][wg][7] >> demux_gates[pl][wg][8] ;
	// cout << demux_gates[pl][wg][0] << demux_gates[pl][wg][1] << demux_gates[pl][wg][2] << demux_gates[pl][wg][3] << demux_gates[pl][wg][4] << demux_gates[pl][wg][5]  << demux_gates[pl][wg][6] << demux_gates[pl][wg][7] << demux_gates[pl][wg][8] << endl ;
      }  
    }
    dbfile.close();
  //
    TString tt,ttopen,ttclose;
    ttopen = Form("demux_pl_%d_%d_run%d.ps[",first_plane,last_plane,runnum);
    tt = Form("demux_pl_%d_%d_run%d.ps",first_plane,last_plane,runnum);
    ttclose = Form("demux_pl_%d_%d_run%d.ps]",first_plane,last_plane,runnum);
    TCanvas *d0 = new TCanvas("d0","d0",1000,600);
    d0->Divide(2,6);
    d0->Print(ttopen);
    Int_t max_bin,max_content,ng;
    for (pl=first_plane-1;pl<last_plane;pl++){
      Int_t ii = 0;
      for (wg=0;wg<nwg_plane[pl];wg++){
	ii++;
	d0->cd(ii);
	demux[pl][wg]->Draw();
	d0->Modified();
	d0->Update();
	max_bin = demux[pl][wg]->GetMaximumBin();
	max_content = demux[pl][wg]->GetBinContent(max_bin);
	for (ng=0;ng<9;ng++) {
	  TLine *tzline = new TLine(demux_gates[pl][wg][ng],0,demux_gates[pl][wg][ng],max_content);
	  tzline->SetLineColor(2);
	  tzline->Draw();
	  d0->Modified();
	  d0->Update();
	}
	//cout << " ii = " << ii << endl;
	if ( ii == 12) {
	  //cout << " Print " << i << endl;
       d0->Print(tt);
       d0->Clear();
       d0->Divide(2,6);
       ii = 0;
	} 
      }
   d0->Print(tt);
   d0->Clear();
   d0->Divide(2,6);
    }
    d0->Print(ttclose);
}
