#include<iostream>
#include<fstream>
#include<math.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

COORD coord={0,0};

void gotoxy(int x,int y){
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

class player{
public:
    int hp,max_hp,attack,defence,level,exp=0,next_exp,money=5000,potion=5,super_potion=0,max_potion=0;
    player(int player_level){
        level = player_level;
        init();
    }
    void init(){
        next_exp = level*5 + level;
        if(level>=100)
            exp = next_exp;
        max_hp = level*4 + level;
        hp = max_hp;
        attack = level*3 + level;
        defence = level*2 + level;
    }
    void level_up(int previous_exp){
        if(level<100)
            level++;
        exp = previous_exp;
        init();
    }
    void exp_up(int value){
        exp += value;
        if(exp>=next_exp)
            level_up(exp-next_exp);
    }
    void hit(int damage){
        hp -= damage;
    }
    void heal(int value){
        hp += value;
        if(hp>=max_hp)
            hp = max_hp;
    }
};

class enemy{
public:
    int hp,max_hp,attack,defence,level,potion=5;
    enemy(int enemy_level){
        level = enemy_level;
        init();
    }
    void init(){
        max_hp = level*4 + level;
        hp = max_hp;
        attack = level*3 + level;
        defence = level*2 + level;
    }
    void hit(int damage){
        hp -= damage;
    }
    void heal(){
        hp += 20;
        if(hp>=max_hp)
            hp = max_hp;
    }
};
int i,j,k,x=5,y=5;
string place;
int quest[20],questLevel;
bool isOnQuest = false, questCompleted[20];

bool match(player &p,enemy &e);
void shop_display(string name, string description, int cost, int y);
void shop(player &p);
void wild(player &p);
void forest(player &p);
void dungeon(player &p);
void free_roam(player &p);
void inventory(player &p);
bool fexists(const string& filename);
int* intro();

bool match(player &p,enemy &e){
    while(p.hp>0&&e.hp>0){
        system("cls");
        gotoxy(1,0);
        cout<<"hp : ";
        for(i=0;i<p.hp*20/p.max_hp;i++)
            cout<<"*";
        gotoxy(26,0);
        cout<<"("<<p.hp<<"/"<<p.max_hp<<")";

        gotoxy(1,1);
        cout<<"exp : ";
        for(i=0;i<p.exp*20/p.next_exp;i++)
            cout<<"*";
        gotoxy(27,1);
        cout<<"("<<p.exp<<"/"<<p.next_exp<<")";

        gotoxy(1,2);
        cout<<"level : "<<p.level;

        gotoxy(1,3);
        cout<<"attack : "<<p.attack;

        gotoxy(1,4);
        cout<<"defence : "<<p.defence;

        gotoxy(0,5);
        for(i=0;i<80;i++)
            cout<<"_";

        gotoxy(0,20);
        for(i=0;i<80;i++)
            cout<<"_";

        gotoxy(45,21);
        cout<<"defence : "<<e.defence;

        gotoxy(45,22);
        cout<<"attack : "<<e.attack;

        gotoxy(45,23);
        cout<<"level : "<<e.level;

        gotoxy(45,24);
        cout<<"hp : ";
        for(i=0;i<e.hp*20/e.max_hp;i++)
            cout<<"*";
        gotoxy(70,24);
        cout<<"("<<e.hp<<"/"<<e.max_hp<<")";

        gotoxy(1,6);
        cout<<"Please choose an action : 1.Attack\t2.Defend\t3.Heal\n";
        int pm,ph;
        pm = getch();
        pm -= 48;
        int em;
        if(e.hp<e.max_hp/2 && e.potion>0){
            srand (time(NULL));
            em = rand()%3+1;
        }else{
            srand (time(NULL));
            em = rand()%2+1;
        }
        switch(pm){
        case 1:
            gotoxy(1,10);
            cout<<"Attack";
            gotoxy(65,10);
            if(em==1){
                cout<<"Attack";
                int damage = p.attack - e.attack;
                if(damage>0){
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    e.hit(damage);
                    gotoxy(65,15);
                    cout<<"hp : -"<<damage;
                }else if(damage<0){
                    p.hit(0-damage);
                    gotoxy(1,15);
                    cout<<"hp : -"<<0-damage;
                    e.hit(2);
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }else{
                    e.hit(2);
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }
            }
            else if(em==2){
                cout<<"Defend";
                int damage = p.attack - e.defence;
                if(damage>0){
                    e.hit(damage);
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    gotoxy(65,15);
                    cout<<"hp : -"<<damage;
                }else if(damage<0){
                    p.hit(0-damage);
                    gotoxy(1,15);
                    cout<<"hp : -"<<0-damage;
                    e.hit(2);
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }else{
                    e.hit(2);
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }
            }else{
                cout<<"Heal";
                e.heal();
                e.potion--;
                gotoxy(65,15);
                cout<<"hp : +20";
                e.hit(p.attack);
                gotoxy(65,16);
                cout<<"hp : -"<<p.attack;
            }
            break;
        case 2:
            gotoxy(1,10);
            cout<<"Defend";
            gotoxy(65,10);
            if(em==1){
                cout<<"Attack";
                int damage = p.defence - e.attack;
                if(damage>0){
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    e.hit(damage);
                    gotoxy(65,15);
                    cout<<"hp : -"<<damage;
                }else if(damage<0){
                    p.hit(damage);
                    gotoxy(1,15);
                    cout<<"hp : -"<<damage;
                    e.hit(2);
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }else{
                    e.hit(2);
                    p.hit(2);
                    gotoxy(1,15);
                    cout<<"hp : -2";
                    gotoxy(65,15);
                    cout<<"hp : -2";
                }
            }
            else if(em==2){
                cout<<"Defend";
            }else{
                cout<<"Heal";
                e.heal();
                e.potion--;
                gotoxy(65,15);
                cout<<"hp : +20";
            }
            break;
        case 3:
            gotoxy(1,8);
            if(p.potion <=0 && p.super_potion <=0 && p.max_potion <= 0){
                cout<<"No healing items left";
                break;
            }
            cout<<"1.Potion ("<<p.potion<<")\t2.Super Potion ("<<p.super_potion<<")\t3.Max Potion ("<<p.max_potion<<")";
            ph = getch();
            ph -= 48;
            gotoxy(1,10);
            cout<<"Heal";
            gotoxy(65,10);
            if(em==1){
                cout<<"Attack";
                gotoxy(1,15);
                if(ph==1){
                    if(p.potion <= 0){
                        cout<<"No potions left...";
                        break;
                    }
                    cout<<"hp : +20";
                    p.heal(20);
                    p.potion--;
                }else if(ph==2){
                    if(p.super_potion <= 0){
                        cout<<"No super potions left...";
                        break;
                    }
                    cout<<"hp : +50";
                    p.heal(50);
                    p.super_potion--;
                }else if(ph==3){
                    if(p.max_potion <= 0){
                        cout<<"No max potions left...";
                        break;
                    }
                    cout<<"hp : +"<<p.max_hp-p.hp;
                    p.heal(p.max_hp-p.hp);
                    p.max_potion--;
                }
                p.hit(e.attack);
                gotoxy(1,16);
                cout<<"hp : -"<<e.attack;
            }
            else if(em==2){
                cout<<"Defend";
                gotoxy(1,15);
                if(ph==1){
                    if(p.potion <= 0){
                        cout<<"No potions left...";
                        break;
                    }
                    cout<<"hp : +20";
                    p.heal(20);
                    p.potion--;
                }else if(ph==2){
                    if(p.super_potion <= 0){
                        cout<<"No super potions left...";
                        break;
                    }
                    cout<<"hp : +50";
                    p.heal(50);
                    p.super_potion--;
                }else if(ph==3){
                    if(p.max_potion <= 0){
                        cout<<"No max potions left...";
                        break;
                    }
                    cout<<"hp : +"<<p.max_hp-p.hp;
                    p.heal(p.max_hp-p.hp);
                    p.max_potion--;
                }
            }else{
                cout<<"Heal";
                gotoxy(1,15);
                if(ph==1){
                    if(p.potion <= 0){
                        cout<<"No potions left...";
                        break;
                    }
                    cout<<"hp : +20";
                    p.heal(20);
                    p.potion--;
                }else if(ph==2){
                    if(p.super_potion <= 0){
                        cout<<"No super potions left...";
                        break;
                    }
                    cout<<"hp : +50";
                    p.heal(50);
                    p.super_potion--;
                }else if(ph==3){
                    if(p.max_potion <= 0){
                        cout<<"No max potions left...";
                        break;
                    }
                    cout<<"hp : +"<<p.max_hp-p.hp;
                    p.heal(p.max_hp-p.hp);
                    p.max_potion--;
                }
                e.heal();
                gotoxy(65,15);
                cout<<"hp : +20";
            }
            break;
        default:
            gotoxy(1,10);
            cout<<"Confused";
            gotoxy(65,10);
            if(em==1){
                cout<<"Attack";
                p.hit(e.attack);
                gotoxy(1,15);
                cout<<"hp: -"<<e.attack;
            }else if(em==2){
                cout<<"Defend";
            }else{
                cout<<"Heal";
                e.heal();
                gotoxy(65,15);
                cout<<"hp : +20";
            }
            break;
        }
        getch();
    }
    if(p.hp<=0){
        system("cls");
        gotoxy(40,12);
        cout<<"You lose...";
        getch();
        return false;
    }else if(e.hp<=0){
        system("cls");
        gotoxy(40,12);
        cout<<"You win...";
        int exp_gain = (int)(sqrt(e.level)*10);
        int money_gain = (int)(sqrt(e.level)*20);
        if(p.exp+exp_gain >= p.next_exp && p.level<100){
            gotoxy(40,14);
            cout<<"Congratulations, you level up...";
        }
        if(isOnQuest){
            if(p.level<100){
                p.exp_up(exp_gain*2);
                gotoxy(40,13);
                cout<<"exp : +"<<exp_gain*2;
            }
            gotoxy(40,16);
            cout<<"money: +"<<money_gain*2;
            p.money += money_gain*2;
        }else{
            if(p.level<100){
                p.exp_up(exp_gain);
                gotoxy(40,13);
                cout<<"exp : +"<<exp_gain;
            }
            gotoxy(40,16);
            cout<<"money: +"<<money_gain;
            p.money += money_gain;
        }
        getch();
        return true;
    }
}

void shop_display(string name, string description, int cost, int y){
    gotoxy(1,y);
    cout<<name;
    gotoxy(30,y);
    cout<<description;
    gotoxy(70,y);
    cout<<"$"<<cost;
}

void shop(player &p){
    system("cls");
    int item;
    while(item != 0){
        system("cls");
        gotoxy(10,1);
        cout<<"Welcome to the shop. Select an item to buy. Press 0 to exit.";
        gotoxy(60,20);
        cout<<"Money Left : "<<p.money;
        shop_display("1. Potion","Restores 20 hp",500,5);
        shop_display("2. Super Potion","Restores 50 hp",1000,7);
        shop_display("3. Max Potion","Restores full hp",2000,9);
        item = getch();
        item -= 48;
        gotoxy(10,20);
        switch(item){
        case 1:
            if(p.money-500 < 0){
                cout<<"Sorry you don't have enough money...";
                getch();
                break;
            }
            p.potion++;
            p.money -= 500;
            cout<<"Potion + 1";
            getch();
            break;
        case 2:
            if(p.money-1000 < 0){
                cout<<"Sorry you don't have enough money...";
                getch();
                break;
            }
            p.super_potion++;
            p.money -= 1000;
            cout<<"Super Potion + 1";
            getch();
            break;
        case 3:
            if(p.money-2000 < 0){
                cout<<"Sorry you don't have enough money...";
                getch();
                break;
            }
            p.max_potion++;
            p.money -= 2000;
            cout<<"Max Potion + 1";
            getch();
            break;
        default:
            if(item == 0)
                break;
            cout<<"Wrong Choice";
            getch();
        }
    }
    free_roam(p);
}

void wild(player &p){
    int choice;
    while(choice != 2){
        system("cls");
        gotoxy(20,1);
        cout<<"A wild monster appeared";
        gotoxy(0,2);
        for(i=0;i<80;i++)
            cout<<"_";
        gotoxy(1,5);
        cout<<"1. Fight";
        gotoxy(1,7);
        cout<<"2. Run";
        gotoxy(1,15);
        choice = getch();
        choice -= 48;
        switch(choice){
        case 2:
            if(!place.compare("forest"))
                forest(p);
            else if(!place.compare("dungeon"))
                dungeon(p);
            break;
        default:
            cout<<"Wrong choice";
            getch();
            break;
        case 1:
            int el;
            srand(time(NULL));
            if(!place.compare("forest")){
                el = rand()%p.level + 1;
                if(el>100)
                    el = 100;
            }else if(!place.compare("dungeon")){
                el = rand()%100 + p.level;
                if(el>100)
                    el = 100;
            }
            enemy e(el);
            bool result = match(p,e);
            if(!result){
                x = 20;
                y = 11;
                p.hp = p.max_hp;
                free_roam(p);
            }else
                if(!place.compare("forest"))
                    forest(p);
                else if(!place.compare("dungeon"))
                    dungeon(p);
            break;
        }
    }
}

void quests(player &p){
    int choice;
    while(choice != 0){
        system("cls");
        gotoxy(40,1);
        cout<<"Quests Selector";
        gotoxy(0,2);
        for(i=0;i<80;i++)
            cout<<"_";
        gotoxy(1,5);
        cout<<"Select the level of quest you want to select ( 1 to 20 ) : ";
        cin>>choice;
        gotoxy(1,7);
        if(choice>0 && choice<21 && !questCompleted[choice-1]){
            questLevel = choice-1;
            isOnQuest = true;
            cout<<"Defeat the Leader of monsters in the ";
            if(questLevel>=0 && questLevel<=9)
                cout<<"Forest";
            else if(questLevel>=10 && questLevel<=19)
                cout<<"Dungeon";
            getch();
            free_roam(p);
        }else if(choice == 0){
            free_roam(p);
        }else{
            if(questCompleted[choice-1])
                cout<<"Quest already completed";
            else
                cout<<"Wrong Choice";
            getch();
        }
    }
}

void dungeon(player &p){
    system("cls");
    place = "dungeon";
    char c;
    gotoxy(1,23);
    cout<<"hp : ";
    for(i=0;i<p.hp*20/p.max_hp;i++)
        cout<<"*";
    gotoxy(26,23);
    cout<<"("<<p.hp<<"/"<<p.max_hp<<")";

    gotoxy(1,24);
    cout<<"exp : ";
    for(i=0;i<p.exp*20/p.next_exp;i++)
        cout<<"*";
    gotoxy(27,24);
    cout<<"("<<p.exp<<"/"<<p.next_exp<<")";
    gotoxy(60,24);
    cout<<"Level : "<<p.level;
    gotoxy(0,22);
    for(i=0;i<80;i++)
        cout<<"_";
    gotoxy(79,3);
    cout<<"T";
    if(isOnQuest && questLevel>=10){
        gotoxy(5,15);
        cout<<"Q";
    }
    while(c != 27){
        int t = 1;
        while(!kbhit()){
            srand(time(NULL));
            t = rand()%100;
            if(t%5 == 0){
                wild(p);
            }
        }
        gotoxy(x,y);
        cout<<"*";
        c=getch();
        if(c==KEY_UP)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y-1);
        cout<<"*";
        y=y-1;
        }
        else if(c==KEY_DOWN)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y+1);
        cout<<"*";
        y=y+1;
        }
        else if(c==KEY_LEFT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x-1,y);
        cout<<"*";
        x=x-1;
        }
        else if(c==KEY_RIGHT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x+1,y);
        cout<<"*";
        x=x+1;
        }
        else if(c=='i')
            inventory(p);
        else if(c=='k'){
            ofstream of("save.txt");
            of<<p.level<<' '<<p.exp<<' '<<p.potion<<' '<<p.super_potion<<' '<<p.max_potion<<' '<<p.money;
            of.close();
        }
        if(x > 79){
            gotoxy(0,y+1);
            cout<<" ";
            x = 0;
        }
        if(x < 0){
            gotoxy(1,y);
            cout<<" ";
            x = 79;
        }
        if(y > 21){
            gotoxy(x,22);
            cout<<"_";
            y = 0;
        }
        if(y < 0){
            gotoxy(x+1,0);
            cout<<" ";
            y = 21;
        }
        if(x==79 && y==3){
            x = 1;
            free_roam(p);
        }
        if(x==5 && y==15 && isOnQuest && questLevel>=10){
            enemy e(quest[questLevel]);
            bool result = match(p,e);
            if(!result){
                p.hp = p.max_hp;
                x = 20;
                y = 11;
                isOnQuest = false;
                free_roam(p);
            }else{
                questCompleted[questLevel] = true;
                isOnQuest = false;
                dungeon(p);
            }
        }
    }
}

void forest(player &p){
    system("cls");
    place = "forest";
    char c;
    gotoxy(1,23);
    cout<<"hp : ";
    for(i=0;i<p.hp*20/p.max_hp;i++)
        cout<<"*";
    gotoxy(26,23);
    cout<<"("<<p.hp<<"/"<<p.max_hp<<")";

    gotoxy(1,24);
    cout<<"exp : ";
    for(i=0;i<p.exp*20/p.next_exp;i++)
        cout<<"*";
    gotoxy(27,24);
    cout<<"("<<p.exp<<"/"<<p.next_exp<<")";
    gotoxy(60,24);
    cout<<"Level : "<<p.level;
    gotoxy(0,22);
    for(i=0;i<80;i++)
        cout<<"_";
    gotoxy(0,3);
    cout<<"T";
    if(isOnQuest && questLevel<=9){
        gotoxy(75,15);
        cout<<"Q";
    }
    while(c != 27){
        int t = 1;
        while(!kbhit()){
            srand(time(NULL));
            t = rand()%100;
            if(t%5 == 0){
                wild(p);
            }
        }
        gotoxy(x,y);
        cout<<"*";
        c=getch();
        if(c==KEY_UP)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y-1);
        cout<<"*";
        y=y-1;
        }
        else if(c==KEY_DOWN)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y+1);
        cout<<"*";
        y=y+1;
        }
        else if(c==KEY_LEFT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x-1,y);
        cout<<"*";
        x=x-1;
        }
        else if(c==KEY_RIGHT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x+1,y);
        cout<<"*";
        x=x+1;
        }
        else if(c=='i')
            inventory(p);
        else if(c=='k'){
            ofstream of("save.txt");
            of<<p.level<<' '<<p.exp<<' '<<p.potion<<' '<<p.super_potion<<' '<<p.max_potion<<' '<<p.money;
            of.close();
        }
        if(x > 79){
            gotoxy(0,y+1);
            cout<<" ";
            x = 0;
        }
        if(x < 0){
            gotoxy(1,y);
            cout<<" ";
            x = 79;
        }
        if(y > 21){
            gotoxy(x,22);
            cout<<"_";
            y = 0;
        }
        if(y < 0){
            gotoxy(x+1,0);
            cout<<" ";
            y = 21;
        }
        if(x==0 && y==3){
            x = 78;
            free_roam(p);
        }
        if(x==75 && y==15 && isOnQuest && questLevel<=9){
            enemy e(quest[questLevel]);
            bool result = match(p,e);
            if(!result){
                p.hp = p.max_hp;
                x = 20;
                y = 11;
                isOnQuest = false;
                free_roam(p);
            }else{
                questCompleted[questLevel] = true;
                isOnQuest = false;
                forest(p);
            }
        }
    }
}

void free_roam(player &p){
    system("cls");
    place = "town";
    char c;
    gotoxy(1,23);
    cout<<"hp : ";
    for(i=0;i<p.hp*20/p.max_hp;i++)
        cout<<"*";
    gotoxy(26,23);
    cout<<"("<<p.hp<<"/"<<p.max_hp<<")";

    gotoxy(1,24);
    cout<<"exp : ";
    for(i=0;i<p.exp*20/p.next_exp;i++)
        cout<<"*";
    gotoxy(27,24);
    cout<<"("<<p.exp<<"/"<<p.next_exp<<")";
    gotoxy(60,24);
    cout<<"Level : "<<p.level;
    gotoxy(0,22);
    for(i=0;i<80;i++)
        cout<<"_";
    gotoxy(20,10);
    cout<<"H";
    gotoxy(40,10);
    cout<<"S";
    gotoxy(79,3);
    cout<<"F";
    gotoxy(0,3);
    cout<<"D";
    gotoxy(60,10);
    cout<<"Q";
    while(c != 27){
        gotoxy(x,y);
        cout<<"*";
        c=getch();
        if(c==KEY_UP)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y-1);
        cout<<"*";
        y=y-1;
        }
        else if(c==KEY_DOWN)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x,y+1);
        cout<<"*";
        y=y+1;
        }
        else if(c==KEY_LEFT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x-1,y);
        cout<<"*";
        x=x-1;
        }
        else if(c==KEY_RIGHT)
        {gotoxy(x,y);
        cout<<" ";
        gotoxy(x+1,y);
        cout<<"*";
        x=x+1;
        }
        else if(c=='i')
            inventory(p);
        else if(c=='k'){
            ofstream of("save.txt");
            of<<p.level<<' '<<p.exp<<' '<<p.potion<<' '<<p.super_potion<<' '<<p.max_potion<<' '<<p.money;
            of.close();
        }
        if(x > 79){
            gotoxy(0,y+1);
            cout<<" ";
            x = 0;
        }
        if(x < 0){
            gotoxy(1,y);
            cout<<" ";
            x = 79;
        }
        if(y > 21){
            gotoxy(x,22);
            cout<<"_";
            y = 0;
        }
        if(y < 0){
            gotoxy(x+1,0);
            cout<<" ";
            y = 21;
        }
        if(x==20 && y==10){
            system("cls");
            gotoxy(20,10);
            cout<<"Health restored...";
            p.hp = p.max_hp;
            y += 1;
            getch();
            free_roam(p);
        }
        if(x==40 && y==10){
            y += 1;
            shop(p);
        }
        if(x==79 && y==3){
            x = 1;
            forest(p);
        }
        if(x==0 && y==3){
            x = 78;
            dungeon(p);
        }
        if(x==60 && y==10){
            y += 1;
            quests(p);
        }
    }
}

void inventory(player &p){
    int item;
    while(item != 0){
        system("cls");
        gotoxy(1,0);
        cout<<"hp : ";
        for(i=0;i<p.hp*20/p.max_hp;i++)
            cout<<"*";
        gotoxy(26,0);
        cout<<"("<<p.hp<<"/"<<p.max_hp<<")";

        gotoxy(1,1);
        cout<<"exp : ";
        for(i=0;i<p.exp*20/p.next_exp;i++)
            cout<<"*";
        gotoxy(27,1);
        cout<<"("<<p.exp<<"/"<<p.next_exp<<")";

        gotoxy(1,2);
        cout<<"level : "<<p.level;

        gotoxy(1,3);
        cout<<"attack : "<<p.attack;

        gotoxy(1,4);
        cout<<"defence : "<<p.defence;

        gotoxy(60,3);
        cout<<"Money : "<<p.money;

        gotoxy(0,5);
        for(i=0;i<80;i++)
            cout<<"_";

        gotoxy(1,7);
        cout<<"1.Potion ("<<p.potion<<")\t2.Super Potion ("<<p.super_potion<<")\t3.Max Potion ("<<p.max_potion<<")";
        gotoxy(1,15);
        item = getch();
        item -= 48;
        switch(item){
        case 1:
            if(p.potion <= 0){
                cout<<"No potions left...";
                getch();
                break;
            }
            p.heal(20);
            p.potion--;
            cout<<"hp : +20";
            getch();
            break;
        case 2:
            if(p.super_potion <= 0){
                cout<<"No super potions left...";
                getch();
                break;
            }
            p.heal(50);
            p.super_potion--;
            cout<<"hp : +50";
            getch();
            break;
        case 3:
            if(p.max_potion <= 0){
                cout<<"No max potions left...";
                getch();
                break;
            }
            p.heal(p.max_hp-p.hp);
            p.max_potion--;
            cout<<"hp : +"<<p.max_hp-p.hp;
            getch();
            break;
        default:
            if(item==0)
                break;
            cout<<"Wrong Choice";
            getch();
        }
    }
    if(!place.compare("town"))
        free_roam(p);
    else if(!place.compare("forest"))
        forest(p);
    else if(!place.compare("dungeon"))
        dungeon(p);
}

bool fexists(const string& filename) {
  ifstream ifile(filename.c_str());
  return ifile;
}

int* intro(){
    while(1){
        system("cls");
        int item;
        gotoxy(25,1);
        cout<<"Welcome to Ichiban Bushi...";
        gotoxy(0,2);
        for(i=0;i<80;i++)
            cout<<"_";
        gotoxy(1,7);
        cout<<"1. Continue";
        gotoxy(1,9);
        cout<<"2. New Game";
        gotoxy(1,15);
        item = getch();
        item -= 48;
        switch(item){
        default:
            cout<<"Wrong Choice...";
            getch();
            break;
        case 1:
            if(!fexists("save.txt")){
                cout<<"There is no saved game...";
                getch();
                break;
            }else{
                int level,exp,potion,super_potion,max_potion,money;
                ifstream iff("save.txt");
                while(iff>>level>>exp>>potion>>super_potion>>max_potion>>money){
                    int sav[] = {level,exp,potion,super_potion,max_potion,money};
                    return sav;
                }
                iff.close();
            }
            break;
        case 2:
            player p(5);
            ofstream of("save.txt");
            of<<p.level<<' '<<p.exp<<' '<<p.potion<<' '<<p.super_potion<<' '<<p.max_potion<<' '<<p.money;
            int sav[] = {p.level,p.exp,p.potion,p.super_potion,p.max_potion,p.money};
            of.close();
            return sav;
            break;
        }
    }
}

int main(){
    system("cls");
    quest[0] = 5;
    for(i=1;i<20;i++){
        quest[i] = quest[i-1] + 5;
        questCompleted[i-1] = false;
    }
    int *ptr = intro();
    player p(*ptr);
    p.exp = *(ptr+1);
    p.potion = *(ptr+2);
    p.super_potion = *(ptr+3);
    p.max_potion = *(ptr+4);
    p.money = *(ptr+5);
    free_roam(p);
}
