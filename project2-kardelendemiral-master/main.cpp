#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include "Hacker.h"
#include "Event.h"
#include "fstream"
#include <set>
#include <iomanip>
using namespace std;

//compares hackers in the sticker queue
struct CompareHackers2 {
    bool operator()(Hacker  * & h1, Hacker * & h2)
    {
        if(abs(h1->lastQueueEntrance-h2->lastQueueEntrance) < 0.00001){
            return h1->id > h2->id;
        }
        return h1->lastQueueEntrance > h2->lastQueueEntrance;

    }
};

//compares events
struct CompareEvents {
    bool operator()(Event  & e1, Event  & e2)
    {

        if(abs(e1.time-e2.time)<0.00001){
            return e1.id > e2.id;
        }
        return e1.time > e2.time;
    }
};

//compares hackers in hoodie queue
struct CompareHackers {
    bool operator()(Hacker  * & h1, Hacker * & h2)
    {
        if(h1->codeCommits!=h2->codeCommits){
            return h1->codeCommits < h2->codeCommits;
        }

        if(abs(h1->lastQueueEntrance - h2->lastQueueEntrance) < 0.00001){
            return h1->id > h2->id;

        }
        return h1->lastQueueEntrance > h2->lastQueueEntrance;


    }
};
int main(int argc, char* argv[]) {

    priority_queue<Event,vector<Event>,CompareEvents> heap;
    priority_queue<Hacker*, vector<Hacker*>, CompareHackers2> stickerQueue;
    priority_queue<Hacker*, vector<Hacker*>, CompareHackers> hoodieQueue;

    float totalCodeLength=0;
    float giftsGrabbed=0;
    int invalidInsufficientCommits=0;
    int invalidHadEnoughGifts=0;
    float maxStickerQueueLength=0;
    float maxHoodieQueueLength=0;
    float totalWaitingTimeSticker=0;
    float totalWaitingTimeHoodie=0;
    float totalTurnaroundTime=0;
    float hoodieQueueLength=0;
    float stickerQueueLength=0;
    float lastEventTime=0;



    string infileName= argv[1];
    string outfileName= argv[2];

    ifstream infile;
    ofstream outfile;

    infile.open(infileName);
    outfile.open(outfileName);

    int nOfHackers;
    infile >> nOfHackers;

   vector<Hacker> hackers;

    for (int i = 1; i < nOfHackers+1; i++) {
        float arrivalTime;
        infile >> arrivalTime;
        hackers.push_back(Hacker(i,arrivalTime));
    }

    int nOfCodeCommits;
    infile >> nOfCodeCommits;

    for (int i=0;i<nOfCodeCommits;i++){
        int id;
        int lines;
        float time;
        infile >> id >> lines >> time;

        totalCodeLength += lines;

        heap.push(Event("CodeCommit",time,id,lines));
    }

    int nOfEntranceAttempts;
    infile >> nOfEntranceAttempts;

    for(int i=0;i<nOfEntranceAttempts;i++){
        int id;
        float time;
        infile >>  id >> time;

        heap.push(Event("StickerQueueEntrance",time,id));
    }

    int nOfStickerDesks;
    infile >> nOfStickerDesks;
    float stickerDeskArr[nOfStickerDesks];
    set<pair<int,int>> stickerDeskSet;

    for (int i = 0; i < nOfStickerDesks; i++) {
        float serviceTime;
        infile >> serviceTime;

        stickerDeskArr[i]=serviceTime;
        stickerDeskSet.insert(make_pair(0,i));
    }

    int nOfHoodieDesks;
    infile >> nOfHoodieDesks;
    float hoodieDeskArr[nOfHoodieDesks];
    set<pair<int,int>> hoodieDeskSet;

    for (int i = 0; i < nOfHoodieDesks; i++) {
        float serviceTime;
        infile >> serviceTime;

        hoodieDeskArr[i]=serviceTime;
        hoodieDeskSet.insert(make_pair(0,i));
    }

    //main while loop
    while(!heap.empty()){
        Event e=heap.top();
        float time=e.time;
        int hackerId=e.id;

        Hacker *hacker=&hackers.at(hackerId-1);

        lastEventTime=time;


        heap.pop();

        if(e.type=="CodeCommit"){
            if(e.data>=20){
                hacker->codeCommits++;
            }

        } else if(e.type=="StickerQueueEntrance"){

            if(hacker->codeCommits<3){
                invalidInsufficientCommits++;
            } else if(hacker->gifts>=3){
                invalidHadEnoughGifts++;
            } else {
                hacker->lastTurnaroundBegin=time;
                if(stickerQueue.empty()&&stickerDeskSet.begin()->first==0){

                    int deskId=stickerDeskSet.begin()->second;
                    stickerDeskSet.erase(make_pair(0,deskId));
                    stickerDeskSet.insert(make_pair(1,deskId)); //the desk is filled

                    heap.push(Event("HoodieQueueEntrance",time+stickerDeskArr[deskId],hackerId,deskId));

                } else {
                    hacker->lastQueueEntrance=time;
                    stickerQueue.push(hacker);
                    stickerQueueLength++;
                    if(stickerQueueLength>maxStickerQueueLength){
                        maxStickerQueueLength=stickerQueueLength;
                    }
                }
            }

        } else if(e.type=="HoodieQueueEntrance"){

            int stickerDeskId=e.data;


            if(!stickerQueue.empty()){
                Hacker *h=stickerQueue.top();
                stickerQueueLength--;

                float waitingTime=time - h->lastQueueEntrance;
                totalWaitingTimeSticker += waitingTime;
                h->totalTimeWaited += waitingTime;

                stickerQueue.pop();

                heap.push(Event("HoodieQueueEntrance",time+stickerDeskArr[stickerDeskId],h->id,stickerDeskId));

            } else {
                stickerDeskSet.erase(make_pair(1,stickerDeskId));
                stickerDeskSet.insert(make_pair(0,stickerDeskId));
            }

            if(hoodieQueue.empty()&&hoodieDeskSet.begin()->first==0){
                int hoodieDeskId=hoodieDeskSet.begin()->second;

                hoodieDeskSet.erase(make_pair(0,hoodieDeskId));
                hoodieDeskSet.insert(make_pair(1,hoodieDeskId)); 

                heap.push(Event("HoodieQueueQuit",time+hoodieDeskArr[hoodieDeskId],hackerId,hoodieDeskId));


            } else {
                hacker->lastQueueEntrance=time;
                hoodieQueue.push(hacker);
                hoodieQueueLength++;
                if(hoodieQueueLength>maxHoodieQueueLength){
                    maxHoodieQueueLength=hoodieQueueLength;
                }

            }



        } else if (e.type=="HoodieQueueQuit"){
            int deskId = e.data;

            giftsGrabbed++;
            float turnaroundTime = time - hacker->lastTurnaroundBegin;
            totalTurnaroundTime  += turnaroundTime;
            hacker->gifts++;



            if(!hoodieQueue.empty()){
                Hacker *h = hoodieQueue.top();
                hoodieQueueLength--;

                float waitingTime = time - h->lastQueueEntrance;
                h->totalTimeWaited += waitingTime;
                totalWaitingTimeHoodie += waitingTime;

                hoodieQueue.pop();

                heap.push(Event("HoodieQueueQuit", time + hoodieDeskArr[deskId], h->id, deskId));

            } else {
                hoodieDeskSet.erase(make_pair(1,deskId));
                hoodieDeskSet.insert(make_pair(0,deskId));
            }

        }

    }


    float avGift= giftsGrabbed / (float) nOfHackers;
    float avWaitingSticker= totalWaitingTimeSticker / giftsGrabbed;
    float avWaitingHoodie= totalWaitingTimeHoodie /  giftsGrabbed;
    float avCodeCommit=   (float) nOfCodeCommits /  (float) nOfHackers;
    float avLengthCommits=  totalCodeLength / (float) nOfCodeCommits;
    float avTurnaroundTime=  totalTurnaroundTime /  giftsGrabbed;

    int mostWaitedHackerId=1;
    float mostTimeWaited=hackers.at(0).totalTimeWaited;

    //find most waited hacker
    for(int i=1;i<nOfHackers+1;i++){
        if(hackers.at(i-1).totalTimeWaited - mostTimeWaited > 0.00001){
            mostWaitedHackerId=i;
            mostTimeWaited=hackers.at(i-1).totalTimeWaited;
        }

    }

    int leastWaitedHackerId=-1;
    float leastTimeWaited=-1;

    for(int i=1;i<nOfHackers+1;i++){
        if(hackers.at(i-1).gifts==3){
            leastTimeWaited=hackers.at(i-1).totalTimeWaited;
            leastWaitedHackerId=i;
            break;
        }
    }

    //find the least waited hacker
    if(leastWaitedHackerId!=-1){
        for(int i=leastWaitedHackerId;i<nOfHackers+1;i++){
            if(leastTimeWaited - hackers.at(i-1).totalTimeWaited > 0.00001 && hackers.at(i-1).gifts==3){
                leastWaitedHackerId=i;
                leastTimeWaited=hackers.at(i-1).totalTimeWaited;
            }
        }
    }
    

    outfile << maxStickerQueueLength << endl << maxHoodieQueueLength << endl;
    outfile << fixed << setprecision(3) << avGift << endl;
    outfile << fixed << setprecision(3) << abs(avWaitingSticker) << endl;
    outfile << fixed << setprecision(3) << abs(avWaitingHoodie) << endl;
    outfile << fixed << setprecision(3) << avCodeCommit << endl;
    outfile << fixed << setprecision(3) << avLengthCommits  << endl;
    outfile << fixed << setprecision(3) << avTurnaroundTime << endl;
    outfile << invalidInsufficientCommits << endl << invalidHadEnoughGifts << endl << mostWaitedHackerId << " ";
    outfile << fixed << setprecision(3) << mostTimeWaited << endl;
    outfile << leastWaitedHackerId << " ";
    outfile << fixed << setprecision(3) << leastTimeWaited << endl;
    outfile << fixed << setprecision(3) << lastEventTime;

    return 0;

}