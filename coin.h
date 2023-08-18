//coin.h
#ifndef __COIN_H__
#define __COIN_H__

#include "movingObject.h"
#include <time.h>
#include <cstdlib>

//Class Coin inherits publically from class MovingObject
class Coin : public MovingObject {
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;
  int seed; //seed for unique random
  char color[10]; //to set color of sprite

  // Moving parts
  Circle coin_circle;

 public:
  int type; //1-coin, 2-bomb, 3-magnet
  bool attatched; //to see if coin is attatched to lasso
  int last_jump_end; //to track when the coin last fell/yanked

 //constructor call (includes constructor of base class MovingObject
 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta, const char* pcolor, int stype, int seed_val) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    seed = seed_val;
    type = stype;
    last_jump_end = 0;

    int i=0;
    while(pcolor[i] != '\0'){
        color[i] = pcolor[i];
        i++;
    }
    color[i] = '\0';

    initCoin();
  }

  void initCoin();
  void resetCoin(float);
  void show(); //to unhide the circle shape
}; // End class Coin

void Coin::show(){
    coin_circle.show();
};

//initialize coin at the bottom of 'floor'
void Coin::initCoin() {
    coin_start_x = (PLAY_X_START+WINDOW_X)/2;
    coin_start_y = PLAY_Y_HEIGHT;
    coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
    coin_circle.setColor(COLOR(color));
    coin_circle.setFill(true);
    coin_circle.hide();//show on unpausing
    attatched = false;
    addPart(&coin_circle);
}

//seed is to get different values even if functions are ran at the same time
int randcoinspeed(int seed){
    std::srand(time(0));
    return ((seed*rand())%200) + 150;
}

int randcoinangle(int seed){
    std::srand(time(0));
    return ((seed*rand())%60) + 80;
}

int randcoinXpos(int seed){
    std::srand(time(0));
    return ((seed*rand())%200);
}

void Coin::resetCoin(float currtime) {
  double coin_speed = randcoinspeed(seed);
  double coin_angle_deg = randcoinangle(seed);
  coin_start_x = ((PLAY_X_START + WINDOW_X)/2)- 100 + randcoinXpos(seed);
  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  coin_circle.hide();
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  last_jump_end = currtime;
}
#endif
