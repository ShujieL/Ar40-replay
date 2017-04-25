void format_gates(){
  
  ifstream dbfile("../new_gates.dat");
  Float_t demux_gates[24][35][9];
  Int_t pl,wg;
  pl=0;
  wg=0;
  
  for( pl=0; pl<24; pl++){
    for( wg=0; wg<35; wg++){
      
      dbfile >> demux_gates[pl][wg][0] >> demux_gates[pl][wg][1] >> demux_gates[pl][wg][2] >> demux_gates[pl][wg][3] >> demux_gates[pl][wg][4] >> demux_gates[pl][wg][5]  >> demux_gates[pl][wg][6] >> demux_gates[pl][wg][7] >> demux_gates[pl][wg][8] ;
      
    }
  }
  dbfile.close();
  char fname[128];
  sprintf(fname,"format_gates.dat");
  cout<<"reformat database to: "<<fname<<endl;
  FILE *ff= fopen(fname,"w");
  for ( pl=0;pl<24;pl++ ) {
    if(pl!=23)
      for (wg=0;wg<35;wg ++) fprintf(ff,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f\\  \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
    else
      for (wg=0;wg<35;wg ++) {
	
	if(wg!=34)
	  fprintf(ff,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f\\  \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
	else
	  fprintf(ff,"%8.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f%7.2f  \n",demux_gates[pl][wg][0],demux_gates[pl][wg][1],demux_gates[pl][wg][2],demux_gates[pl][wg][3],demux_gates[pl][wg][4],demux_gates[pl][wg][5],demux_gates[pl][wg][6],demux_gates[pl][wg][7],demux_gates[pl][wg][8]);
      }
  }
  fclose(ff);
  cout<<"Done! Copy the formatted file to the database."<<endl;
}
