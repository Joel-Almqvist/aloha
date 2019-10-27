/* Written by Joel Almqvist (joeal360) for TSIN01
*/
#ifndef ITERATION_H
#define ITERATION_H

#include <string>

using namespace std;


// This file collects a data generated from one iteration (IE slot)
// of the algorithm. It bascially a struct with an overloaded print function.
class ItData{
  public:
    int id;
    int backlogSize;
    int oldBacklogSize;
    int channelState;
    int arrivals;
    int acceptedArrivals;
    double backlogEst;
    int slotsWaited;

    ItData();
    ~ItData();

    void print();
};
#endif // ITERATION_H
