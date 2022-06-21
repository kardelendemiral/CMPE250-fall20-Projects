#include "Hacker.h"
Hacker::Hacker(int id_, float arrivalTime_) {
    this->id=id_;
    this->arrivalTime=arrivalTime_;
    this->codeCommits=0;
    this->lastQueueEntrance=0;
    this->gifts=0;
    this->totalTimeWaited=0;
    this->lastTurnaroundBegin=0;
}
Hacker& Hacker:: operator=(const Hacker& hacker){
    if(this==&hacker){
        return *this;
    }

    this->id=hacker.id;
    this->arrivalTime=hacker.arrivalTime;
    this->codeCommits=hacker.codeCommits;
    this->gifts=hacker.gifts;
    this->lastQueueEntrance=hacker.lastQueueEntrance;
    this->lastTurnaroundBegin=hacker.lastTurnaroundBegin;
    this->totalTimeWaited=hacker.totalTimeWaited;
    return *this;

}