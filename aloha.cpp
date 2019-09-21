#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "node.h";
using namespace std;

const double DEFAULT_QR = 0.01;
const int NR_OF_NODES = 50;

int main(int argc, char *argv[]) {
  srand(time(NULL));
  double qr;
  if(argc > 1){
    //cout << argv[1] << endl;
    qr = atof(argv[1]);
    //cout << qr << endl;
  } else{
    qr = DEFAULT_QR;
  }

  //AlohaNode* node = new AlohaNode(0.01);

  // Works as malloc
  //AlohaNode* node2 = new AlohaNode();

  // Q - Why do both of these work?
  //vector<AlohaNode> vect(NR_OF_NODES, AlohaNode(qr));

  vector<AlohaNode> vect(NR_OF_NODES, qr);


  cout << vect[0].sleep << endl;

  vect[0].sleep = 13;

  cout << vect[1].sleep << endl;
  cout << vect[0].sleep << endl;

  //AlohaNode* nodes = new AlohaNode[NR_OF_NODES];
  //for(int i = 0; i < NR_OF_NODES; i++){
  //  nodes[i] = AlohaNode(qr);
  //}

  //cout << nodes[0] << endl;
  //cout << nodes[0].getState() << endl;

  AlohaNode node = AlohaNode(0.01);

}
