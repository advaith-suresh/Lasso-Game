#ifndef __LASSO_H__
#define __LASSO_H__

#include "movingObject.h"
#include "coin.h"

//class Lasso inherits publically from base class MovingObject
class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;

  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  Coin* the_coin[5];
  int numcoin; //number of coins currently in the lasso
  int num_coins; //number of coins collected totally

  void initLasso();
 public:
 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  bool lasso_looped;

  void draw_lasso_band();
  void yank(float);
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin, int &, bool &, float &, float);
  int getNumCoins() { return num_coins; }
  void decreaseCoins() {num_coins--;}
  void attract(Coin *coin);
}; // End class Lasso


//draws the lasso band using a straight line
void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR(165,42,42));
  lasso_circle.setFill(false);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(165,42,42));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;

  for(int i=0; i<5; i++)
    the_coin[i] = NULL;
  num_coins = 0;
  numcoin = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR(139, 69, 19));

  lasso_band.setColor(COLOR(218,165,32));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank(float currtime) {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;

  if(numcoin != 0) {
    for(int i=0; i<numcoin; i++){
        the_coin[i]->attatched = false;
        the_coin[i]->resetCoin(currtime);
        the_coin[i] = NULL;
    }
  }

  numcoin = 0;
} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) {
    lasso_loop.reset(getXPos(), getYPos(), LASSO_SIZE/2);
    lasso_looped = false;
    return;
  } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(0); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

//passing lives so that it can be reduced if object is of type bomb
//passing mag to set bool in main as true i.e. magnet has been captures
//eventned and time passed to set the time in main when the effect of magnet must wear off
void Lasso::check_for_coin(Coin *coinPtr, int &lives, bool &mag, float &eventend, float time=0) {

    double lasso_x = getXPos();
    double lasso_y = getYPos();
    double coin_x = coinPtr->getXPos();
    double coin_y = coinPtr->getYPos();
    double xdiff = (lasso_x - coin_x);
    double ydiff = (lasso_y - coin_y);
    double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
    if(distance <= LASSO_RADIUS) {
        if(coinPtr->type == 1 && coinPtr->attatched == false){
            num_coins++;
            the_coin[numcoin] = coinPtr;
            the_coin[numcoin]->getAttachedTo(this);
            coinPtr->attatched = true;
            numcoin++;
        }
        else if(coinPtr->type == 2){
            lives--;
            coinPtr->resetCoin(0);
        }
        else if(coinPtr->type == 3){
            mag = true;
            eventend = time + 1;
            coinPtr->resetCoin(0);
        }
    }
} // End Lasso::check_for_coin()

//to attract the non-attatched coins to the lasso if magnet effect is on
void Lasso::attract(Coin *coin){

    if(coin->attatched != true){
        double x = this->getXPos() - coin->getXPos();
        double y = this->getYPos() - coin->getYPos();

        coin->set_vx(x*2);
        coin->set_vy(y*2);
    }
}

#endif
