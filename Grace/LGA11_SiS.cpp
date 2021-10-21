#include<iostream>
#include <string>
#include <random>
using namespace std;

enum EventType { REVIEW, DEMAND, DELIVERY };

struct Event
{
    double at;
    EventType type;
    int count;
};

double Exponential(double m) {
    return (-m * log(1.0 - (float) rand()/RAND_MAX));
}

int Geometric(double p) {
    return ((int) (log(1.0 - (float) rand()/RAND_MAX) / log(p))); 
}

bool compareEvent(Event e1, Event e2)
{
    return (e1.at < e2.at);
}


int main() {
    vector<Event> eventList;
    int s = 20;
    int S = 80;
    double t = 0; // simulation clock
    int T = 1000; // no new events after T
    // SiS specific state
    int inventory = S;
    int onOrder = 0;
    // for reporting
    int backorderCount = 0; // number of cars purchased when inventory <= 0
    int orderCount = 0;
    // schedule an inventory review at 7 days
    Event firstReview = {7, REVIEW};
    eventList.push_back(firstReview);
    // schedule a first demand arrival
    Event firstDemand = {Exponential((float) 2/3), DEMAND};
    eventList.push_back(firstDemand);

    while (eventList.size() > 0) {
        sort(eventList.begin(), eventList.end(), compareEvent);

        Event event = eventList.front();
        eventList.erase(eventList.begin());
        t = event.at;
        if (event.type == REVIEW) {
            Event nextReview = {t+7, REVIEW};
            if (nextReview.at < T) {
                eventList.push_back(nextReview);
            }
            if (inventory+onOrder <= s) {
                Event deliveryEvent = {t + Exponential(7), DELIVERY, S-(inventory+onOrder)};
                onOrder += deliveryEvent.count;
                eventList.push_back(deliveryEvent);
                orderCount++;
                
            }
        } else if (event.type == DELIVERY) {
            onOrder -= event.count;
            inventory += event.count;
        } else if (event.type == DEMAND) {
            Event nextDemand = {t + Exponential((float) 2/3), DEMAND};
            if (nextDemand.at < T) {
                eventList.push_back(nextDemand);
            }

            int purchased = Geometric((float) 3/4);
            if (purchased > 0) {
                
                if (inventory >= purchased) {
                    inventory -= purchased;
                } else {
                    if (inventory > 0) {
                        purchased -= inventory;
                        inventory = 0;
                    }
                    inventory -= purchased;
                    backorderCount += purchased;
                }
            }
        }
    }

    if (inventory < S) {
        if (inventory < 0) {
            backorderCount += inventory;
        }
        orderCount++;
        t += Exponential(7);
    }

    cout << "t: " << t << endl;
    cout << "orderCount: " << orderCount << endl;
    cout << "backorderCount: " << backorderCount << endl;



}