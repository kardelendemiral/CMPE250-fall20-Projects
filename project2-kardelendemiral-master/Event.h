#ifndef EVENT_H
#define EVENT_H

#include <string>


using namespace std;

class Event {
public:
    string type;
    float time;
    int id;
    int data;


    Event(string type_, float time_, int id_);
    Event(string type_, float time_, int id_, int data_);
    Event(const Event& event);
    Event& operator=(const Event& character);


};


#endif //EVENT_H