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
const int NR_OF_ITER = 20;

void tick(double QA, vector<AlohaNode>* vecPtr){
  vector<AlohaNode>& vect = *vecPtr;
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

    if(currNode->getState() != Status::IDLE){
      // The node is either backlogged and or transmitting
      if(lineUsed){
        if(!traversed){
          // Collide all previous sending nodes
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
        // First time a node uses the line this tick
        lineUsed = true;
      }
    }
  }
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
  } else{
    qr = DEFAULT_QR;
  }

  // Q - Why do both of these work?
  //vector<AlohaNode> vect(NR_OF_NODES, AlohaNode(qr));
  vector<AlohaNode> vect(NR_OF_NODES, qr);




  for(int i = 0; i < NR_OF_ITER; i++){
    cout << "Iteration " << i << endl;
    tick(QA, &vect);

    cout << endl;
  }


  //tick(QA, &vect);
}
