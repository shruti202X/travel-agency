#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//idea in my mind rn: make the date ptr like active date ptr;
/*
struct Date{
    int date;
    struct Date *next;
};
*/

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
    struct Group *prev;
};

struct Main {
    int date;
    int place;
    int t_tourists;//total_tourists
    struct Group *groups;
    struct Main *prev;
    struct Main *next;
};

int c1(FILE *fp, struct Place ** addr_p_active_dest);
int c2(FILE *fp, int *);
int c3(FILE *fp, int N, struct Main ** addr_main_head, struct Place * p_active_dest, int );
int c3a(FILE *fp, int N, struct Main ** addr_main_head, struct Place * p_active_dest, int );
int c5(FILE *fp, struct Main *main_head);
int c8(FILE *fp, struct Main *main_head);


void free_groups(struct Group *groups_list){
    struct Group * temp = groups_list;
    while(groups_list!=NULL){
        temp = groups_list->next;
        free(groups_list);
        groups_list = temp;
    }
};

void free_mains(struct Main *main_head){
    struct Main * temp = main_head;
    while(main_head!=NULL){
        temp = main_head->next;
        free_groups(main_head->groups);
        main_head = temp;
    }
};

void print_groups(struct Group *groups_list){
    struct Group * temp = groups_list;
    while(temp!=NULL){
        printf("%s ",temp->grp_lead);
        temp = temp->next;
    }
    printf("\n");
};

void print_destination(int dest, struct Main *main_head){
    struct Main *temp_main = main_head;
    while(temp_main!=NULL){
        if (temp_main->place == dest){
            printf("May %d: ",temp_main->date);
            if(temp_main->t_tourists <1 || temp_main->date >25) printf("No trip\n");
            else {
                printf("%d tourists: ", temp_main->t_tourists);
                print_groups(temp_main->groups);
            }
        }
        temp_main = temp_main->next;
    }
    printf("-----------------------------------------\n");
};

int main(){
    FILE *fp;
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
    int * active_date = NULL;
    //ptr cuz i want it to change inside func.
    struct Main *main_head = NULL;
    int running = 1;
    char c;
    while(running){
        do c = fgetc(fp); while(c==' ' || c=='\n' || c==EOF);
        if(c!='C' && c!='c') running=0;
        else {
            c=fgetc(fp);
            switch(c){
                case '1': c1(fp, &p_active_dest);
                break;
                case '2': c2(fp, active_date);
                break;
                case '3':
                c = fgetc(fp);
                if ('A'==c || 'a'==c) {
                    c3a(fp, N, &main_head, p_active_dest, *active_date);
                    break;
                }
                c3(fp, N, &main_head, p_active_dest, *active_date);
                break;
                case '5': c5(fp, main_head);
                break;
                case '8':
                c8(fp, main_head);
                break;
                case '9':
                running=0;break;
            };
        }
    }
    fclose(fp);
    free(p_active_dest);
    free(active_date);
    free_mains(main_head);
    return 0;
}
int c1(FILE *fp, struct Place ** addr_p_active_dest){
    char c;
    do c = fgetc(fp); while(c==' ' || c=='\n');
    if (c!='T') return 0;
    c = fgetc(fp);
    if (c!='S') return 0;
    c = fgetc(fp);
    if (* addr_p_active_dest==NULL){
        * addr_p_active_dest = (struct Place *) malloc(sizeof(struct Place));
        (*addr_p_active_dest)->place_num = 0;
    }
    switch(c){
        case '1': (*addr_p_active_dest)->place_num = 1; break;
        case '2': (*addr_p_active_dest)->place_num = 2; break;
        case '3': (*addr_p_active_dest)->place_num = 3; break;
        default: return 0;
        return 0;
    };
    return 1;
}

int c2(FILE *fp, int  * active_date){
    int date = 0;
    char c;
    do c=fgetc(fp); while(c==' ' || c=='\n');
    //'0's code is 48 => '9's code is 57
    if (c<48 || c>57) return 0; //c is not a num
    date = (int)c - (int)'0';
    c = fgetc(fp);
    if (c==' ' || c=='\n');
    else if (c>=48 && c<=57){
        date = date*10 + (int)c - (int)'0';
    } else return 0;
    if(date>31 || date<1) return 0;
    //newDate
    *active_date = date;
    return 1;
}

struct Main * make_main(int date, int dest){
    struct Main * newMain = (struct Main *)malloc(sizeof(struct Main));
    newMain->date = date;
    newMain->place = dest;
    newMain->t_tourists = 0;
    newMain->groups = NULL;
    newMain->prev = NULL;
    newMain->next = NULL;
    return newMain;
}
int c3(FILE *fp, int N, struct Main ** addr_main_head, struct Place * p_active_dest, int active_date){
    char name[5];
    char c;
    do c=fgetc(fp); while (c==' ' || c=='\n');
    name[0] = c;
    c=fgetc(fp);
    name[1]=c;
    name[2]='\0';
    int tourists;
    fscanf(fp,"%d",&tourists);
    if (tourists<1 || tourists>N) return 0;

    if (p_active_dest == NULL) return 0;
    int active_dest = p_active_dest->place_num;

    struct Main * newMain=NULL;

    //firstly we want to go to that groups column or collection or Main
    //where active dest and date matches then see if tourists spots available
    //if no such Main then make one insert it into list of mains at appropriate pos
    //add groups

    //assumed that *addr_main_head!=NULL
    struct Main * temp_main = NULL;
    temp_main = *addr_main_head;
    //make sure that temp is not null cuz if null then next component wont work
    if(temp_main!=NULL){
        while(temp_main->date < active_date && temp_main->next!=NULL){
            temp_main = temp_main->next;
        }
        //make new node either way at end or here i.e. at temp or temp->next if temp->date!=active_date
        if (temp_main->date != active_date){
            //here order of if-else is imp
            newMain = make_main(active_date, active_dest);
            if (temp_main->date > active_date){
                //make new node at temp->prev =>temp->prev->next=newNode
                temp_main->prev->next = newMain;
                newMain->prev = temp_main->prev->next;
                newMain->next = temp_main;
                temp_main->prev = newMain;
            }
            else if (temp_main->next==NULL){
                //make new node at temp->next =>temp->next=newNode
                temp_main->next = newMain;
                newMain->prev = temp_main;
            }
            else return 0;//well i dont think its possible
        } else {
            while((temp_main->place < active_dest && temp_main->date == active_date) && temp_main->next!=NULL){
                temp_main = temp_main->next;
            }
            if(temp_main->place > active_dest || temp_main->date > active_date){
                newMain = make_main(active_date, active_dest);
                temp_main->prev->next = newMain;
                newMain->prev = temp_main->prev->next;
                newMain->next = temp_main;
                temp_main->prev = newMain;
            }
            else if (temp_main->place == active_dest) newMain = temp_main;//no need to link cuz temp is already linked
            else if(temp_main->next == NULL){
                newMain = make_main(active_date, active_dest);
                temp_main->next = newMain;
                newMain->prev = temp_main;
            }
        }
    } else {
        newMain = make_main(active_date, active_dest);
        *addr_main_head = newMain;
    }
    //
    //
    //maybe u can make this better:
    //
    if (newMain->t_tourists + tourists > N) return 0;
    newMain->t_tourists+=tourists;

    struct Group *newGroup;
    newGroup = (struct Group *)malloc(sizeof(struct Group));
    strcpy(newGroup->grp_lead,name);
    newGroup->grp_count=tourists;
    newGroup->date = active_date;
    newGroup->destination = active_dest;
    newGroup->type = 0;
    newGroup->next = NULL;
    //do we need prev
    newGroup->prev=NULL;
    //in get Date func
    //
    //
    struct Group * temp_grp = newMain->groups;
    //
    //now we need to add newGroup to groups_list
    //also make sure that this list isnt empty
    //and add to the end of the list
    if (temp_grp == NULL){
        newMain->groups = newGroup;
    } else {
        while(temp_grp->next!=NULL){
            temp_grp=temp_grp->next;
        }
        temp_grp->next = newGroup;
        //do we need prev
        newGroup->prev = temp_grp;
        //no need to take care of preceeding nodes as we inserted at end
    }
    return 1;
}

//ignored the fact that if all the grps in the list are already premium =>type 1 and not 0
int c3a(FILE *fp, int N, struct Main ** addr_main_head, struct Place * p_active_dest, int active_date){
    char name[5];
    char c;
    do c=fgetc(fp); while (c==' ' || c=='\n');
    name[0] = c;
    c=fgetc(fp);
    name[1]=c;
    name[2]='\0';
    //to do: err if initials already present

    int tourists;
    fscanf(fp,"%d",&tourists);//to do: show err
    if (tourists<1 || tourists>N) return 0;

    if (p_active_dest == NULL) return 0;
    int active_dest = p_active_dest->place_num;

    struct Main * newMain=NULL;

    //firstly we want to go to that groups column or collection or Main
    //where active dest and date matches then see if tourists spots available
    //if no such Main then make one insert it into list of mains at appropriate pos
    //add groups

    //assumed that *addr_main_head!=NULL
    struct Main * temp_main = NULL;
    temp_main = *addr_main_head;
    //make sure that temp is not null cuz if null then next component wont work
    if(temp_main!=NULL){
        while(temp_main->date < active_date && temp_main->next!=NULL){
            temp_main = temp_main->next;
        }
        //make new node either way at end or here i.e. at temp or temp->next if temp->date!=active_date
        if (temp_main->date != active_date){
            //here order of if-else is imp
            newMain = make_main(active_date, active_dest);
            if (temp_main->date > active_date){
                //make new node at temp->prev =>temp->prev->next=newNode
                temp_main->prev->next = newMain;
                newMain->prev = temp_main->prev->next;
                newMain->next = temp_main;
                temp_main->prev = newMain;
            }
            else if (temp_main->next==NULL){
                //make new node at temp->next =>temp->next=newNode
                temp_main->next = newMain;
                newMain->prev = temp_main;
            }
            else return 0;//well i dont think its possible
        } else {
            while((temp_main->place < active_dest && temp_main->date == active_date) && temp_main->next!=NULL){
                temp_main = temp_main->next;
            }
            if(temp_main->place > active_dest || temp_main->date > active_date){
                newMain = make_main(active_date, active_dest);
                temp_main->prev->next = newMain;
                newMain->prev = temp_main->prev->next;
                newMain->next = temp_main;
                temp_main->prev = newMain;
            }
            else if (temp_main->place == active_dest) newMain = temp_main;//no need to link cuz temp is already linked
            else if(temp_main->next == NULL){
                newMain = make_main(active_date, active_dest);
                temp_main->next = newMain;
                newMain->prev = temp_main;
            }
        }
    } else {
        newMain = make_main(active_date, active_dest);
        *addr_main_head = newMain;
    }
    //
    struct Group * temp_grp = newMain->groups;
    newMain->t_tourists += tourists;
    //to improve: is it clean code/logic?
    while(newMain->t_tourists > N){
        temp_grp = newMain->groups;
        if (temp_grp->next == NULL){
            newMain->t_tourists -= temp_grp->grp_count;
            // No need to: temp_grp->prev->next = NULL; === NULL = NULL;
            free(temp_grp);
            temp_grp=NULL;
            newMain->groups = temp_grp;
            //simply: newMain->groups = NULL;
            //above line cuz we are changing head
        } else {
            while(temp_grp->next!=NULL){
                temp_grp = temp_grp->next;
            }
            newMain->t_tourists -= temp_grp->grp_count;
            temp_grp->prev->next = NULL;
            temp_grp =temp_grp->prev;
            free(temp_grp->next);
        }
        //temp_grp can be NULL after iteration
    }
    struct Group *newGroup;
    newGroup = (struct Group *)malloc(sizeof(struct Group));
    strcpy(newGroup->grp_lead,name);
    newGroup->grp_count=tourists;
    newGroup->date = active_date;
    newGroup->destination = active_dest;
    newGroup->type = 1;
    newGroup->next = NULL;
    newGroup->prev=NULL;
    //
    //
    temp_grp = newMain->groups;
    //
    if (temp_grp == NULL){
        newMain->groups = newGroup;
    } else {
        newMain->groups = newGroup;
        temp_grp->prev = newGroup;
        newGroup->next = temp_grp;
    }
    return 1;
}

int c5(FILE *fp, struct Main * main_head){
    //to improove: if initals one time were in capslock but anther in lower case
    char name[5];
    fscanf(fp, "%s", name);
    //
    //now find that main_head;
    //also check for err if no such main exists;
    struct Main *temp_main = main_head;
    struct Group *temp_grp = temp_main->groups;
    int finding=1;
    //first it showed me err cuz i used || instead of &&
    while(temp_main!=NULL && finding==1){
        temp_grp = temp_main->groups;
        while(temp_grp!=NULL && finding==1){
            if (strcmp(temp_grp->grp_lead,name)==0){
                finding = 0;//cuz we found
                break;
            }
            temp_grp=temp_grp->next;
        }
        if(finding==0) break;
        temp_main=temp_main->next;
    }
    //if (temp_grp==NULL);
    //means no such name in our database;
    //maybe we could return 0;
    //but for now lets keep it like this;
    if (temp_grp!=NULL && temp_main!=NULL){//or if(finding==0); acc to me, finding==0 or temp_grp!=NULL or temp_main!=NULL
        struct Group *new_temp_grp = temp_main->groups;
        temp_main->t_tourists -= temp_grp->grp_count;
        if(new_temp_grp == temp_grp){
            temp_main->groups = temp_grp->next;
            if(temp_grp->next!=NULL) temp_grp->next->prev=NULL;
            free(temp_grp);
        } else {
            temp_grp->prev->next = temp_grp->next;
            temp_grp->next->prev = temp_grp->prev;
            free(temp_grp);
        }
    }
    return 1;
}

int c8(FILE *fp, struct Main *main_head){
     char c;
    do c = fgetc(fp); while(c==' ' || c=='\n');
    if (c!='T') return 0;
    c = fgetc(fp);
    if (c!='S') return 0;
    c = fgetc(fp);
    switch(c){
        case '1': printf("\nTS1:  Darjeeling\n");
        print_destination(1,main_head);
        break;
        case '2': printf("\nTS2:  Leh\n");
        print_destination(2,main_head);
        break;
        case '3': printf("\nTS3:  Ooty\n");
        print_destination(3,main_head);
        break;
        default: return 0;//or u might wanna return 1
    };
    return 1;
}