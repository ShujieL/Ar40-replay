#include <iostream>


Int_t tzero_calc(){
  TString ss,sss,scut;
  char pltype[24] = {'v','v','v','u','u','u','v','v','v','u','u','u',,'u','u','v','v','x','x','u','u','u','v','v','v'};
  Int_t runnum;
  cout<<"Run number? : ";
  cin>>runnum;
  TString froot;

froot = Form("/adaqfs/home/a-onl/gmp//replay/Rootfiles/right_gmp_%d.root",runnum);

  TChain eff("T");
  eff.Add(froot);
  // TTree *t1 = (TTree*)f->Get("T");                  //Get loaded tree 
  TH1F *tdc[24][35]; 
  // the histogram order is v1-3,u1-3,v4-6,u4-6,u7-8,v7-8,x1-2,u9-11,v9-11
  // This matchs the expected order in the database to the tzero offsets
  // The histogram is in units of 0.5ns
  // The database expects the tzero offset in ns
  Int_t nwg_plane[24] = {20,20,20,22,22,22,20,20,20,22,22,22,30,30,30,30,31,31,34,34,34,34,34,34};
  Int_t nplane[24] = {1,2,3,1,2,3,4,5,6,4,5,6,7,8,7,8,1,2,9,10,11,9,10,11} ;
  Int_t pl,wg,k;
  Int_t hbinmax=1800;
  Int_t hbinmin=1000;
  Int_t hbinnum=800;
  // Get old tzero from file  
  // 40 tzero per 24 planes.
  Float_t old_tzero[24][40];
  Float_t new_tzero[24][40];
  ifstream dbfile("old_tzero.dat");
  pl = 0;
  wg = 0;
  cout << " Get old tzero offsets for planes " << endl;
  for( pl=0; pl<6; pl++) {
    cout << pltype[pl] << nplane[pl] << " "  ;
    for( wg=0; wg<40; wg++)  { dbfile >> old_tzero[pl][wg] ;
      cout << "Plane : " << pl+1 << "Wiregroup" << wg+1 << " Old tzero = " << old_tzero[pl][wg] << endl ;
      new_tzero[pl][wg] = old_tzero[pl][wg] ;
    }
  }
  cout << endl ;
  dbfile.close();
  //
  // fill histograms
  //for( pl=0; pl<24; pl++) {
  // cout << "Plane : " << pl+1 << endl << "Wiregroup" << endl ;
 //for( wg=0; wg<nwg_plane[pl]; wg++) {
 //     k=wg+1;  
 //   ss = Form("tdc%c%d_%d",pltype[pl],nplane[pl],k);
 //   tdc[pl][wg] = new TH1F (ss,ss,hbinnum,hbinmin,hbinmax);
 //   sss = Form("L.fpp.%c%d.ltdc",pltype[pl],nplane[pl]);
 //   scut = Form("L.fpp.%c%d.wire==%d",pltype[pl],nplane[pl],k);
 //   t1->Project(ss,sss,scut);
 //   cout << k << endl ;
 //}
 //}
  // Create ps file
  TString tt,ttopen,ttclose;
  ttopen = Form("tzero_%d.ps[",runnum);
  tt = Form("tzero_%d.ps",runnum);
  ttclose = Form("tzero_%d.ps]",runnum);
  // create canvas and plot raw tdc histo and determine tzero offset
  // write tzero offsets in new_tzero.dat
  TCanvas *d0 = new TCanvas("d0","d0",800,500);
  d0->Divide(1,1);
  d0->Print(ttopen);
  Int_t max_bin,max_content,kkk,ii,iii;
  Float_t tzero;
  char test ;
  pl = 0 ;
  while ( pl < 6 &&  pl >= 0 ) {
    // value of "min" may need to be changed depending on the run.
    Float_t min = 5;
    char ans;
    cout << " Do plane (1-6 ; enter 25 to quit) (last plane : "<< pl+1  << "): ";
    cin >> pl;
    pl = pl-1 ;
    if ( pl <  6 && pl >= 0 ) {
      for (wg=0;wg<nwg_plane[pl];wg++){
	ss = Form("tdc%c%d_%d",pltype[pl],nplane[pl],wg+1);
	tdc[pl][wg] = new TH1F (ss,ss,hbinnum,hbinmin,hbinmax);
	sss = Form("R.fpp.%c%d.ltdc",pltype[pl],nplane[pl]);
	scut = Form("R.fpp.%c%d.wire==%d",pltype[pl],nplane[pl],wg+1);
	//t1->Project(ss,sss,scut);
	eff.Project(ss,sss,scut);
	d0->cd(1);
	tdc[pl][wg]->Draw();
	max_bin = tdc[pl][wg]->GetMaximumBin();
	max_content = tdc[pl][wg]->GetBinContent(max_bin);
	min = max_content*.1;
	if ( min < 5) min = 5;
	d0->Modified();
	d0->Update();
	kkk=hbinmax-hbinmin+1;
	Int_t found_bin =0;
	while ( found_bin == 0 && kkk > 8 ) {
	  kkk=kkk-1;
	  Int_t sum = 0;
	  for (Int_t t=kkk-8;t<kkk;t++) {
	    Int_t num = tdc[pl][wg]->GetBinContent(t);
	    sum = sum + num;
	    //     cout << " t = " << t << " num = " << num << " sum = " << sum << endl;
	  }
	  Float_t ave = sum/8.;
	  if ( ave >= min ) { 
	    found_bin = tdc[pl][wg]->GetBin(kkk);
	    kkk = 0;
	  }
	  //    cout << " found_bin = " << found_bin << " ave = " << ave  << endl;
	}
	if ( found_bin == 0 ) found_bin = 650 ;
	TLine *tzline = new TLine(found_bin+hbinmin,0,found_bin+hbinmin,max_content);
	tzline->SetLineColor(2);
	tzline->Draw();
	TLine *tzline = new TLine(2*new_tzero[pl][wg],0,2*new_tzero[pl][wg],max_content);
	tzline->SetLineColor(4);
	tzline->Draw();
	d0->Modified();
	d0->Update();
	ans = 'y';
	cout << " New tzero (ns) ( red line) (chan = 0.5ns) = " << (found_bin+hbinmin)/2. << " Old val (blue line)=" << new_tzero[pl][wg] << endl;
	cout << " Use old value, new value or type in new value   (o/n/t)?" << endl;
	cin >> ans;
	while ( ans != 'q' ) {
	  switch ( ans) {
	  case 'o' :
	    ans= 'q';
	    break;
	  case 'n' :
	    new_tzero[pl][wg] =(found_bin+hbinmin)/2.;
	    ans= 'q';
	    break;
	  case 't' :
	    new_tzero[pl][wg] =(found_bin+hbinmin)/2.;          
	    cout << " Enter value for tzero ( in channel number) Present value : " << new_tzero[pl][wg]*2. << endl ;
	    cin >> found_bin ;
	    tdc[pl][wg]->Draw();     
	    TLine *tzline = new TLine(2*new_tzero[pl][wg],0,2*new_tzero[pl][wg],max_content);
	    tzline->SetLineColor(4);
	    tzline->Draw();
	    found_bin = found_bin - hbinmin ;
	    TLine *tzline = new TLine(found_bin+hbinmin,0,found_bin+hbinmin,max_content);
	    tzline->SetLineColor(2);
	    tzline->Draw();
	    d0->Modified();
	    d0->Update();
	    cout << " New tzero (ns) ( red line) (chan = 0.5ns) = " << (found_bin+hbinmin)/2. << " Old val (blue line)=" << new_tzero[pl][wg] << endl;
	    cout << " Use old value, new value or type in new value   (o/n/t)?" << endl;
	    cin >> ans;
	    break;
	  default :
	    cout << " New tzero (ns) (chan = 0.5ns) = " << (found_bin+hbinmin)/2. << " Old val =" << new_tzero[pl][wg] << endl;
	    cout << " Use old value, new value or type in new value   (o/n/t)?" << endl;
	    cin >> ans;
	    break;
	  }
	}
	d0->Modified();
	d0->Update();
	d0->Print(tt);
	FILE *ff = fopen("new_tzero.dat","w");
	for (ii=0;ii<6;ii++){
	  for (iii=0;iii<40;iii++) fprintf(ff," %6.1f \n",new_tzero[ii][iii]);
	}
	fclose(ff);
      }
    } 
    //    else if ( pl !=6 ) pl = 1;
    // cout << " plane = " << pl << endl;
    
 }
 //
d0->Print(ttclose);
}
