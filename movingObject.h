//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

//MovingObject inherits from public (base class) Sprite
class MovingObject : public Sprite {
  vector<Sprite*> parts; //list of sprites that move according to parameters and method calls of given MovingObject object
  double vx, vy;
  double ax, ay;
  bool paused;
  //initialising the parameters of MovingObject
  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }
 public:
 //constructor to define MovingObject using vel and acc coordinates in cartesian
 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
  //overloaded constructor to define sprite using acc cartesian coordinates and vel polar coordinates (bool rtheta is probably to get unique signature)
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  //methods to set individual parameters
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  //accessor methods
  double getXPos();
  double getYPos();
  //
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  //to return/modify state of MovingObject
  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  //to add more sprite 'p' in the parts vector?
  void addPart(Sprite* p) {
    parts.push_back(p);
  }
  //
  void nextStep(double t);
  void getAttachedTo(MovingObject *m);
};

//MovingObject.cpp

//changes velocity of MovingObject after time t has elapsed (where is function move defined? prolly in Sprite)
void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  if(getXPos()<100)
        vx = - vx;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

//returns X coordinate of zeroeth sprite if the size of sprite vector is non zero (where is getX defined? prolly in sprite)
double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

//returns X coordinate of zeroeth sprite if the size of sprite vector is non zero (where is getX defined? prolly in sprite)
double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

//resets parameters of Moving object and assosciated sprites (where is moveTo defined?:prolly in sprite)
void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  //resets pos of sprites assosciated to MovingObject Object
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }
  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;
} // End MovingObject::reset_all()

//changes parameters of assosciated sprites and this MovingObject object to those of object 'm'
void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

#endif
