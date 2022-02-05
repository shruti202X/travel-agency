#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Date{
    int date;
    struct Date *next;
};

struct Place{
    int place_num;
};

struct Group {
    char grp_lead[5];
    int grp_count;
    int date;
    int destination;
    int type;//0 for normal and 1 for premium
    struct Group *next;
};

struct Main {
    struct Date * p_date;
    struct Place *p_place;
    int t_tourists;//total_tourists
    struct Group *grp;
    struct Main *prev;
    struct Main *next;
};

int c1(FILE *fp, struct Place ** addr_p_active_dest);

int main(){
    int *Dar=0, *Leh=0, *Ooty=0;
    FILE *fp;
    /*
    fp = fopen("tour.txt","w");
    if (fp==NULL){
        printf("Unable to create tour.txt\n");
        return 0;
    }
    fprintf(fp,"10 C1 TS2 C8 C3 C3A C3 C9");
    fclose(fp);
    */
    fp = fopen("tour.txt", "r");
    if (fp==NULL){
        printf("Unable to open tour.txt\n");
        return 0;
    }
    int N=0;
    fscanf(fp,"%d",&N);
    if (N==0){
        printf("No trips possible.\n");
        return 0;
    }
    struct Place *p_active_dest=NULL;
    int running = 1;
    char c;
    char its_s[10];//to be del
    while(running){
        do c = fgetc(fp); while(c==' ' || c=='\n' || c==EOF);
        if(c!='C' && c!='c') running=0;
        else {
            c=fgetc(fp);
            switch(c){
                case '1': c1(fp, &p_active_dest);
                printf("C1 was detected\n");
                printf("Now dest code is %d\n",p_active_dest->place_num);
                break;
                case '2': 
                printf("C2 detected\n");break;
                case '3':
                c = fgetc(fp);
                if ('A'==c || 'a'==c) {printf("C3A detected\n");break;}
                printf("C3 detected\n");break;
                case '5':printf("C5 detected\n");break;
                case '8':printf("C8 detected\n");break;
                case '9':printf("C9 detected\n");running=0;break;
            };
        }
    }
    fclose(fp);
    free(p_active_dest);
    return 0;
}
/*
if (addr_p_active_dest==NULL){
    * addr_p_active_dest = (struct Place *) malloc(sizeof(struct Place));
    //malloc return pointer to the allocated memory, we typecast it to struct Place * datatype
}
*/
//to do: if returned 0
int c1(FILE *fp, struct Place ** addr_p_active_dest){
    char c;
    do c = fgetc(fp); while(c==' ' || c=='\n' || c==EOF);
    if (c!='T') return 0;
    c = fgetc(fp);
    if (c!='S') return 0;
    c = fgetc(fp);
    //to remember: * derference things
    //to improve: you can choose to not allocate memory if default-case runs(not 1, 2 or 3)
    if (* addr_p_active_dest==NULL){
        * addr_p_active_dest = (struct Place *) malloc(sizeof(struct Place));
        //malloc return pointer to the allocated memory, we typecast it to struct Place * datatype
        (*addr_p_active_dest)->place_num = 0;
    }
    switch(c){
        case '1': (*addr_p_active_dest)->place_num = 1; break;
        case '2': (*addr_p_active_dest)->place_num = 2; break;
        case '3': (*addr_p_active_dest)->place_num = 3; break;
        //maybe we can write where/at which line its wrong
        default: printf("TS_, _ is not 1, 2 or 3\n");
        return 0;
    };
    return 1;
}