double CalcCharge(int run, TString kin, TString dev) {
    //const char* ROOTfilePath = gSystem->ExpandPathName(Form("/work/halla/gmp12/longwu/gmp_analysis/rootfiles/Fall2016/%s",kin.Data()));
    const char* ROOTfilePath = gSystem->ExpandPathName(Form("/adaqfs/home/a-onl/gmp/replay/Rootfiles",kin.Data()));
    TString basename = Form("right_gmp_%d",run);
    TString rootfile = basename + ".root";

    TChain* T = new TChain("T");

    Long_t split=0;
    char* file = 0;
    while (file = gSystem->Which(ROOTfilePath,rootfile)) {
	T->Add(file);
	cout << "ROOT file " << rootfile << " added to TChain." << endl;
	split++;
	rootfile = basename + "_" + split + ".root";
    }

    if (split==0) {
	cerr << "The specified run does not exist or has not been replayed." << endl;
	return;
    }

    const int ndev = 6;
    const TString bcm[] = { "u1", "d1", "d3", "d10", "unew", "dnew" };
    Double_t isrenewed;
    Double_t charge[ndev], totalcharge[ndev], lastcharge[ndev];
    Double_t current[ndev];
    Double_t clkcountr;

    T->SetBranchStatus("*",0);
    //for (int i=0; i<ndev; i++)
    //	cout << "Branch Status: " << Form("N.evright.charge_%s",bcm[i]) << " " << T->GetBranchStatus(Form("N.evright.charge_%s",bcm[i])) << endl;
    //T->SetBranchStatus("N.evright.isrenewed", 1);
    T->SetBranchStatus("N.evright.isrenewed", 1);

    T->SetBranchStatus("evright_clkcountr",1);
    T->SetBranchAddress("evright_clkcountr",&clkcountr);

    for (int i=0; i<ndev; i++) {

	totalcharge[i] = 0;
	lastcharge[i] = 0;

    	//T->SetBranchStatus(Form("N.evright.charge_%s",bcm[i].Data()), 1); 
    	//T->SetBranchAddress(Form("N.evright.charge_%s",bcm[i].Data()), charge+i); 
    	T->SetBranchStatus(Form("N.evright.charge_%s",bcm[i].Data()), 1); 
    	T->SetBranchAddress(Form("N.evright.charge_%s",bcm[i].Data()), charge+i); 
   	//T->SetBranchStatus(Form("N.evright.current_%s",bcm[i].Data()), 1); 
    	//T->SetBranchAddress(Form("N.evright.current_%s",bcm[i].Data()), current+i); 
   	T->SetBranchStatus(Form("N.evright.current_%s",bcm[i].Data()), 1); 
    	T->SetBranchAddress(Form("N.evright.current_%s",bcm[i].Data()), current+i); 

    }
  
    //T->Draw(">>elist","N.evright.isrenewed==1","entrylist");
    T->Draw(">>elist","N.evright.isrenewed==1","entrylist");

    TEntryList* elist = (TEntryList*)gDirectory->Get("elist");

    Long64_t listentries = elist->GetN();
    T->SetEntryList(elist);

//    Long64_t listentries = T->GetEntries();

    for (Long64_t entry=0; entry<listentries; entry++) {
	Long64_t entryNumber = T->GetEntryNumber(entry);
	if (entry<0) break;

	T->GetEntry(entryNumber);

	for (int j=0; j<ndev; j++) { 
            assert(clkcountr>0);
	    if (current[j]<=3) {
	        lastcharge[j] = charge[j];
		continue;
	    }
	    totalcharge[j] += charge[j]-lastcharge[j];
	    lastcharge[j] = charge[j];
	}
    }

    Double_t totcharge=0;
    cout << "Summary of beam charge in run " << run << ": " << endl;
    for (int i=0; i<ndev; i++) {
	cout << bcm[i] << ": " << totalcharge[i] << " uC" << endl;
    	if (dev.CompareTo(bcm[i],TString::kIgnoreCase) == 0) 
	    totcharge = totalcharge[i];
    }

    //ch = totcharge;
    return totcharge;
}
