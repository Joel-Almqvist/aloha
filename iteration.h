#ifndef ITERATION_H
#define ITERATION_H

#include <string>

using namespace std;

class ItData{
  public:
    int id;
    int backlogSize;
    int oldBacklogSize;
    int channelState;
    int arrivals;
    int acceptedArrivals;
    double backlogEst;

    ItData();
    ~ItData();

    void print();
};
#endif // ITERATION_H
