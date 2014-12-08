#include <stdio.h>              /* FILE 类型 */
#include <string.h>             /* bzero  不兼容 */
#include <stdlib.h>             /* malloc 不兼容 */


#define BOOL unsigned char
#define TRUE  1
#define FALSE 0
#define KKK  char

#define MAX_NUM_PITCH 64        /* 假设每次比较，最多剁碎成64个音符 */
#define XML_LINE_LEN 256        /* xml每行长度 */


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



typedef struct note {            /* 音符 */
    unsigned short  measure_num; /* 所属的小节号数 */
    char            step[32];    /* 音名, 含临时升降 */
    unsigned short   duration;   /* 时值 */
    struct note     *next;
} NOTE;

typedef struct  {               /* 一个小节中每个音符的内容,
                                   已经剁碎了，最终比较时 */
    char step[32];
    unsigned char duration;
} pitch;

pitch measure1[MAX_NUM_PITCH];
pitch measure2[MAX_NUM_PITCH];


NOTE *new_note(void);
int get_dis(char *);
NOTE *xml_data(FILE *, unsigned short *);





/* 新增一个音符节点 */
NOTE *new_note() {
    NOTE *p;
    
    p=malloc(sizeof(NOTE));
    bzero(p, sizeof(NOTE));
    p->next=NULL;
    
    return (p);
}


/* input: pitch name */
/* return: it's pitch distance */
int get_dis(char * pName) {
    int dis=0;
    int i;

    for(i=0; i<17; i++){
        if(strcmp(pName, p_dis_tab[i].p_name)==0)
            return p_dis_tab[i].p_dis;
    }

    return 0;
}

/*NOTE * */
NOTE *xml_data(FILE *fp, unsigned short *divisions) {
    NOTE *head=NULL, *cur = NULL; /* 链表当前音符 */
    char xml_line[XML_LINE_LEN];
    unsigned short note_num=0;
    char div[32]="";		/* divisions */
    char num[32]="";            /* 小节号 */
    char dur[32]="";            /* 时值 */
    char acc[32]="";            /* 升降号 */
    char acc_step[32]="";       /* 音符带升降号 */
    BOOL acc_in_measure=FALSE;  /* 本小节是否有升降号 */
    BOOL new_measure=FALSE;     /* 是否在同一小节 */


    
    /* 读入一行 */
    bzero(xml_line, sizeof(xml_line));
    while ( NULL != fgets(xml_line, XML_LINE_LEN, fp) ) {
        if ( strstr(xml_line, "<measure ") ) { /* 检测小节 */
	    
            /* 得到measure号 */
            bzero(num, sizeof(num));
            sscanf(xml_line, "%*[^\"]\"%[^\"]", num); /* %d 直接读取更好 ！ */
            
	    

	    
	    /* 小节开始 */
	    
	    bzero(xml_line, sizeof(xml_line));
	    while ( NULL != fgets(xml_line, XML_LINE_LEN, fp) ) { /* 在一个小节内 */
		
		/* 读入mode， key， deats， deat-type等等 */
		if ( strstr(xml_line, "<divisions>")) { /* divisions */
		    sscanf(xml_line, "%*[^>]>%[^<]", div); /* %d 直接读取更好 ！ */
		    *divisions=atol(div);
		    continue;
		}
		
		if ( strstr(xml_line, "<note") ) { /* 一个音符开始 */
		    /* 进入一个note */
		    if ( 0==note_num  ) { /* 第一个音符 */
			head=new_note(); /* 生成节点 */
			cur=head;
			note_num=1;	/* 以后再也不进入本分支 */

		    } else { /* 第二小节开始, 指针定位 */
			cur->next=new_note(); /* 链结进来一个新节点 */
			cur=cur->next; /* cur指针前进一步 */
		    }
		    cur->measure_num = atol(num); /* measure号写入data */
		    continue;
		} 
		  

		
                
		/* bzero(xml_line, sizeof(xml_line)); */
		/* while ( NULL != fgets(xml_line, XML_LINE_LEN, fp) ) { /\* 在一个音符内 *\/ */
		
                if ( strstr(xml_line, "<step>") ) {
                    sscanf(xml_line, "%*[^>]>%[^<]", cur->step);
                    if ( acc_in_measure==TRUE) { /* 如果本小节有升降号， 那么把同名音符改为升降音 */
                        if ( cur->step[0] == acc_step[0] ) { /* 如果同名 */
                            strcpy(cur->step, acc_step);
                        }
                    }
                    continue;
                }
		
                /* 多个音符有升降号怎么处理？ */
                /* 还原符号怎么处理 */
                if ( strstr(xml_line, "<accidental>") ) { /* 临时升降号 */
                    acc_in_measure=TRUE;               /* 本小节有升降号 */
                    
                    bzero(acc, sizeof(acc));
                    sscanf(xml_line, "%*[^>]>%[^<]", acc);
                    sprintf(cur->step+strlen(cur->step), "%s", acc); /* 对第一个音符追加升降号 */
                    strcpy(acc_step, cur->step); /* 复制该升降音符 */
                    continue;
                }
		
                if ( strstr(xml_line, "<duration>") ) { /* 音符的时值 */
                    sscanf(xml_line, "%*[^>]>%[^<]", dur);
                    cur->duration=atol(dur);
                    continue;
                }
		
                if ( strstr(xml_line, "</note>") ) { /* 一个音符结束 */
                    continue;
                }
	
	
            
		if ( strstr(xml_line, "</measure>") ) { /* 小节结束 */
		    acc_in_measure=FALSE;            /* 清除临时升降记号标记 */
		    break;
		}
	    } /* 在一小节内 */
	}   /* 完成一小节内 */
    }
    
    cur->next=new_note(); /* 末尾空节点 */
    cur=cur->next;	  /* cur指针前进一步 */
    
    return head;
}


main(int argc, char **argv) {
    
    unsigned short divisions=0;  /* 以4表示一个四分音的时值 */
    char mode[32] = "";         /* "major" "minor" */
    short fifths = 0;         /* 升降符号的个数，也就是调号, 0代表C */
    unsigned char beats = 2;  /* 每小节的拍子数 */
    unsigned char beat_type = 4; /* 以什么音符为一拍 */

    NOTE *head1 = NULL, *head2 = NULL; /* 链表第一个音符 */
    NOTE *cur1 = NULL, *cur2 = NULL;   /* 链表当前音符 */


    FILE *fp1;
    FILE *fp2;
    

    
    /* 1  Open 2 xml files */
    fp1 = fopen(argv[1], "r");
    if( fp1 == NULL ) {
        printf("open file error.\n");
        return -1;
    }

    /* fp2 = fopen(argv[2], "r"); */
    /* if( fp2 == NULL ) { */
    /*     printf("open file error.\n"); */
    /*     return -1; */
    /* } */



        
        
    /* 读入XML到链表 */
    head1 = xml_data(fp1, &divisions); /* &divisions应该替换成 div，mode，key，beats等的结构指针 */
    /* head2 = xml_data(fp2); */

    cur1=head1;
    for (cur1=head1; cur1->next!=NULL; cur1=cur1->next) {
        printf("measuer_num=%d\n", cur1->measure_num);
        printf("step=%s\n", cur1->step);
        printf("duration=%d\n", cur1->duration);
    }

    printf("div=%d\n", divisions);

    printf("\n\n");
    

}
