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
int c2(FILE *fp, struct Date ** addr_date_head);

void free_dates(struct Date *date_head){
    struct Date * temp = date_head;
    while(date_head!=NULL){
        temp = date_head->next;
        free(date_head);
        date_head = temp;
    }
};

void print_dates(struct Date *date_head){
    struct Date * temp = date_head;
    while(temp!=NULL){
        printf("%d ",temp->date);
        temp = temp->next;
    }
    printf("\n");
};

int main(){
    int *Dar=0, *Leh=0, *Ooty=0;
    FILE *fp;
    /*
    fp = fopen("tour.txt","w");
    if (fp==NULL){
        printf("Unable to create tour.txt\n");
        return 0;
    }
    fprintf(fp,"12 C1 TS2 C2 14 C2 30 C2 22 C9");
    fclose(fp);
    */
    fp = fopen("tour.txt", "r");
    if (fp==NULL){
        printf("Unable to open tour.txt\n");
        return 0;
    }
    int N=0;
    fscanf(fp,"%d",&N);
    if (N<1){
        printf("No trips possible.\n");
        return 0;
    }
    struct Place *p_active_dest=NULL;
    struct Date *date_head;
    int running = 1;
    char c;
    //you wanna put this in queue
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
                case '2': c2(fp, &date_head);
                printf("C2 detected\n");
                printf("Dates are: "); print_dates(date_head);
                break;
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
    free_dates(date_head);
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
    do c = fgetc(fp); while(c==' ' || c=='\n');
    //you dont add while c==EOF okay, so removed in this commit
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

int c2(FILE *fp, struct Date ** addr_date_head){
    int success = 1; //func returns 1 if nothing goes wrong
    int date = 0;
    char c;
    // for space after code C2
    do c=fgetc(fp); while(c==' ' || c=='\n');
    //'0's code is 48 => '9's code is 57
    if (c<48 || c>57) return 0; //c is not a num
    date = (int)c - (int)'0'; //+date*10<--no need
    c = fgetc(fp);
    //if (c==EOF || c==' ' || c=='\n') continue;//one digit date
    //err in above line: continue statement not within a loop
    //u might wanna check eof at different places ok
    //to improve: u might wanna change logic what if no space and c
    //for code is started here then it wont get to read c again
    if (c==' ' || c=='\n');//one digit
    else if (c>=48 && c<=57){
        date = date*10 + (int)c - (int)'0';
        //the nxt lines are tested if only 2 digit cuz 1 digit already tested
        /*
        c=fgetc(fp);
        //what if c=EOF or c; we may want to specify add space after dates
        if (c!=' ' && c!='\n') return 0;//month cant be 3 digit;
        //no need to test this cuz then there will be err in nxt command
        */
    } else return 0;

    //month of may has 31 days
    //u need 7 day trips
    //date could be 1 to 25
    if(date>25 || date<1) return 0;

    int prevDate=0;
    struct Date *date_ptr = *addr_date_head;//date pointer to not change date_head
    /*
    while(date_ptr!=NULL){
        prevDate=date_ptr->data;
        date_ptr=date_ptr->next;
    }
    */
    //to improve: maybe a better way to assign prevDate
    if(date_ptr!=NULL){
        //here we know that date_ptr definately has next component
        while(date_ptr->next!=NULL){
            date_ptr = date_ptr->next;
        }
        prevDate = date_ptr->date;
    }
    //idk why anxiety but what if prevDate create scope problem
    
    if(prevDate>date) return 0;
    //i know there was no need for this but just to make this complete
    //y there was no need? see question in README.md

    //newDate
    struct Date *newDate;
    newDate = (struct Date *)malloc(sizeof(struct Date));
    newDate->date = date;
    newDate->next = NULL;

    //inserting newDate at end
    date_ptr = *addr_date_head;
    if (date_ptr==NULL) *addr_date_head = newDate;
    else{
        while(date_ptr->next!=NULL){
            date_ptr = date_ptr->next;
        }
        date_ptr->next = newDate;
    }
    return 1;
}