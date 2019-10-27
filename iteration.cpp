/* Written by Joel Almqvist (joeal360) for TSIN01
*/
#include "iteration.h"
#include "node.h"
#include <iostream>
using namespace std;

// This file collects a data generated from one iteration (IE slot)
// of the algorithm. It bascially a struct with an overloaded print function.
ItData::ItData(){
  slotsWaited = -1;
}

ItData::~ItData(){}

void ItData::print(){
  cout << "Iteration " << id << endl;
  cout << "Channel status: " << strRes(channelState) << endl;
  cout << oldBacklogSize << " old backlog's size\n";
  cout << acceptedArrivals << " accepted arrivals\n";
  cout << backlogSize << " current backlog's size\n";
  cout << arrivals << " number of total arrivals\n\n";


}
