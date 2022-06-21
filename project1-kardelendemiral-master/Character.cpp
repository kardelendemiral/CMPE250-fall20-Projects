#include "Character.h"
Character::Character(string _name, string _type, int _attack, int _defense, int _remainingHealth, int _nMaxRounds) {
this->name=_name;
this->type=_type;
this->attack=_attack;
this->defense=_defense;
this->remainingHealth=_remainingHealth;
this->nMaxRounds=_nMaxRounds;
this->healthHistory= new int[_nMaxRounds+1];
this->nRoundsSinceSpecial=0;
this->healthHistory[0]=_remainingHealth;

}
//copy constructor of the class 
Character::Character(const Character& character) {
    this->name=character.name;
    this->type=character.type;
    this->attack=character.attack;
    this->defense=character.defense;
    this->remainingHealth=character.remainingHealth;
    this->nRoundsSinceSpecial=character.nRoundsSinceSpecial;
    this->healthHistory= new int[character.nMaxRounds+1];
    this->nMaxRounds=character.nMaxRounds;
    this->isAlive=character.isAlive;

    for(int i=0;i<character.nMaxRounds+1;i++){
        this->healthHistory[i]=character.healthHistory[i];
    }

}
//= operator overloader of the class
Character& Character::operator=(const Character& character) {
    if(this==&character){
        return *this;
    }
    if(this->healthHistory != NULL){
        delete[] healthHistory;

        }

    this->name=character.name;
    this->type=character.type;
    this->attack=character.attack;
    this->defense=character.defense;
    this->remainingHealth=character.remainingHealth;
    this->nRoundsSinceSpecial=character.nRoundsSinceSpecial;
    this->healthHistory= new int[character.nMaxRounds+1];
    this->nMaxRounds=character.nMaxRounds;
    this->isAlive=character.isAlive;

     for(int i=0;i<character.nMaxRounds+1;i++){
        this->healthHistory[i]=character.healthHistory[i];
    }
    return *this;


}

//compares two characters according to their name
bool Character::operator<(const Character& other) {
    if(this->name<other.name){
        return true;
    }
    return false;
}

Character::~Character() {
    if(this->healthHistory!=NULL){
        delete[] this->healthHistory;
    }

}


