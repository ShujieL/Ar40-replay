//=======================
// DeadTime Calculation
//Dien 02/23/2017
//Modified from Sheren code_Deadtime
//To add Chain to chain all rutfiles
//add THarun class to get the PS automatically for everyrun

//============================================================


void deadtime(){

  int run; 
  const TString rootfilePath = "/adaqfs/home/a-onl/tritium/replay/RootFiles/";
  int PS[8];
  char *rate = new char[50];
  char hname[8][50];
  char *h = new char[50];
  Double_t LT[8], DT[8];
  int icount[8];
  int daqcount[8];
  stringstream ss;
  TH1F *his[8];

  // ifstream infile; infile.open("runlist.dat");
  // ofstream outfile; outfile.open("Deadtime_table_Ar.dat");
  cout << "\nreplay: Please enter a Run Number (-1 to exit):";
  cin >> run;
  if(run==-1) return;
  
  //ifstream infile; infile.open("Ar_run.dat");
  ofstream outfile; 
 
  outfile.open("DT_Ar.dat",ios::in);
  // cout<<outfile.is_open();
  
  ss.clear();
  ss.str("");
  
  ss << setiosflags(ios::left) << setw(5) << "run:  "  << "     ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T1  " << "       ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T2  " << "       ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T3  " << "       ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T4  " << "       ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T5  " << "       ";
  ss << setiosflags(ios::left) << setw(5) << "DT_T6  " << "-----in %----"<<endl;


  if(!outfile.good()){
    outfile.close();
    outfile.open("DT_Ar.dat");
    outfile<<ss.str();
  
 
  }
  outfile.close();
  outfile.open("DT_Ar.dat",ios::out|ios::app);


  // while(true){

  //  infile >> run;

  //  if(infile.eof())break; 


  TChain *T = new TChain("T");

  std::ostringstream str;

  str << rootfilePath<<"coinc_test_"<<run;

  TString basename = str.str().c_str();
  TString rootfile = basename + ".root";

  cout<< " check the rootfile : "<< rootfile<<endl;

  TFile *file = new TFile(rootfile,"r");
  if(file->IsZombie()){
    //    cout<<" this rootfile doest not exist: "<<endl;
    cout<<" !!!full replay does not exist!!!"<<endl;
    str.clear();
    str.str("");
    str << rootfilePath<<"coinc_online_"<<run;
    basename = str.str().c_str();
    rootfile = basename + ".root";
    file->Close("R");
    delete file;
    TFile *file = new TFile(rootfile,"r");
    if(file->IsZombie()){
      cout<<" !!!online replay does not exist either. Stop!!!"<<endl;
    return();
    }
  }

  THaRun *aRun = (THaRun*)file->Get("Run_Data");
  TArrayI ps = aRun->GetParameters()->GetPrescales();

  for (int k=0; k<8; k++){
    PS[k]= ps[k];
    cout<< " Check the prescaler: "<< PS[k]<<endl;
  }

   

  //====adding splits rootfiles =======================//

  Long_t j=0;
  while ( !gSystem->AccessPathName(rootfile.Data()) ) {
    // LEFT->Add(rootfile.Data());
    T->Add(rootfile.Data());

    cout << "ROOT file " << rootfile << " added to TChain." << endl;
    j++;
       
    rootfile = basename + "_" +j  + ".root";
  }
      

   
  T->Draw("DR.scalLive1/DR.scalLive2>>hh");
  Double_t live_scaler = hh->GetMean();
  cout<<"livetime from scalers="<<live_scaler<<endl;
  
  for (int i=0; i<8; i++){
    TCut t_cut = Form("DR.evtypebits&(1<<%i)",i+1);
    sprintf(rate, "right_t%ic",i+1);
    icount[i] = T->GetMaximum(rate);
    sprintf(hname[i],"t%i",i+1);
    sprintf(h,"DR.evtypebits>>%s", hname[i]);
    his[i] =new TH1F (hname[i], hname[i], 100,0,1000000);
    T->Draw(h,t_cut, "same");
    daqcount[i] = his[i]->GetEntries();
    //  cout<<daqcount[i]<<endl;
    if(icount[i]>0){
      LT[i] = 100.*PS[i]*daqcount[i]/icount[i];
      DT[i] = 100 - LT[i];
      if(DT[i]<0){
	cout<<"Negative DT! Not enough statistics, try a full replay! "<<endl;

      }
      //  cout<<" T "<<setw(10)<<"LiveTime"<<setw(10)<<"DeadTime"<<endl;
      //  cout<<i+1<<setw(10)<<LT[i]<<setw(10)<<DT[i]<<endl;	
    }}

  cout<<ss.str();

  ss.clear();
  ss.str("");
  ss  << setw(5) << run  << "      " ;
  ss << setiosflags(ios::left) << setw(5) << DT[0]<<"     ";
  ss << setiosflags(ios::left) << setw(5) << DT[1]<<"      " ;
  ss << setiosflags(ios::left) << setw(5) << DT[2]<<"       ";
  ss << setiosflags(ios::left) << setw(5) << DT[3]<<"        ";
  ss << setiosflags(ios::left) << setw(5) << DT[4]<<"        ";
  ss << setiosflags(ios::left) << setw(5) << DT[5]<<endl;

  cout<<ss.str();
  outfile<<ss.str();
    
  outfile.close();
  cout<<"----run "<<run<<" deadtime calculated. Check DT_Ar.dat"<<endl;
  exit();

}

