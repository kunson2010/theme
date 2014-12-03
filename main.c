#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define Alpha 0.5
#define Beta  0.5


typedef struct{
    char p_name[32];            /* pitch 名 */
    char p_dis;                 /* 到p0的距离 */
} p_dis;  // one recored of pitch distance

p_dis p_dis_tab[17]={           /* table of all pitchs from p0 */
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


char measure_theme[64][32];     /* 主题的全部纪录， 以16th为一个纪录 */
char measure_var[64][32];       /* 变奏的全部纪录， 以16th为一个纪录 */

int get_dis(char * );

main(int argc, char** argv)
{
    FILE *fp_theme, *fp_var; 
    char str_theme[256];
    char str_var[256];

    int i;
    char mi=0;                  /* number of measure */
    char mii=0;                 /* number of 16th */
    
    char step[32]="";           /* setp 音名 */
    char acc[32]="";            /* accidental, 临时升降记号 */
    char dur[32]="";            /* duration, 音符的时长 */
    char acc_in_measure=0;      /* 小节内有无临时升降号 */
    char acc_step[32]="";       /* 升降的音名 */

    long El_var=0;             /* var 每次音符分解的 Edit length */
    long El_theme=0;           /* theme   每次音符分解的 Edit length */
    float dd=0;                /* pitch_dis*duration */
  

    /* 1  Open 2 xml files */
    fp_theme = fopen("./K265-theme.xml", "r");
    if( fp_theme == NULL ) {
        printf("open file error.\n");
        return -1;
    }


    fp_var   = fopen("./K265-varI.xml", "r");
    if( fp_var == NULL ) {
        printf("open file error.\n");
        return -1;
    }
    


    /* 2  read theme.xml */
    bzero(str_theme, sizeof(str_theme));

    mi=0; mii=0; 
    bzero(measure_theme, 64*32);
    while ( NULL !=fgets(str_theme, 256, fp_theme) ) {

        if(strstr(str_theme, "<measure")) { /* 一小节内 */
            mi++; 
      
            bzero(str_theme, sizeof(str_theme));
            while ( NULL !=fgets(str_theme, 256, fp_theme) )  { /* 小节内读入行 */

                if(strstr(str_theme, "<step")) { /* 音名 */
                    sscanf(str_theme, "%*[^>]>%[^<]", step); 
                    if(acc_in_measure==1) {     /* 如果有升降音符，
                                                   那么把与升降音同名的音符改为升降音 */
                        if (step[0] == acc_step[0]) {   /* 如果与升降音同音名 */
                            strcpy(step, acc_step);
                        }
                    }
                    
                    /* printf("step=%s\n", step); */
                    continue;
                }
          
                if(strstr(str_theme, "<accidental")){ /* 临时升降号 */
                    acc_in_measure=1;                 /* 小节内有升降号 */
                    sscanf(str_theme, "%*[^>]>%[^<]", acc);
                    /* printf("acc=%s\n", acc); /\* accidental *\/ */
                    sprintf(step+strlen(step), "%s", acc); /* 对第一个升降音符追加升降号 */
                    strcpy(acc_step, step);                /* 复制该升降音符 */
                    /* printf("%s\n", step); */
                    continue;
                }

          
                if(strstr(str_theme, "<duration")){ /* 音符的时长 */
                    sscanf(str_theme, "%*[^>]>%[^<]", dur);
                    /* printf("duration=%s\n", dur); /\* duration *\/ */
                    continue;
                }

        
                if(strstr(str_theme, "</note")){ /* 一个音符结束 */
                    /* printf("16th notes num=%d\n", atol(dur)/64); */
                    /* 1.1.1 fill in measure_theme */
                    for (i=0; i<atol(dur)/64;i++){
                        sprintf(measure_theme[mii+i],"%s", step); /* 写入分裂该音节为16th并写入 */
                    }
                    mii=mii+i;          /* 有效记录数增加到新值  */
          
                    /* for (i=0; i<mii; i++) { */
                    /*   printf("mesure[%d]=%s\n", i, measure_theme[i]); */
                    /* } */
                    continue;
                }
        
                if(strstr(str_theme, "</measure")) { 
                    break;
                }
            } /* 小节内读入行 */
        }     /* 在一小节内 */
    }         /* 读取theme文件 */
  
    printf("mii=%d\n", mii);
    for (i=0; i<mii; i++) {
        printf("mesure_theme[%02d]=%s\n", i, measure_theme[i]);
    }
    printf("%d measures found.\n", mi);


    /* 3  read var.xml */
    bzero(str_var, sizeof(str_var));

    mi=0; mii=0; 
    bzero(measure_var, 8*32);
    while ( NULL !=fgets(str_var, 256, fp_var) )  {
        if(strstr(str_var, "<measure")) { /* 一小节内 */
            mi++; 
	    
            bzero(str_var, sizeof(str_var));
            while ( NULL !=fgets(str_var, 256, fp_var) )  { /* 小节内读入行 */

                if(strstr(str_var, "<step")) { /* 音名 */
                    sscanf(str_var, "%*[^>]>%[^<]", step); 
                    if(acc_in_measure==1) {     /* 如果有升降音符，
                                                   那么把与升降音同名的音符改为升降音 */
                        if (step[0] == acc_step[0]) {   /* 如果与升降音同音名 */
                            strcpy(step, acc_step);
                        }
                    }
                    
                    /* printf("step=%s\n", step); */
                    continue;
                }
          
                if(strstr(str_var, "<accidental")){ /* 临时升降号 */
                    acc_in_measure=1;                 /* 小节内有升降号 */
                    sscanf(str_var, "%*[^>]>%[^<]", acc);
                    /* printf("acc=%s\n", acc); /\* accidental *\/ */
                    sprintf(step+strlen(step), "%s", acc); /* 对第一个升降音符追加升降号 */
                    strcpy(acc_step, step);                /* 复制该升降音符 */
                    /* printf("%s\n", step); */
                    continue;
                }

          
                if(strstr(str_var, "<duration")){ /* 音符的时长 */
                    sscanf(str_var, "%*[^>]>%[^<]", dur);
                    /* printf("duration=%s\n", dur); /\* duration *\/ */
                    continue;
                }

        
                if(strstr(str_var, "</note")){ /* 一个音符结束 */
                    /* printf("16th notes num=%d\n", atol(dur)/64); */
                    /* 1.1.1 fill in measure_var */
                    for (i=0; i<atol(dur)/64;i++){
                        sprintf(measure_var[mii+i],"%s", step); /* 写入分裂该音节为16th并写入 */
                    }
                    mii=mii+i;          /* 有效记录数增加到新值  */
          
                    /* for (i=0; i<mii; i++) { */
                    /*   printf("mesure[%d]=%s\n", i, measure_var[i]); */
                    /* } */
                    continue;
                }
        
                if(strstr(str_var, "</measure")) {
                    acc_in_measure=0;
                    break;
                }
            } /* 小节内读入行 */
        }     /* 在一小节内 */
    }         /* 读取theme文件 */

    printf("mii=%d\n", mii);    
    for (i=0; i<mii; i++) {
       printf("mesure_var[%d]=%s\n", i, measure_var[i]);
    }
    printf("%d measures found.\n", mi);

    printf("\n\n\n\n");

    /* 4  计算和累加 */

    dd=0; El_theme=0; El_var=0;

    for(i=0; i<=63; i++) {
        do {            /* into 4个16th */
            if(strcmp(measure_theme[i], measure_var[i])!=0) { /* sum( pitch_dis*duation ) */
                dd += abs( get_dis(measure_var[i]) - get_dis(measure_theme[i])) * (64.0/512.0);
            }
            
            El_theme = 5;       /* !!!! */
            El_var = 0;         /* !!!! */
            i++;
        } while (0 != ( (i)%4 ));
        i--;

        if( 0 == ( (i+1)%4 ) ) { /* finished 4个16th */ 
            printf("A(d1+d2) + B(sum[pitch_dis*duration)]   =   A(%d+%d)+B(%f)\n", El_theme, El_var, dd);               
            dd=0; El_theme=0; El_var=0;
        }
        
        if( 0 == ( (i+1)%8 ) ) { /* 完成了一个小节 */
            printf("measure %d finished------------\n", (i+1)/8 );
        } 
    }
    
        
    
    /* 5 输出 */
    /* printf("\n\n\n\ndistainc of these two pieces of music is  -------->%f\n", dd); */
    printf("\n\n\n\n Finished.\n");
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

