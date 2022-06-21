#include "Event.h"


Event::Event(string type_, float time_, int id_) {
    this->type=type_;
    this->time=time_;
    this->id=id_;
}

Event::Event(string type_,float time_, int id_,int data_){
    this->type=type_;
    this->time=time_;
    this->data=data_;
    this->id=id_;
}

Event::Event(const Event& event){
    this->type=event.type;
    this->time=event.time;
    this->data=event.data;
    this->id=event.id;
}

Event& Event::operator=(const Event& event) {
    if(this==&event){
        return *this;
    }

    this->time=event.time;
    this->type=event.type;
    this->data=event.data;
    this->id=event.id;
    return *this;


}