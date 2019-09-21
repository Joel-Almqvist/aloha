#ifndef ALOHANODE_H
#define ALOHANODE_H


#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

class AlohaNode{
private:
  int state;
  double qr;
public:
  int sleep;
  bool isBacklogged;

  AlohaNode(double qr);
  ~AlohaNode();

  int getState();
  void receivePacket();
  int backlogTick();
  void collided();

};

#endif // ALOHANODE_H
