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

AlohaNode::AlohaNode(double qr){
  state = Status::IDLE;
  AlohaNode::qr = qr;
}

AlohaNode::~AlohaNode(){
  // decontrsuctor
  //cout << "inside destructor\n";
}


int AlohaNode::getState(){
  return state;
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
bool AlohaNode::receivePacket(){
  if(state == Status::IDLE){
    state = Status::TRANS;
    return true;
    //return Arrival::SUCCESS;
  }
    return false;
}


int AlohaNode::backlogTick(){
  sleep--;

  if(sleep == 0){
    state = Status::TRANS;
    return 1;
  }
  return 0;
}


void AlohaNode::collide(){
  state = Status::BACKLOG;
  // TODO reconsider this randomness, it is not really accuarate,
  // every node sleep duration is uniformly chosen from [0, 1/qr]
  // instead of having a qr chance of sending every tick
  int mod = round(1/qr);
  sleep = rand() % mod;
}
