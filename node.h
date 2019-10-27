/* Written by Joel Almqvist (joeal360) for TSIN01
*/
#ifndef ALOHANODE_H
#define ALOHANODE_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

// The 3 states of an AlohaNode
enum Status // unscoped
{
IDLE = 0,
TRANS = 1,
BACKLOG = 2,
};

// The 3 states of the channel
enum Response
{
  UNUSED = 3,
  COL = 4,
  SUCCESS = 5,
};

string strRes(int res);

// Every node in the Aloha protocol is represented by its own AlohaNode object.
class AlohaNode{
private:
  int state;
  double qr;
  double qa;
  double backlogEst;
public:
  int arrivalTime;

  AlohaNode(double qr, double qa);
  ~AlohaNode();

  int getState();
  bool receivePacket(bool pseudoBayFlag, int slot);
  void backlogTick();
  void collide();
  bool isSending();
  void calcQr();
  void estimateBacklog(int feedback);
  double getBacklogEst();
};

#endif // ALOHANODE_H
