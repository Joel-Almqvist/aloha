#ifndef ALOHANODE_H
#define ALOHANODE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;


enum Status // unscoped
{
IDLE = 0,
TRANS = 1,
BACKLOG = 2,
};


enum Response
{
  UNUSED = 3,
  COL = 4,
  SUCCESS = 5,
};

string strRes(int res);


class AlohaNode{
private:
  int state;
  double qr;
  double qa;
  double backlogEst;
public:

  AlohaNode(double qr, double qa);
  ~AlohaNode();

  int getState();
  bool receivePacket(bool pseudoBayFlag);
  void backlogTick();
  void collide();
  bool isSending();
  void calcQr();
  void estimateBacklog(int feedback);
  double getBacklogEst();
};

#endif // ALOHANODE_H
