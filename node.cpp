#include "node.h"
using namespace std;

const double DEFAULT_QR = 0.01;
const int NR_OF_NODES = 50;

string strRes(int res){
  switch(res){
    case Response::UNUSED:
      return "Transmission not attempted";
    case Response::COL:
      return "Transmission failed";
    case Response::SUCCESS:
      return "Transmission succesful";
    default:
    return "Unkown response provided";
  }
};

AlohaNode::AlohaNode(double qr, double qa){
  state = Status::IDLE;
  AlohaNode::qr = qr;
  AlohaNode::qa = qa;
  AlohaNode::backlogEst = 0;
}

AlohaNode::~AlohaNode(){
  // decontrsuctor
  //cout << "inside destructor\n";
}


int AlohaNode::getState(){
  return state;
}

// Only used in the Pseudo bayesian stabilization
void AlohaNode::calcQr(){
  if(backlogEst == 0){
    qr = 1;
  }
  else{
    qr =  min(1.0, 1.0 / backlogEst);
  }
}

void AlohaNode::estimateBacklog(int feedback){
  if(feedback == Response::UNUSED || feedback == Response::SUCCESS){
    backlogEst = max(qa, backlogEst + qa -1);
  } else if(feedback == Response::COL){
    backlogEst = backlogEst + qa + 1 / (0.71828182845);
  } else {
    cout << "Bad input to estimateBacklog\n";
  }
  calcQr();
}


bool AlohaNode::isSending(){
  if(state == Status::TRANS){
    return true;
  }
  else{
    return false;
  }
}


// Return whether the reception was possible
bool AlohaNode::receivePacket(bool pseudoBayFlag){
  if(state == Status::IDLE){
    state = pseudoBayFlag ? Status::BACKLOG : Status::TRANS;
    return true;
  }
    return false;
}


void AlohaNode::backlogTick(){
  if(state == Status::BACKLOG){
    int mod = round(1/qr);
    if(rand() % mod == 0){
      state = Status::TRANS;
    }
  }
}


void AlohaNode::collide(){
  state = Status::BACKLOG;
}

double AlohaNode::getBacklogEst(){
  return backlogEst;
}
