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
  }
    return false;
}


void AlohaNode::backlogTick(){
  if(state == Status::BACKLOG){
    int mod = round(1/qr);
    if(rand() % mod == 1){
      state = Status::TRANS;
    }
  }
}


void AlohaNode::collide(){
  state = Status::BACKLOG;
}
