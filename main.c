#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define Alpha 0.5
#define Beta  0.5


typedef struct{
    char p_name[32];            /* pitch �� */
    char p_dis;                 /* ��p0�ľ��� */
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


char measure_theme[64][32];     /* �����ȫ����¼�� ��16thΪһ����¼ */
char measure_var[64][32];       /* �����ȫ����¼�� ��16thΪһ����¼ */

int get_dis(char * );

main(int argc, char** argv)
{
    FILE *fp_theme, *fp_var; 
    char str_theme[256];
    char str_var[256];

    int i;
    char mi=0;                  /* number of measure */
    char mii=0;                 /* number of 16th */
    
    char step[32]="";           /* setp ���� */
    char acc[32]="";            /* accidental, ��ʱ�����Ǻ� */
    char dur[32]="";            /* duration, ������ʱ�� */
    char acc_in_measure=0;      /* С����������ʱ������ */
    char acc_step[32]="";       /* ���������� */

    long El_var=0;             /* var ÿ�������ֽ�� Edit length */
    long El_theme=0;           /* theme   ÿ�������ֽ�� Edit length */
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

        if(strstr(str_theme, "<measure")) { /* һС���� */
            mi++; 
      
            bzero(str_theme, sizeof(str_theme));
            while ( NULL !=fgets(str_theme, 256, fp_theme) )  { /* С���ڶ����� */

                if(strstr(str_theme, "<step")) { /* ���� */
                    sscanf(str_theme, "%*[^>]>%[^<]", step); 
                    if(acc_in_measure==1) {     /* ���������������
                                                   ��ô����������ͬ����������Ϊ������ */
                        if (step[0] == acc_step[0]) {   /* �����������ͬ���� */
                            strcpy(step, acc_step);
                        }
                    }
                    
                    /* printf("step=%s\n", step); */
                    continue;
                }
          
                if(strstr(str_theme, "<accidental")){ /* ��ʱ������ */
                    acc_in_measure=1;                 /* С������������ */
                    sscanf(str_theme, "%*[^>]>%[^<]", acc);
                    /* printf("acc=%s\n", acc); /\* accidental *\/ */
                    sprintf(step+strlen(step), "%s", acc); /* �Ե�һ����������׷�������� */
                    strcpy(acc_step, step);                /* ���Ƹ��������� */
                    /* printf("%s\n", step); */
                    continue;
                }

          
                if(strstr(str_theme, "<duration")){ /* ������ʱ�� */
                    sscanf(str_theme, "%*[^>]>%[^<]", dur);
                    /* printf("duration=%s\n", dur); /\* duration *\/ */
                    continue;
                }

        
                if(strstr(str_theme, "</note")){ /* һ���������� */
                    /* printf("16th notes num=%d\n", atol(dur)/64); */
                    /* 1.1.1 fill in measure_theme */
                    for (i=0; i<atol(dur)/64;i++){
                        sprintf(measure_theme[mii+i],"%s", step); /* д����Ѹ�����Ϊ16th��д�� */
                    }
                    mii=mii+i;          /* ��Ч��¼�����ӵ���ֵ  */
          
                    /* for (i=0; i<mii; i++) { */
                    /*   printf("mesure[%d]=%s\n", i, measure_theme[i]); */
                    /* } */
                    continue;
                }
        
                if(strstr(str_theme, "</measure")) { 
                    break;
                }
            } /* С���ڶ����� */
        }     /* ��һС���� */
    }         /* ��ȡtheme�ļ� */
  
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
        if(strstr(str_var, "<measure")) { /* һС���� */
            mi++; 
	    
            bzero(str_var, sizeof(str_var));
            while ( NULL !=fgets(str_var, 256, fp_var) )  { /* С���ڶ����� */

                if(strstr(str_var, "<step")) { /* ���� */
                    sscanf(str_var, "%*[^>]>%[^<]", step); 
                    if(acc_in_measure==1) {     /* ���������������
                                                   ��ô����������ͬ����������Ϊ������ */
                        if (step[0] == acc_step[0]) {   /* �����������ͬ���� */
                            strcpy(step, acc_step);
                        }
                    }
                    
                    /* printf("step=%s\n", step); */
                    continue;
                }
          
                if(strstr(str_var, "<accidental")){ /* ��ʱ������ */
                    acc_in_measure=1;                 /* С������������ */
                    sscanf(str_var, "%*[^>]>%[^<]", acc);
                    /* printf("acc=%s\n", acc); /\* accidental *\/ */
                    sprintf(step+strlen(step), "%s", acc); /* �Ե�һ����������׷�������� */
                    strcpy(acc_step, step);                /* ���Ƹ��������� */
                    /* printf("%s\n", step); */
                    continue;
                }

          
                if(strstr(str_var, "<duration")){ /* ������ʱ�� */
                    sscanf(str_var, "%*[^>]>%[^<]", dur);
                    /* printf("duration=%s\n", dur); /\* duration *\/ */
                    continue;
                }

        
                if(strstr(str_var, "</note")){ /* һ���������� */
                    /* printf("16th notes num=%d\n", atol(dur)/64); */
                    /* 1.1.1 fill in measure_var */
                    for (i=0; i<atol(dur)/64;i++){
                        sprintf(measure_var[mii+i],"%s", step); /* д����Ѹ�����Ϊ16th��д�� */
                    }
                    mii=mii+i;          /* ��Ч��¼�����ӵ���ֵ  */
          
                    /* for (i=0; i<mii; i++) { */
                    /*   printf("mesure[%d]=%s\n", i, measure_var[i]); */
                    /* } */
                    continue;
                }
        
                if(strstr(str_var, "</measure")) {
                    acc_in_measure=0;
                    break;
                }
            } /* С���ڶ����� */
        }     /* ��һС���� */
    }         /* ��ȡtheme�ļ� */

    printf("mii=%d\n", mii);    
    for (i=0; i<mii; i++) {
       printf("mesure_var[%d]=%s\n", i, measure_var[i]);
    }
    printf("%d measures found.\n", mi);

    printf("\n\n\n\n");

    /* 4  ������ۼ� */

    dd=0; El_theme=0; El_var=0;

    for(i=0; i<=63; i++) {
        do {            /* into 4��16th */
            if(strcmp(measure_theme[i], measure_var[i])!=0) { /* sum( pitch_dis*duation ) */
                dd += abs( get_dis(measure_var[i]) - get_dis(measure_theme[i])) * (64.0/512.0);
            }
            
            El_theme = 5;       /* !!!! */
            El_var = 0;         /* !!!! */
            i++;
        } while (0 != ( (i)%4 ));
        i--;

        if( 0 == ( (i+1)%4 ) ) { /* finished 4��16th */ 
            printf("A(d1+d2) + B(sum[pitch_dis*duration)]   =   A(%d+%d)+B(%f)\n", El_theme, El_var, dd);               
            dd=0; El_theme=0; El_var=0;
        }
        
        if( 0 == ( (i+1)%8 ) ) { /* �����һ��С�� */
            printf("measure %d finished------------\n", (i+1)/8 );
        } 
    }
    
        
    
    /* 5 ��� */
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

