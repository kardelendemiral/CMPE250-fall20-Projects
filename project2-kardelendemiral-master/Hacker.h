#ifndef PROJECT2_HACKER_H
#define PROJECT2_HACKER_H


class Hacker {
public:
    float arrivalTime;
    int codeCommits;
    int id;
    float lastQueueEntrance;
    int gifts;
    float totalTimeWaited;
    float lastTurnaroundBegin;


    Hacker(int id_ ,float arrivalTime_);
    Hacker& operator=(const Hacker& character);

};


#endif //PROJECT2_HACKER_H