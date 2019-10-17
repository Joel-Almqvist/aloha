#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "node.h"
#include "iteration.h"
using namespace std;

const double DEFAULT_QR = 0.1;
// The chance for a single node to receive a packet this slot
const double QA = 0.05;
const int NR_OF_NODES = 50;
const int NR_OF_ITER = 20;


void createArrivals(vector<AlohaNode>* vecPtr, ItData* dataPtr){
  // TODO change how this randomness is calculated and used
  int posSpace = round(1/QA);
  int arrivals = 0;
  int receivedPackets = 0;
  int outcome;
  for(size_t i = 0; i < vecPtr->size();i++){
    outcome = rand();
    if(outcome % posSpace == 1){
      receivedPackets += (*vecPtr)[i].receivePacket();
      arrivals++;
    }
  }
  dataPtr->arrivals = arrivals;
  dataPtr->acceptedArrivals = receivedPackets;
}


void stepBacklog(vector<AlohaNode>* vecPtr){
  for(size_t i = 0; i < vecPtr->size();i++){
    (*vecPtr)[i].backlogTick();
  }
}


int getBacklog(vector<AlohaNode>* vecPtr){
  int counter = 0;
  for(size_t i = 0; i < vecPtr->size();i++){
    if((*vecPtr)[i].getState() == Status::BACKLOG){
      counter++;
    }
  }
  return counter;
}

// Changes the status of every transmitting node from [0, endIndex) to collided
void collide(vector<AlohaNode>* vecPtr, int endIndex){
  AlohaNode* currNode;
  for(size_t i = 0; i < endIndex; i++){
    currNode = &(*vecPtr)[i];
    if(currNode->isSending()){
      currNode->collide();
    }
  }
}

/* Make every node with state "transmitting" transmit and backlog them
 * if a collision occurs.
 * Returns a Response enum indicating the channel status.
*/
int transmit(vector<AlohaNode>* vecPtr){
  int channelState = Response::UNUSED;

  bool alreadySending = false;
  bool sorted = false;
  bool transmitting;
  AlohaNode* currNode;
  for(size_t i = 0; i < vecPtr->size(); i++){
    currNode = &(*vecPtr)[i];
    transmitting = currNode->isSending();


    if(transmitting && alreadySending){
      currNode->collide();
      if(channelState != Response::COL){
        channelState = Response::COL;
      }

      // We only want to sort one time due to performance reason
      if(!sorted){
        collide(vecPtr, i);
      }
    }
    else if(transmitting){
      channelState = Response::SUCCESS;
      alreadySending = true;
    }
  }
  return channelState;
}


int main(int argc, char *argv[]) {
  // Set the seed
  srand(time(NULL));
  double qr;
  if(argc > 2){
    qr = atof(argv[2]);
    if(qr < 0 || qr > 1){
      qr = DEFAULT_QR;
    }
  }
  else{
    qr = DEFAULT_QR;
  }

  vector<AlohaNode> nodes(NR_OF_NODES, qr);
  // Store the result of every iteration in this vector
  vector<ItData> data(NR_OF_ITER);
  ItData* currIt;

  for(int i = 0; i < NR_OF_ITER; i++){
    currIt = &data[i];
    currIt->id = i;
    if(i !=0 ){
      currIt->oldBacklogSize = data[i-1].backlogSize;
    }else {
      currIt->oldBacklogSize = 0;
    }

    createArrivals(&nodes, currIt);
    // Decide whether a backlogged node is to transmit or not
    stepBacklog(&nodes);

    currIt->channelState = transmit(&nodes);
    currIt-> backlogSize = getBacklog(&nodes);
    currIt->print();
  }
}
