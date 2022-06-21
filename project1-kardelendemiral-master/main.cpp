#include "Character.h"
#include <fstream>
#include <stack>
#include <iostream>
using namespace std;

//returns the pointer of a character with the given name
Character& getCharacter(const string name,Character* community[5]){
    for(int i=0;i<5;i++){
        if(community[i]->name==name){
            return *community[i];
        }
    }
    return *community[0];
}

//finds the next alive character to be replaced with the dead attacker or defender, returns its index
int findNextAlive(string name, Character* community[5], string sortedNames[5]){ 
    int index=0;
    for(;index<5;index++){
        if(sortedNames[index]==name){
            break;
        }
    }

    while(!getCharacter(sortedNames[index],community).isAlive && index<5){
        index++;
    }

    if(index==5){
        index--;
        while(!getCharacter(sortedNames[index],community).isAlive && index>=0){
            index--;
        }
    }

    for(int i=0;i<5;i++){
        if(community[i]->name==sortedNames[index]){
            return i;
        }
    }

    return -1;

}
// returns the index of the hobbit of thw community
int findHobbit(Character* community[5]){ //INDEX DE RETURN EDEBİLİR
    for(int i=0;i<5;i++){
        if(community[i]->type=="Hobbit"){
            return i;
        }
    }
    return 0;

}
//makes the character alive with the given name, makes its nRoundsSpecial 0
void revive(string &name, Character* community[5], int &casualties){
    for(int i=0;i<5;i++){
        if(community[i]->name==name){
            if(!community[i]->isAlive){
                community[i]->nRoundsSinceSpecial=0;
             }
            community[i]->isAlive=true;
            community[i]->remainingHealth=community[i]->healthHistory[0];
            casualties--;
            break;
        }
    }

}

int main(int argc, char* argv[]) {

    int nMaxRounds;
    string winner="Draw";
    int casualties=0;
    int nRoundsPassed=0;

    //stacks to store dead members' names so that the last dead character can revive
    stack<string> community1Killed;
    stack<string> community2Killed;

    Character* community1[5];
    Character* community2[5];

    // names arrays to be sorted to determine which character will be replaces to the dead character
    string sortedNames1[5];
    string sortedNames2[5];

    string infileName= argv[1];
    string outfileName= argv[2];

    ifstream infile;
    ofstream outfile;

    infile.open(infileName);
    outfile.open(outfileName);

    infile >> nMaxRounds;


    //first community created
    for(int i=0;i<5;i++){
        string name;
        string type;
        int attack;
        int defense;
        int health;
        infile >> name >> type >> attack >> defense >> health;
        community1[i] = new Character(name,type,attack,defense,health,nMaxRounds);
        sortedNames1[i]=name;
    }
    //second community created
    for(int i=0;i<5;i++){
        string name;
        string type;
        int attack;
        int defense;
        int health;
        infile >> name >> type >> attack >> defense >> health;
        community2[i] = new Character(name,type,attack,defense,health,nMaxRounds);
        sortedNames2[i]=name;
    }
    //name arrays are sorted
    for(int i=0;i<4;i++){
        for(int j=0;j<4-i;j++){
            if(sortedNames1[j]>sortedNames1[j+1]){
                string tmp=sortedNames1[j];
                sortedNames1[j]=sortedNames1[j+1];
                sortedNames1[j+1]=tmp;
            }
            if(sortedNames2[j]>sortedNames2[j+1]){
                string tmp=sortedNames2[j];
                sortedNames2[j]=sortedNames2[j+1];
                sortedNames2[j+1]=tmp;
            }
        }
    }
    //main loop executing rounds
    for(int round=1;round<=nMaxRounds;round++){
        nRoundsPassed++;
        string attackerName;
        string defenderName;
        string specialString;
        infile >> attackerName >> defenderName >> specialString;
        bool attackerIsFrom1;

        if(round%2==1){
            attackerIsFrom1=true;
        } else {
            attackerIsFrom1=false;
        }

        bool special;
        if(specialString=="SPECIAL"){
            special=true;
        } else {
            special=false;
        }

        Character* attacker;
        Character* defender;
        //determine the attacker and the defender charaters, assign them to the pointers
         for(int j=0;j<5;j++){
            if(community1[j]->name==attackerName){
                attacker = community1[j];
            } else if(community1[j]->name==defenderName){
                defender = community1[j];
            }
        }

        for(int j=0;j<5;j++){
            if(community2[j]->name==attackerName){
                attacker = community2[j];
            } else if(community2[j]->name==defenderName){
                defender = community2[j];
            }
        }

        //attacker is dead
        if(!attacker->isAlive){
            int index;
            if(attackerIsFrom1){
                index=findNextAlive(attacker->name,community1,sortedNames1);
                attacker=community1[index];
            } else {
                index=findNextAlive(attacker->name,community2,sortedNames2);
                attacker=community2[index];
            }
        }

         //defender is dead
        if(!defender->isAlive){
            int index;
            if(attackerIsFrom1){
                index=findNextAlive(defender->name,community2,sortedNames2);
                defender=community2[index];
            } else {
                index=findNextAlive(defender->name,community1,sortedNames1);
                defender=community1[index];
            }
        }

        //if special skill is triggered, (here if the character uses its special skill its nRoundsSpeacial becomes -1 since it will be incremented at the end and become 0
        bool damageDoubled=false;
        if(special){
            if(attacker->type=="Elves" && attacker->nRoundsSinceSpecial>=10){
                attacker->nRoundsSinceSpecial=-1;
                Character* hobbit;   
                if(attackerIsFrom1){
                    hobbit=community1[findHobbit(community1)];
                    } else {
                    hobbit=community2[findHobbit(community2)];
                }
                hobbit->remainingHealth+=attacker->remainingHealth/2;
                attacker->remainingHealth=attacker->remainingHealth/2;

                } else if(attacker->type=="Dwarfs" && attacker->nRoundsSinceSpecial>=20){
                attacker->nRoundsSinceSpecial=-1;
                damageDoubled=true;

            } else if(attacker->type=="Wizards" && attacker->nRoundsSinceSpecial>=50){
                if(attackerIsFrom1){
                    if(!community1Killed.empty()){
                        string revivedName = community1Killed.top();
                        revive(revivedName,community1,casualties);
                        attacker->nRoundsSinceSpecial=-1;
                    }
                    } else {
                    if(!community2Killed.empty()){
                        string revivedName = community2Killed.top();
                        revive(revivedName,community2,casualties);
                        attacker->nRoundsSinceSpecial=-1;
                    }
                }

            }

        }

        //damage is calculated
        int damage = attacker->attack-defender->defense;

        if(damageDoubled){
            damage = damage*2;
        }

        if(damage>=0){
            defender->remainingHealth-=damage;
        }

         //if the defender died
        bool hobbitIsDead=false;
        bool everyOneElseIsDead=false;

        if(defender->remainingHealth<=0){
            defender->remainingHealth=0;
            defender->isAlive=false;
            casualties++;
            if(attackerIsFrom1){
                community2Killed.push(defender->name);
                if(defender->type=="Hobbit"){
                    hobbitIsDead=true;
                    winner="Community-1";
                }
                int deadCount=0;
                for(int i=0;i<5;i++){
                    if(!community2[i]->isAlive){
                        deadCount++;
                    }
                }
                if(deadCount==4){
                    everyOneElseIsDead=true;
                    winner="Community-1";
                }
                 } else {
                community1Killed.push(defender->name);
                if(defender->type=="Hobbit"){
                    hobbitIsDead=true;
                    winner="Community-2";
                }
                int deadCount=0;
                for(int i=0;i<5;i++){
                    if(!community1[i]->isAlive){
                        deadCount++;
                    }
                }
                 if(deadCount==4){
                    everyOneElseIsDead=true;
                    winner="Community-2";
                }
            }
        }

        //upload every characters' health records since the round ended
         for(int i=0;i<5;i++){
            community1[i]->healthHistory[round]=community1[i]->remainingHealth;
            community1[i]->nRoundsSinceSpecial+=1;
            community2[i]->healthHistory[round]=community2[i]->remainingHealth;
            community2[i]->nRoundsSinceSpecial+=1;
        }
        //check the ending conditions
        if(hobbitIsDead||everyOneElseIsDead){ 
            break;
        }



    }

    //print the output
    outfile << winner << endl << nRoundsPassed << endl << casualties << endl;

    for(int i=0;i<5;i++){
        outfile << community1[i]->name << " ";
        for(int j=0;j<nRoundsPassed+1;j++){
            outfile << community1[i]->healthHistory[j] << " ";
        }
        outfile << endl;
    }

     for(int i=0;i<5;i++){
        outfile << community2[i]->name << " ";
        for(int j=0;j<nRoundsPassed+1;j++){
            outfile << community2[i]->healthHistory[j] << " ";
        }
        outfile << endl;
    }

    outfile.close();
    infile.close();
    //delete the objects
    for(int i=0;i<5;i++){
        delete community1[i];
        delete community2[i];
    }





    return 0;
}