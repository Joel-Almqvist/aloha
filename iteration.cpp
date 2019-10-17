#include "iteration.h"
#include "node.h"
#include <iostream>
using namespace std;

ItData::ItData(){
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
