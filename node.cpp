#include "node.h";
using namespace std;

const double DEFAULT_QR = 0.01;
const int NR_OF_NODES = 50;

enum Status // unscoped
{
IDLE = 0,
TRANS = 1,
BACKLOG = 2,
};

AlohaNode::AlohaNode(double qr){
  isBacklogged = false;
  //cout << "inside constructor\n";
  state = Status::IDLE;
  qr = qr;
  // constructor
}

AlohaNode::~AlohaNode(){
  // decontrsuctor
  //cout << "inside destructor\n";
}


int AlohaNode::getState(){
  return state;
}


void AlohaNode::receivePacket(){
  if(state == Status::IDLE){
    state = Status::TRANS;
  }
  else if(state == Status::TRANS){
    // Buffer the packet for real
    // but do nothing on this dummy implementation
  }
}


int AlohaNode::backlogTick(){
  sleep--;

  if(sleep == 0){
    state = Status::TRANS;
    return 1;
  }
  return 0;
}


void AlohaNode::collided(){
  state = Status::BACKLOG;
  int mod = round(1/qr);
  sleep = rand() % mod;
}
