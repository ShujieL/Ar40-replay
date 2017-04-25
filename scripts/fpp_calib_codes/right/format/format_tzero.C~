void format_tzero(){
 
  ifstream dbfile("./tmp_tzero.dat");
  Float_t tzero[24][40];

  Int_t pl, wg;
  pl=0;
  wg=0;
  for( pl=0; pl<6; pl++) {
    for( wg=0; wg<40; wg++)
      dbfile >> tzero[pl][wg];
  }
  
  dbfile.close();

  FILE *ff = fopen("format_tzero.dat","w");
  for (int i=0;i<6;i++){
    if(i!=23)
    for (int ii=0;ii<40;ii++) fprintf(ff," %6.1f\\ \n",tzero[i][ii]);
    else 
      for (int ii=0;ii<40;ii++) {
	if(ii!=39)
	  fprintf(ff," %6.1f\\ \n",tzero[i][ii]);
	else
	  fprintf(ff," %6.1f \n",tzero[i][ii]);
      }
  }
  fclose(ff);
  cout<<"Done! Copy the formatted file to the database!"<<endl;
}

