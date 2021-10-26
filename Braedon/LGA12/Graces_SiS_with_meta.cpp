#include<iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cmath>
#include <math.h>

using namespace std;

enum EventType { REVIEW, DEMAND, DELIVERY, META_A, META_B };

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
    // Meta statistics
    double a_mean = 0;
    double a_v = 0;
    double b_mean = 0;
    double b_v = 0;
    int a_i = 1;
    int b_i = 1;
    int b_i_last_time_jump = -1;

    while (eventList.size() > 0) {
        sort(eventList.begin(), eventList.end(), compareEvent);
        Event event = eventList.front();
        // create event 3 days earlier IF:
        //     the current event is a review and it's going to result in an order
        //     and if the current value of b_i isn't the value of b_i that was last seen by this function
        //     plus 1 (since we only want b_i to be intterated once per order)
        if (event.type == REVIEW && inventory+onOrder <= s && b_i != b_i_last_time_jump + 1) {   
            b_i_last_time_jump = b_i;
            Event threeDaysBeforeOrder = {event.at - 3, META_B};
            eventList.push_back(threeDaysBeforeOrder);
            continue;
        }
            

        eventList.erase(eventList.begin());
        t = event.at;
        if (event.type == REVIEW) {
            Event nextReview = {t+7, REVIEW};
            if (nextReview.at < T) {
                eventList.push_back(nextReview);
            }
            if (inventory+onOrder <= s) {
                Event deliveryEvent = {t + Exponential(7), DELIVERY, S-(inventory+onOrder)};
                Event threeDaysAfterOrder = {t + 3, META_A};
                onOrder += deliveryEvent.count;
                eventList.push_back(deliveryEvent);
                eventList.push_back(threeDaysAfterOrder);
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
        } else if (event.type == META_A) {
            a_i++; 
            a_mean = a_mean + (1.0/a_i) * (inventory - a_mean);
            a_v = a_v + ((a_i - 1.0)/a_i) * pow(inventory - a_mean, 2);
        } else if (event.type == META_B) {
            b_i++;
            b_mean = b_mean + (1.0/b_i) * (inventory - b_mean);
            b_v = b_v + ((b_i - 1.0)/b_i) * pow(inventory - b_mean, 2);
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
    cout << "a mean: " << a_mean << endl;
    cout << "a std dev: " << sqrt(a_v / a_i) << endl;
    cout << "b mean: " << b_mean << endl;
    cout << "b std dev: " << sqrt(b_v / b_i) << endl;
    cout << "backorderCount: " << backorderCount << endl;

}
