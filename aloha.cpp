/* Written by Joel Almqvist (joeal360) for TSIN01
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "node.h"
#include "iteration.h"
#include "matplotlib-cpp/matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

const int NR_OF_NODES = 100;
const int NR_OF_ITER = 1000;
const double qr = 0.1;
//const double lambda = 1/2.7182818284590452353602874; // 1 / e
const double lambda = 0.5;
double QA = 1.0 - exp(-lambda/NR_OF_NODES); // QA = 1 - e^(-lambda/m)
//const double QA = 0.5;
const bool GENERATE_IMGS = true;
const bool PSEUDO_BAY = true;

// Creates an arrival for each node and store the nodes behavior to this event
// in the data class ItData.
void createArrivals(vector<AlohaNode>* vecPtr, ItData* dataPtr){
  int posSpace = round(1/QA);
  int arrivals = 0;
  int receivedPackets = 0;
  int outcome;
  AlohaNode* currNode;
  for(size_t i = 0; i < vecPtr->size();i++){
    currNode = &(*vecPtr)[i];
    outcome = rand();
    if(outcome % posSpace == 0){
      receivedPackets += currNode->receivePacket(PSEUDO_BAY, dataPtr->id);
      arrivals++;
    }
  }
  dataPtr->arrivals = arrivals;
  dataPtr->acceptedArrivals = receivedPackets;
}

// Check whether a backlog node wants to start transmitting this iteration
void stepBacklog(vector<AlohaNode>* vecPtr){
  for(size_t i = 0; i < vecPtr->size();i++){
    (*vecPtr)[i].backlogTick();
  }
}

// Get the backlog's size
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


void reestimateBacklog(vector<AlohaNode>* nodes, int prevSlotFeedback){
  for(size_t i = 0; i < nodes->size(); i++){
    (*nodes)[i].estimateBacklog(prevSlotFeedback);
  }
}


/* Make every node with state "transmitting" transmit and backlog them
 * if a collision occurs. Determine the channel's state afterwards.
 *
 * Store data from the algorithm above in the provided ItData object
*/
void transmit(vector<AlohaNode>* vecPtr, ItData* data){
  int channelState = Response::UNUSED;

  bool alreadySending = false;
  bool sorted = false;
  bool transmitting;
  AlohaNode* currNode;

  // If a transmission is succesful store the succesful node here
  AlohaNode* posTransNode;

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
      posTransNode = &(*currNode);
      channelState = Response::SUCCESS;
      alreadySending = true;
    }
  }
  data->channelState = channelState;

  if(channelState == Response::SUCCESS){
    posTransNode->successTrans();
    data->slotsWaited = data->id - posTransNode->arrivalTime;
  }
}

// Append meta data to the string base. Used to give plots a descriptive name.
string generatePlotName(string base){
  string QR = to_string(qr);
  string qa = to_string(QA);
  string name = base.substr(0, base.length() -1);
  name += "_qr"+QR.substr(0,1)+","+QR.substr(2,2);
  name += "_qa"+qa.substr(0,1)+","+qa.substr(2,4);
  name += "_slots"+to_string(NR_OF_ITER);
  name += "_nodes"+to_string(NR_OF_NODES);
  if(PSEUDO_BAY){
    name += "_PB";
  }
  return name;
}

// Plots system arrivals and system departures for every slot
void plotArrivals(vector<ItData>* iterations){
  vector<int> entering(iterations->size());
  vector<int> leaving(iterations->size());
  vector<int> slots(iterations->size());


  // Ugly hack to handle the first iteration manually
  slots[0] = 0;
  entering[0] = (*iterations)[0].acceptedArrivals;
  (*iterations)[0].channelState == Response::SUCCESS ? leaving[0] = 1 : leaving[0] = 0;

  for(int i = 1; i < iterations->size(); i++){
    entering[i] = (*iterations)[i].acceptedArrivals + entering[i-1];
    slots[i] = i;

    if((*iterations)[i].channelState == Response::SUCCESS){
      leaving[i] = 1 + leaving[i-1];
    }
    else{
      leaving[i] = leaving[i-1];
    }
  }


  plt::named_plot("Packets arriving",slots, entering);
  plt::named_plot("Packets departing",slots, leaving);
  //plt::ylim(0,5);
  plt::legend();
  plt::title("System arrivals and departures");
  if(GENERATE_IMGS){
    plt::save("./plots/"+generatePlotName("system_arrivals"));
  }
  plt::clf();
}

// Plots the value of the backlog for every slot
void plotBacklog(vector<ItData>* iterations){
  vector<double> backlogs(iterations->size());
  vector<double> slots(iterations->size());
  vector<double> backlogEstimates(iterations->size());

  for(int i = 0; i < iterations->size(); i++){
    backlogs[i] = (*iterations)[i].backlogSize;
    backlogEstimates[i] = (*iterations)[i].backlogEst;
    slots[i] = i;
  }


  plt::named_plot("Backlog",slots, backlogs);
  //plt::plot(slots, backlogs);
  if(PSEUDO_BAY){
    plt::named_plot("Est. backlog", slots, backlogEstimates);
    //plt::plot( slots, backlogEstimates);
  }
  plt::legend();

  plt::title("Backlog size per slot");
  if(GENERATE_IMGS){
      plt::save("./plots/"+generatePlotName("backlog"));
  }
  plt::clf();

  // Create extra graph with no GUI hiding part of the graph
  if(GENERATE_IMGS && PSEUDO_BAY){
    plt::plot(slots, backlogEstimates);
    plt::save("./plots/"+generatePlotName("backlog-extra"));
    plt::clf();
  }

}


void plotSteadyState(vector<ItData>* iterations){
  vector<double> steadyStateProb(NR_OF_NODES);
  double avgBacklog = 0;
  double succ = 0;
  // We draw from [0, backlogMaxSize], it may not be 0
  int backlogMaxSize = 1;
  int currBacklogSize;

  for(int i = 0; i < iterations->size(); i++){
    currBacklogSize = (*iterations)[i].backlogSize;
    steadyStateProb[currBacklogSize] += 1 / (double) iterations->size();
    avgBacklog += currBacklogSize / (double) iterations->size();

    if((*iterations)[i].channelState == Response::SUCCESS){
      succ += 1.0 / NR_OF_ITER;
    }

    if(currBacklogSize > backlogMaxSize){
      backlogMaxSize = currBacklogSize;
    }
  }

  cout << "Avg succ rate: " << succ << endl;
  cout << "Avg backlog: " << avgBacklog << endl;

  plt::bar(steadyStateProb);
  plt::title("Steady state probability for backlog size");
  plt::xlim(0, backlogMaxSize);
  if(GENERATE_IMGS){
      plt::save("./plots/"+generatePlotName("steady_state"));
  }
  plt::clf();

}

/* Plots the average packet delay for the Pseudo Bayesian stabilization
* algorithm.
*/
void plotAvgPacketDelayPB(){
  double e = 2.7182818284590452353602874;
  // May not be 0
  double startVal = 0.05;
  // endVal is not actually included, the range is [startval, endVal)
  double endVal = 1.0/e;
  int intervals = 100;
  double stepSize = (endVal-startVal) / intervals;
  double lambda2;
  vector<double> arrRates(intervals);
  vector<double> avgDelay(intervals);

  for(int i = 0; i < intervals; i++){
    lambda2 = startVal + i*stepSize;
    arrRates[i] = lambda2;
    avgDelay[i] = (e-0.5) / (1.0-lambda2*e) -
      (e-1)*(exp(lambda2)-1)/(lambda2*(1-(e-1)*(exp(lambda2)-1)));
  }

  // NOTE: These values are hardcoded and taken from the cout of
  // plotSystemPacketWait() when running the whole application with differing
  // lambda values,
  vector<double> simWaitTime{0.465, 1.33071, 6, 17.5576, 45.7674, 63.46, 68};
  vector<double> arrivalRate{0.05, 0.15, 0.25, 0.35, 0.4, 0.45, 0.5};
  plt::plot(arrRates, avgDelay);
  plt::plot(arrivalRate, simWaitTime, "bo");
  plt::title("Average delay in slots as a function of arrival rates");
  if(GENERATE_IMGS){
    plt::save("./plots/avg-packet-delays");
  }
  plt::clf();
}

// Calculate the average delay of every succesfull transmission. The output is
// simply printed (and later hardcoded).
void calcAvgDelay(vector<ItData>* dataVect){
  double slotsCtr = 0;
  double slotsWtd = 0;

  ItData* data;
  for(int i = 0; i < dataVect->size(); i++){
    data = &(*dataVect)[i];
    if(data->slotsWaited != -1){
      slotsCtr += 1;
      slotsWtd += data->slotsWaited;
    }
  }
  cout << "Avg wait for lambda = " << lambda<< " is "<< slotsWtd/slotsCtr << endl;

}


int main(int argc, char *argv[]) {
  // Set the random seed to the current time
  srand(time(NULL));

  vector<AlohaNode> nodes(NR_OF_NODES, AlohaNode(qr, QA));
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

    // We have no feedback for the first slot and hence use a default value
    if(PSEUDO_BAY && i != 0){
      reestimateBacklog(&nodes, data[i-1].channelState);
      currIt->backlogEst = nodes[0].getBacklogEst();
    }

    // Decide whether a backlogged node is to transmit or not
    stepBacklog(&nodes);
    transmit(&nodes, currIt);
    currIt-> backlogSize = getBacklog(&nodes);
    //currIt->print();
  }
  plotBacklog(&data);
  plotArrivals(&data);
  plotSteadyState(&data);
  if(PSEUDO_BAY) plotAvgPacketDelayPB();
  calcAvgDelay(&data);

}
