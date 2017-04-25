//////////////////////////////////////////////////////////////////////////
//
// Bigbite lib replay script definition header
//
// https://hallaweb.jlab.org/wiki/index.php/ReplayCore
//
// It is suggested to use this file set replay environmental variable for 
// different machine. So, they can use same replay script
//	
//////////////////////////////////////////////////////////////////////////
//	
//	Author : Jin Huang (jinhuang@jlab.org)    Aug 2007
//	Modify History:
//
//      27 Mar 08 ole  Change ROOT file names for DetL and PhysL replays
//                     from _det_nnnn to _det_L_nnnn etc.
//
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_REPLAY_DEF
#define ROOT_REPLAY_DEF


//////////////////////////////////////////////////////////////////
// extra structures
//////////////////////////////////////////////////////////////////

#include "TObject.h" //trick to avoid error: `Int_t' does not name a type

typedef struct _sReplaySetUp
{
    Int_t DefReplayNum; //defaut replay event num
    char* OutFileFormat;//output file format
    char* OutDefineFile;//out define
    char* CutDefineFile;//cut define
}sReplaySetUp;


//////////////////////////////////////////////////////////////////
// common defines
//////////////////////////////////////////////////////////////////

static const char* PATHS[] = {
  //  "/mss/halla/Ar40/raw",
  "/v/lustre2/expphy/cache/halla/Ar40/raw",
  "/cache/halla/Ar40/raw",
   0
};

static const char* RAW_DATA_FORMAT="%s/Ar40_%d.dat.%d";
//static const char* RAW_DATA_FORMAT="%s/dvcs14_%d.dat.%d";

static const char* STD_REPLAY_OUTPUT_DIR="../rootfiles";
static const char* CUSTOM_REPLAY_OUTPUT_DIR="../ScratchROOTfiles";

static const char* SUMMARY_PHYSICS_FORMAT="../summaryfiles/summaryphy_%d.log";

//not used since Nov 15, 2008
static const Int_t ANA_MARK_INTERVAL=1000;

// #include "ReplayCore.C"  

// DO NOT DELETE!
#endif//#define ROOT_REPLAY_DEF

