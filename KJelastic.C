#include <iostream> 
#include <fstream>
#include <cmath> 
#include <math.h> 
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMinuit.h>
#include <TPaveText.h>
#include <TText.h>
#include <TSystem.h>
#include <TArc.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLeaf.h>

// .x KJelastic.C(12751)  12751: elastic C12(single foil) run

// C. Hyde ----
// W - M(12C) = \sqrt{M(12C)^2 + 2M(12C)(k-k') - Q^2}
// I probably applied some nominal electron id cut and HRS acceptance cut.
// 7MeV off due to im-precise HRS central momentum (See Doug's email 06/05)
//  http://hallaweb.jlab.org/publications/Technotes/files/2001/01-049.pdf



// HRS theta = 16.629 (deg)
// E0(set) = 2.300GeV/c
// E0 = 2.281GeV/c


int KJelastic(int run) {
    //    gSystem->Load("libDVCS.so");
    gStyle->SetPalette(1);
    gStyle->SetOptStat(1);

    //  carbon elastic event analysis (particular Run 372)

    if(gSystem->AccessPathName(Form("./RootFiles/coinc_test_%d.root",run))) {
	cout << "Run not analyzed yet. Please run the replay_DVCS first." << endl;
	return 1;
    }

    //  Re-define the T as tree
    /* TTree *tree=(TTree*)file->Get("T"); */

    TChain *tree = new TChain("T");
    tree->Add(Form("./RootFiles/coinc_test_%d.root",run));
    
    Int_t split=1;
    while(!gSystem->AccessPathName(Form("./RootFiles/coinc_test_%d_%d.root",run,split))) tree->Add(Form("./RootFiles/coinc_test_%d_%d.root",run,split++));
    cout << "split file =" << split << endl;
    

    Double_t Mprot = 0.93827204;
    Double_t EBeam = 2.222;
    Double_t Ep,Ee,Pe,ThetaD_e,Theta_e,PhiD_e,Phi_e;
    Double_t kPx,kPy,kPz,kP2;
    Double_t MM=0;
    Double_t NumTrack;
    //    Double_t CMass = 0.9315*12.; // 931.5MeV times 12 for C
    Double_t CMass =  0.9315*26;

    Double_t L_tr_n,L_tr_px,L_tr_py,L_tr_pz;

    Double_t PionRejOne;
    Double_t PionRejTwo;

    tree->SetBranchStatus("L.tr.n",1);
    tree->SetBranchStatus("L.prl1.asum_c",1);
    tree->SetBranchStatus("L.prl2.asum_c",1);

    tree->SetBranchAddress("L.tr.n",&NumTrack);
    tree->SetBranchAddress("L.prl1.asum_c",&PionRejOne);
    tree->SetBranchAddress("L.prl2.asum_c",&PionRejTwo);

    // Obtain the number of event
    cout<<tree->GetEntries()<<endl;
    Int_t nevnt;
    nevnt = tree->GetEntries();
    tree->GetEntry(0);
 
    // Define the Histograms
    Int_t hid;
    TH1F *myhist[10000]; 
    char *histname = new char[10];
    for(Int_t ih=0;ih<1;ih++){
	hid = 1000+10*(ih);
	sprintf(histname,"histo_%d",hid); 
	myhist[hid]=new TH1F(histname,"13Al Energy Spectra (W-M[12C])",300, 2.05, 2.15);
    } 


    // fill histogram and calculation through event-by-event
    // for (int i=0;i<nevnt;i++) {
    for (int i=0;i<1000;i++) {
	tree->GetEntry(i); // to get i-th event
	if (i%3000==0)  cout<<Form("Processed event (i)=%d",i)<< endl;
	Double_t W2 = tree->GetLeaf("EKL.W2")->GetValue(0);
	Double_t Q2 = tree->GetLeaf("EKL.Q2")->GetValue(0);
	Double_t Nu = tree->GetLeaf("EKL.nu")->GetValue(0);
	Double_t Eps = tree->GetLeaf("EKL.epsilon")->GetValue(0);

	kP2 = EBeam*EBeam - (W2+Q2-Mprot*Mprot)/(2*Mprot);
    	Ep = TMath::Sqrt(kP2+Mprot*Mprot);
	Ee = EBeam + Mprot + Ep;
	MM = Ee*Ee-kP2;

	//   W - M(12C) = \sqrt{M(12C)^2 + 2M(12C)(k-k') - Q^2}
	Double_t Diff_Inv_M = sqrt(CMass*CMass + 2*CMass*Nu - Q2) - CMass;

	if(Nu>0){
	//	cout << i << " missing mass square =" << MM << endl;
	    for(int ih=0;ih<1;ih++){
		hid = 1000+10*(ih);
		if(NumTrack<2 && PionRejOne > 600.){
		    if(PionRejTwo > 250.){
			myhist[hid]->Fill(Diff_Inv_M);
			cout << Diff_Inv_M << endl;
		    }
		}
	    }
	}


    }

    
    // Create the TCanvas 
    Int_t hhid;
    TCanvas *c1[2000];
    c1[0]=new TCanvas();
    //    TBrowser b; // Open TBrowser in external window
    gPad->SetLogz(1);
    
//
    for(Int_t j=0;j<1;j++){
	Int_t fid  = 1000+10*(j);
	c1[0] = new TCanvas(); 
	myhist[fid]->Draw("same"); 
	gPad->SetLogy(1); 
	
    }
    
    return 0;
        
}
