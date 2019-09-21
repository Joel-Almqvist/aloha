#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "node.h"
using namespace std;

const double DEFAULT_QR = 0.2;
// The chance for a single node to receive a packet this slot
const double QA = 0.1;
const int NR_OF_NODES = 50;

void tick(double QA, vector<AlohaNode>* vect2){
  vector<AlohaNode> vect = *vect2;
  int pSpace = round(1/QA);
  bool lineUsed = false;
  bool traversed = false;
  AlohaNode* currNode;

  for(int i = 0; i < vect.size(); i++){
    currNode = &vect[i];
    int outcome = rand();

    if(outcome % pSpace == 1){
      currNode->receivePacket();
    }
    currNode->backlogTick();

    //cout << currNode->getState() << endl;
    if(currNode->getState() != Status::IDLE){
      //cout << "not idle" << endl;
      if(lineUsed){
        // Collide all previous sending nodes
        if(!traversed){
          for(int j = 0; j< i+1; j++){
            if(vect[j].getState() != Status::IDLE){
              vect[j].collided();
              // There can only be one previous occurence
              break;
            }
          }
          traversed = true;
        }
        else{
          currNode->collided();
        }
      }
      else{
        lineUsed = true;
      }
    }
  }


  cout << QA << endl;
}


int main(int argc, char *argv[]) {
  srand(time(NULL));
  double qr;
  if(argc > 2){
    qr = atof(argv[2]);
    if(qr < 0 || qr > 1){
      qr = DEFAULT_QR;
    }
  } else{
    qr = DEFAULT_QR;
  }

  // Q - Why do both of these work?
  //vector<AlohaNode> vect(NR_OF_NODES, AlohaNode(qr));
  vector<AlohaNode> vect(NR_OF_NODES, qr);





  /*
  for(int i = 0; i < vect.size(); i++){
    currNode = &vect[i];
    if(currNode->getState)

  }
  */

  tick(QA, &vect);
}
