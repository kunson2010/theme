#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
  char p_name[32];		/* pitch 名 */
  char p_dis; 			/* 到p0的距离 */
} p_dis;  // one recored of pitch distance

p_dis p_dis_tab[17]={		/* table of all pitchs from p0 */
  {"C", 0},
  {"Dflat", 5},
  {"Csharp", 6},
  {"D", 4},
  {"Eflat", 6},
  {"Dsharp", 6},
  {"E", 3},
  {"F", 5},
  {"Gflat", 7},
  {"Fsharp", 6},
  {"G", 2},
  {"Aflat", 6},
  {"Gsharp", 7},
  {"A", 5},
  {"Bflat", 7},
  {"Asharp", 6},
  {"B", 4},
};


char measure_theme[64][32]; 	/* 每小节 */
char measure_var[64][32];

int get_dis(char * );

main(int argc, char** argv)
{
  int i;
  char mi=0;			/* number of measure */
  char mii=0;			/* number of 16th */
  FILE *fp_theme, *fp_var; 
  char str_theme[256];
  char str_var[256];
  char step[32]="";		/* setp */
  char acc[32]="";		/* accidental, 临时升降记号 */
  char dur[32]="";		/* duration, 音符的时长 */

  float dd=0;
  
  /* for(i=0; i<17; i++){ */
  /*   printf("%d, %s, %d\n", i, p_dis_tab[i].p_name, p_dis_tab[i].p_dis); */
  /* } */


  /* 1. Open 2 xml files */
  fp_theme = fopen("./K265-theme.xml", "r");
  fp_var   = fopen("./K265-varI.xml", "r");

  /* 1.1 read the first measure  */


  /* 1.1  read theme.xml */
  bzero(str_theme, sizeof(str_theme));

  mii=0;
  while ( NULL !=fgets(str_theme, 256, fp_theme) )  {
    bzero(measure_theme, 8*32);
    if(strstr(str_theme, "<measure")){ /* in measure */
      mi++; 
      
      bzero(str_theme, sizeof(str_theme));
      while ( NULL !=fgets(str_theme, 256, fp_theme) )  {

	  
	  if(strstr(str_theme, "<step")) { /* step */
	    sscanf(str_theme, "%*[^>]>%[^<]", step); 
	    printf("step=%s\n", step);
	  }
	  
	  if(strstr(str_theme, "<accidental")){
	    sscanf(str_theme, "%*[^>]>%[^<]", acc);
	    printf("acc=%s\n", acc); /* accidental */
	  }
	  
	  if(strstr(str_theme, "<duration")){
	    sscanf(str_theme, "%*[^>]>%[^<]", dur);
	    printf("duration=%s\n", dur); /* duration */
	  }

	
	if(strstr(str_theme, "</note")){ /* end of a node */
	  printf("16th notes num=%d\n", atol(dur)/64);
	  /* 1.1.1 fill in measure_theme */
	  for (i=0; i<atol(dur)/64;i++){
	    sprintf(measure_theme[mii+i],"%s%s", step, acc); /* --------累加, 但是前8被覆盖------ */
	  }
	  mii=mii+i; 		/* 有效记录数增加到新值 */
	  
	  /* for (i=0; i<mii; i++) { */
	  /*   printf("mesure[%d]=%s\n", i, measure_theme[i]); */
	  /* } */
	}
	
	if(strstr(str_theme, "</measure")){
	  break;
	}
      }
    } else if (strstr(str_theme, "</measure")) { 
      ;
    }
  }
  /* printf("mii=%d\n", mii); */
  /* 	  for (i=0; i<mii; i++) { */
  /* 	    printf("mesure[%d]=%s\n", i, measure_theme[i]); */
  /* 	  } */
  
  /* for(i=0;i<64;i++)  */
  /*   printf("measure_theme[%d]=%s\n", i, measure_theme[i]); */

  printf("%d measures found.\n", mi);
  printf("str_theme:%s\n", str_theme);


  /* 1.2 read var.xml */
  bzero(str_var, sizeof(str_var));

  mii=0;
  while ( NULL !=fgets(str_var, 256, fp_var) )  {
    bzero(measure_var, 8*32);
    if(strstr(str_var, "<measure")){ /* in measure */
      mi++; 
      
      bzero(str_var, sizeof(str_var));
      while ( NULL !=fgets(str_var, 256, fp_var) )  {
	  
	  
	  if(strstr(str_var, "<step")){ /* step */
	    sscanf(str_var, "%*[^>]>%[^<]", step); 
	    printf("step=%s\n", step);
	  }
	  
	  if(strstr(str_var, "<accidental")){
	    sscanf(str_var, "%*[^>]>%[^<]", acc);
	    printf("acc=%s\n", acc); /* accidental */
	  }
	  
	  if(strstr(str_var, "<duration")){
	    sscanf(str_var, "%*[^>]>%[^<]", dur);
	    printf("duration=%s\n", dur); /* duration */
	  }

	if(strstr(str_var, "</note")){ /* end of a node */
	  printf("16th notes num=%d\n", atol(dur)/64);
	  /* 1.1.1 fill in measure_var */
	    for (i=0; i<atol(dur)/64;i++){
	      sprintf(measure_var[mii+i],"%s%s", step, acc); /* --------累加, 但是前8被覆盖------ */
	      
	      
	    }
	    mii=mii+i; 		/* 有效记录数增加到新值 */
	    
	    /* for (i=0; i<mii; i++) { */
	    /*   printf("mesure[%d]=%s\n", i, measure_var[i]); */
	    /* } */
	    
	    
	}
	
	if(strstr(str_var, "</measure")){
	  break;
	}
      }
    } else if (strstr(str_var, "</measure")) { 
      ;
    }
  }
  /* printf("mii=%d\n", mii); */
  /* for (i=0; i<mii; i++) { */
  /*    printf("mesure[%d]=%s\n", i, measure_var[i]); */
  /* } */
  
  
  
  printf("%d measures found.\n", mi);
  printf("str_var:%s\n", str_var);
  
  strcpy(measure_theme[0],"C"); strcpy(measure_theme[1],"C"); strcpy(measure_theme[2],"C"); strcpy(measure_theme[3],"C");
  strcpy(measure_theme[4],"C"); strcpy(measure_theme[5],"C"); strcpy(measure_theme[6],"C"); strcpy(measure_theme[7],"C");
  strcpy(measure_theme[8],"G"); strcpy(measure_theme[9],"G"); strcpy(measure_theme[10],"G"); strcpy(measure_theme[11],"G");
  strcpy(measure_theme[12],"G"); strcpy(measure_theme[13],"G"); strcpy(measure_theme[14],"G"); strcpy(measure_theme[15],"G");
  strcpy(measure_theme[16],"A"); strcpy(measure_theme[17],"A"); strcpy(measure_theme[18],"A"); strcpy(measure_theme[19],"A");
  strcpy(measure_theme[20],"A"); strcpy(measure_theme[21],"A"); strcpy(measure_theme[22],"A"); strcpy(measure_theme[23],"A");
  strcpy(measure_theme[24],"G"); strcpy(measure_theme[25],"G"); strcpy(measure_theme[26],"G"); strcpy(measure_theme[27],"G");
  strcpy(measure_theme[28],"G"); strcpy(measure_theme[29],"G"); strcpy(measure_theme[30],"G"); strcpy(measure_theme[31],"G");
  strcpy(measure_theme[32],"F"); strcpy(measure_theme[33],"F"); strcpy(measure_theme[34],"F"); strcpy(measure_theme[35],"F");
  strcpy(measure_theme[36],"F"); strcpy(measure_theme[37],"F"); strcpy(measure_theme[38],"F"); strcpy(measure_theme[39],"F");
  strcpy(measure_theme[40],"E"); strcpy(measure_theme[41],"E"); strcpy(measure_theme[42],"E"); strcpy(measure_theme[43],"E");
  strcpy(measure_theme[44],"E"); strcpy(measure_theme[45],"E"); strcpy(measure_theme[46],"E"); strcpy(measure_theme[47],"E");
  strcpy(measure_theme[48],"D"); strcpy(measure_theme[49],"D"); strcpy(measure_theme[50],"D"); strcpy(measure_theme[51],"D");
  strcpy(measure_theme[52],"D"); strcpy(measure_theme[53],"D"); strcpy(measure_theme[54],"D"); strcpy(measure_theme[55],"E");
  strcpy(measure_theme[56],"C"); strcpy(measure_theme[57],"C"); strcpy(measure_theme[58],"C"); strcpy(measure_theme[59],"C");
  strcpy(measure_theme[60],"C"); strcpy(measure_theme[61],"C"); strcpy(measure_theme[62],"C"); strcpy(measure_theme[63],"C");
  
  strcpy(measure_var[0],"D"); strcpy(measure_var[1],"C"); strcpy(measure_var[2],"B"); strcpy(measure_var[3],"C");
  strcpy(measure_var[4],"B"); strcpy(measure_var[5],"C"); strcpy(measure_var[6],"B"); strcpy(measure_var[7],"C");
  strcpy(measure_var[8],"A"); strcpy(measure_var[9],"G"); strcpy(measure_var[10],"Fsharp"); strcpy(measure_var[11],"G");
  strcpy(measure_var[12],"Fsharp"); strcpy(measure_var[13],"G"); strcpy(measure_var[14],"Fsharp"); strcpy(measure_var[15],"G");
  strcpy(measure_var[16],"Gsharp"); strcpy(measure_var[17],"A"); strcpy(measure_var[18],"C"); strcpy(measure_var[19],"B");
  strcpy(measure_var[20],"D"); strcpy(measure_var[21],"C"); strcpy(measure_var[22],"B"); strcpy(measure_var[23],"A");
  strcpy(measure_var[24],"A"); strcpy(measure_var[25],"G"); strcpy(measure_var[26],"E"); strcpy(measure_var[27],"D");
  strcpy(measure_var[28],"C"); strcpy(measure_var[29],"B"); strcpy(measure_var[30],"A"); strcpy(measure_var[31],"G");
  strcpy(measure_var[32],"G"); strcpy(measure_var[33],"F"); strcpy(measure_var[34],"D"); strcpy(measure_var[35],"C");
  strcpy(measure_var[36],"B"); strcpy(measure_var[37],"A"); strcpy(measure_var[38],"G"); strcpy(measure_var[39],"F");
  strcpy(measure_var[40],"F"); strcpy(measure_var[41],"E"); strcpy(measure_var[42],"C"); strcpy(measure_var[43],"B");
  strcpy(measure_var[44],"A"); strcpy(measure_var[45],"G"); strcpy(measure_var[46],"F"); strcpy(measure_var[47],"E");
  strcpy(measure_var[48],"D"); strcpy(measure_var[49],"D"); strcpy(measure_var[50],"A"); strcpy(measure_var[51],"A");
  strcpy(measure_var[52],"G"); strcpy(measure_var[53],"G"); strcpy(measure_var[54],"B"); strcpy(measure_var[55],"B");
  strcpy(measure_var[56],"C"); strcpy(measure_var[57],"C"); strcpy(measure_var[58],"C"); strcpy(measure_var[59],"C");
  strcpy(measure_var[60],"C"); strcpy(measure_var[61],"C"); strcpy(measure_var[62],"C"); strcpy(measure_var[63],"C");

  for(i=0;i<64;i++) {
    printf("measure_theme[%d]=%s\n", i, measure_theme[i]); 
    printf("measure_var[%d]=%s\n", i, measure_var[i]); 
  }

  dd=0;
  for(i=0; i<64; i++) {
    if(strcmp(measure_theme[i], measure_var[i])!=0){
      dd += abs( get_dis(measure_var[i]) - get_dis(measure_theme[i]))* 0.125;
    }
  }

  printf("\n\n\n\ndistainc of these two pieces of music is  -------->%f\n", dd);
  fclose(fp_theme);
  fclose(fp_var);
  return 0;
}

  
/* input: pitch name */
/* return: it's pitch distance */
int get_dis(char * pName)
{
  int dis=0;
  int i;

  for(i=0; i<17; i++){
    if(strcmp(pName, p_dis_tab[i].p_name)==0)
      return p_dis_tab[i].p_dis;
  }

  return 0;
}
