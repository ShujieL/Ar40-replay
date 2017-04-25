# Ar40-replay
##Replay code for Ar40. 
###usage: 
```
relay_coinc.C(RUN NUMBER)
```

###For the first time user, please
1. recompile libraries
```
cd libraries; ./compile.sh  
```
2. in def_coinc.h, setup the following input/output directories as you like: 
```
static const char* PATHS[]
```
```
static const char* STD_REPLAY_OUTPUT_DIR="../rootfiles";
static const char* CUSTOM_REPLAY_OUTPUT_DIR="../ScratchROOTfiles";
static const char* SUMMARY_PHYSICS_FORMAT="../summaryfiles/summaryphy_%d.log";
```
3. check your environment varialbles such as $DB_DIR, $ANALYZER
4. reload ReplayCore64.C in ANALYZER:
```
[111@111 replay]$ analyzer
analyzer [0] .L ReplayCore64.C+
```
5. Now if you reopen ANALYZER, you should not see any error message.

