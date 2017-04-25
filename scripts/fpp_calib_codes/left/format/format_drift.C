void format_drift() {

  ifstream dbfile("../new_drift_time_poly.dat");
  Float_t drift[24][7];

  Int_t pl;

  for( pl=0; pl<24; pl++ ){
    dbfile>>drift[pl][0]>>drift[pl][1]>>drift[pl][2]>>drift[pl][3]>>drift[pl][4]>>drift[pl][5]>>drift[pl][6];
  }

  dbfile.close();
  char fname[128];
  sprintf(fname,"format_drift.dat");
  FILE *ff= fopen(fname,"w");
  for ( pl=0;pl<24;pl++ ) {
    fprintf(ff,"%15.5e",drift[pl][0]);
    for (int ii=1;ii<7;ii++) 
      fprintf(ff,"%13.5e",drift[pl][ii]);
    
    if(pl==23)
      fprintf(ff,"\n");
    
    else 
      fprintf(ff,"\\ \n");
    
  }
  cout<<"Done!"<<endl;
}
