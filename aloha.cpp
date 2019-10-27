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

const double DEFAULT_QR = 0.01;
//const double DEFAULT_QR = 0.1;
// The chance for a single node to receive a packet this slot 1/e
const double QA = 1/2.7182818284590452353602874;
//const double QA = 0.5;
const int NR_OF_NODES = 100;
const int NR_OF_ITER = 1000;
const bool GENERATE_IMGS = true;
const bool PSEUDO_BAY = true;

void createArrivals(vector<AlohaNode>* vecPtr, ItData* dataPtr){
  int posSpace = round(1/QA);
  int arrivals = 0;
  int receivedPackets = 0;
  int outcome;
  for(size_t i = 0; i < vecPtr->size();i++){
    outcome = rand();
    if(outcome % posSpace == 1){
      receivedPackets += (*vecPtr)[i].receivePacket(PSEUDO_BAY);
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


void reestimateBacklog(vector<AlohaNode>* nodes, int prevSlotFeedback){
  for(size_t i = 0; i < nodes->size(); i++){
    (*nodes)[i].estimateBacklog(prevSlotFeedback);
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

// Append meta data to the string base. Used to give plots a descriptive name.
string generatePlotName(string base){
  string qr = to_string(DEFAULT_QR);
  string qa = to_string(QA);
  string name = base.substr(0, base.length() -1);
  name += "_qr"+qr.substr(0,1)+","+qr.substr(2,2);
  name += "_qa"+qa.substr(0,1)+","+qa.substr(2,2);
  name += "_slots"+to_string(NR_OF_ITER);
  name += "_nodes"+to_string(NR_OF_NODES);
  if(PSEUDO_BAY){
    name += "_PB";
  }
  return name;
}



void plotPackets(vector<ItData>* iterations){
  vector<int> entering(iterations->size());
  vector<int> leaving(iterations->size());
  vector<int> slots(iterations->size());

  for(int i = 0; i < iterations->size(); i++){
    entering[i] = (*iterations)[i].acceptedArrivals;
    slots[i] = i;

    if((*iterations)[i].channelState == Response::SUCCESS){
      leaving[i] = 1;
    }
    else{
      leaving[i] = 0;
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


  for(int i = 0; i < iterations->size(); i++){
    steadyStateProb[(*iterations)[i].backlogSize] += 1 / (double )iterations->size();
    avgBacklog += (*iterations)[i].backlogSize /(double )iterations->size();

    if((*iterations)[i].channelState == Response::SUCCESS){
      succ += 1.0 / NR_OF_ITER;
    }

  }
  cout << "Avg succ rate: " << succ << endl;
  cout << "Avg backlog: " << avgBacklog << endl;

  plt::bar(steadyStateProb);
  plt::title("Steady state probability for backlog size");
  plt::xlim(0, NR_OF_NODES);
  if(GENERATE_IMGS){
      plt::save("./plots/"+generatePlotName("steady_state"));
  }
  plt::clf();
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

    currIt->channelState = transmit(&nodes);
    currIt-> backlogSize = getBacklog(&nodes);
    //currIt->print();
  }
  plotBacklog(&data);
  plotPackets(&data);
  plotSteadyState(&data);
}
