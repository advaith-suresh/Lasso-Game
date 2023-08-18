//modified by Advaith 200260004 to include comments that explain the code

#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "gameVariables.h"
#include "coin.h"
#include "lasso.h"

using namespace simplecpp;

main_program{

  initCanvas("Lasso", WINDOW_X, WINDOW_Y);
  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;
  //to set the ending time of events
  float eventTime;

  //level of the game (1 to 5)
  int level = 1;
  //shows if we are ready to change level
  bool level_okay = false;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  int lives = 3;

  //static canvas elements
  Rectangle cliff(PLAY_X_START/2, (PLAY_Y_HEIGHT)/2, PLAY_X_START, PLAY_Y_HEIGHT);
  cliff.setColor(COLOR(255, 187, 0));
  cliff.setFill(true);

  //without putting this in a block the sky doesnt work. probably some bug
  {
  Rectangle sky((WINDOW_X+PLAY_X_START)/2, PLAY_Y_HEIGHT/2, WINDOW_X-PLAY_X_START, PLAY_Y_HEIGHT);
  sky.setColor(COLOR(0, 242, 255));
  sky.setFill(true);
  sky.imprint();
  }

  Rectangle grass(WINDOW_X/2, (WINDOW_Y-PLAY_Y_HEIGHT)/2+PLAY_Y_HEIGHT, WINDOW_X, WINDOW_Y-PLAY_Y_HEIGHT);
  grass.setColor(COLOR("green"));
  grass.setFill(true);

  //Legend to give info about coin
  Circle circle_coin(((WINDOW_X*5.0)/6), WINDOW_Y*7.0/8, 4);
  circle_coin.setColor(COLOR("yellow"));
  circle_coin.setFill(true);
  Text circle_coin_txt(((WINDOW_X*5.0)/6)+20, WINDOW_Y*7.0/8, "coin");

  Circle circle_bomb(((WINDOW_X*5.0)/6), WINDOW_Y*7.0/8+20, 4);
  circle_bomb.setColor(COLOR("red"));
  circle_bomb.setFill(true);
  Text circle_bomb_txt(((WINDOW_X*5.0)/6)+25, WINDOW_Y*7.0/8+20, "bomb");

  Circle circle_magnet(((WINDOW_X*5.0)/6), WINDOW_Y*7.0/8+40, 4);
  circle_magnet.setColor(COLOR("blue"));
  circle_magnet.setFill(true);
  Text circle_magnet_txt(((WINDOW_X*5.0)/6)+32, WINDOW_Y*7.0/8+40, "magnet");


  //text on bottom
  string msg("Lives: _ _ _");
  Text livestxt(PLAY_X_START+50, PLAY_Y_HEIGHT+80, msg);

  string lvl_msg("Level: 1");
  Text lvl(PLAY_X_START+50, PLAY_Y_HEIGHT+20, lvl_msg);

  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  //text for magnet ON/OFF
  Text magnet_txt((WINDOW_X+PLAY_X_START)/2, (WINDOW_Y+PLAY_Y_HEIGHT)/2, "Magnet OFF");


  paused = true; rtheta = true;
  //double coin_speed = randcoinspeed();
  //double coin_angle_deg = randcoinangle();
  double coin_ax = 0;
  double coin_ay = COIN_G;

  //5 coins 5 levels
  int coin_speed = randcoinspeed(1);
  int coin_angle = randcoinangle(1);

  Coin coin1(coin_speed, coin_angle, coin_ax, coin_ay, paused, rtheta, "yellow", 1, 1);
  Coin coin2(coin_speed, coin_angle, coin_ax, coin_ay, paused, rtheta, "yellow", 1, 2);
  Coin coin3(coin_speed, coin_angle, coin_ax, coin_ay, paused, rtheta, "yellow", 1, 3);
  Coin coin4(coin_speed, coin_angle, coin_ax, coin_ay, paused, rtheta, "yellow", 1, 4);
  Coin coin5(coin_speed, coin_angle, coin_ax, coin_ay, paused, rtheta, "yellow", 1, 5);

  //3 bombs (at lvl 1, 3, 5)
  int bomb_speed = randcoinspeed(3);
  int bomb_angle = randcoinangle(3);

  Coin bomb1(bomb_speed, bomb_angle, coin_ax, coin_ay, paused, rtheta, "red", 2, 5);
  Coin bomb2(bomb_speed, bomb_angle, coin_ax, coin_ay, paused, rtheta, "red", 2, 5);
  Coin bomb3(bomb_speed, bomb_angle, coin_ax, coin_ay, paused, rtheta, "red", 2, 5);

  //the magnet will appear at start of every level
  int mag_speed = randcoinspeed(5);
  int mag_angle = randcoinangle(5);
  bool mag_capture = false; //to check if magnet has been captured

  Coin magnet(mag_speed, mag_angle, coin_ax, coin_ay, paused, rtheta, "blue", 3, 5);

  /*
  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end1 = 0;
  double last_coin_jump_end2 = 0;
  double last_coin_jump_end3 = 0;
  double last_coin_jump_end4 = 0;
  double last_coin_jump_end5 = 0;

  // After every COIN_GAP sec, make the bomb jump
  double last_bomb_jump_end1 = 0;
  double last_bomb_jump_end2 = 0;
  double last_bomb_jump_end3 = 0;
*/

  //INSTRUCTION
  // When w is pressed, throw lasso
  // If lasso within range, make coin stick
  // When s is pressed, yank lasso
  // When space is pressed, loop lasso,space again to
  // When q is pressed, quit

  //control/input variable
  char c;

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break;}

    //exit if lives are 0 or if user presses 'q'
    if(lives<=0 || c == 'q') { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      c = charFromEvent(e);

      //to print number of lives
      if(lives == 3){
        msg[msg.length()-1] = char(1);
        msg[msg.length()-3] = char(1);
        msg[msg.length()-5] = char(1);
      }
      else if(lives == 2){
        msg[msg.length()-1] = '_';
        msg[msg.length()-3] = char(1);
        msg[msg.length()-5] = char(1);
      }
      else if(lives == 1){
        msg[msg.length()-1] = '_';
        msg[msg.length()-3] = '_';
        msg[msg.length()-5] = char(1);
      }

      livestxt.setMessage(msg);

      if(level == 1)
        lvl_msg[lvl_msg.length()-1] = '1';
      else if(level == 2)
        lvl_msg[lvl_msg.length()-1] = '2';
      else if(level == 3)
        lvl_msg[lvl_msg.length()-1] = '3';
      else if(level == 4)
        lvl_msg[lvl_msg.length()-1] = '4';
      else if(level == 5)
        lvl_msg[lvl_msg.length()-1] = '5';
      else
        lvl_msg[lvl_msg.length()-1] = 'E';

      lvl.setMessage(lvl_msg);

      switch(c) {
        case 'w':
            lasso.unpause();
            break;

        case 's':
            lasso.yank(currTime);
            break;

        case ' ':
            if(!lasso.isPaused()){
                if(lasso.lasso_looped == false)
                    lasso.loopit();
                else{
                    lasso.loopit();
                    //checking as per level number
                    if(level>=5){
                        lasso.check_for_coin(&coin5, lives, mag_capture, eventTime);
                        lasso.check_for_coin(&bomb3, lives, mag_capture, eventTime);
                    }
                    if(level >= 4){
                        lasso.check_for_coin(&coin4, lives, mag_capture, eventTime);
                    }
                    if(level >= 3){
                        lasso.check_for_coin(&coin3, lives, mag_capture, eventTime);
                        lasso.check_for_coin(&bomb2, lives, mag_capture, eventTime);
                    }
                    if(level >= 2){
                        lasso.check_for_coin(&coin2, lives, mag_capture, eventTime);
                    }
                    if(level >= 1){
                        lasso.check_for_coin(&coin1, lives, mag_capture, eventTime);
                        lasso.check_for_coin(&bomb1, lives, mag_capture, eventTime);
                    }
                    lasso.check_for_coin(&magnet, lives, mag_capture, eventTime, currTime);
                    if(mag_capture)
                        magnet_txt.setMessage("Magnet ON");
                         //to show if magnet effect is on or not


                }
            }
            //wait(STEP_TIME*5);
            break;

        case 'a':
            if(lasso.isPaused())
                lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);
            break;

        case 'd':
            if(lasso.isPaused())
                lasso.addAngle(+RELEASE_ANGLE_STEP_DEG);
            break;

        case '[':
            if(lasso.isPaused())
                lasso.addSpeed(-RELEASE_SPEED_STEP);
            break;

        case ']':
            if(lasso.isPaused())
                lasso.addSpeed(+RELEASE_SPEED_STEP);
            break;

        default:
            break;
      }
    }

    lasso.nextStep(stepTime);

    //to apply magnet effect (only when lasso is unpaused)
    if(mag_capture && !lasso.isPaused()){
        lasso.attract(&coin1);
        lasso.attract(&coin2);
        lasso.attract(&coin3);
        lasso.attract(&coin4);
        lasso.attract(&coin5);
    }

    if(currTime > eventTime){
        mag_capture = false;
        magnet_txt.setMessage("Magnet OFF");
    }

    //coin & bomb actions
    //coin 1, bomb 1
    if(level >= 1){
        bomb1.nextStep(stepTime);
        if(bomb1.isPaused()) {
            if((currTime-bomb1.last_jump_end) >= COIN_GAP) {
                bomb1.show();
                bomb1.unpause();
            }
        }

        if(bomb1.getYPos() > PLAY_Y_HEIGHT) {
            bomb1.resetCoin(currTime);
        }

        coin1.nextStep(stepTime);
        if(coin1.isPaused()) {
            if((currTime-coin1.last_jump_end) >= COIN_GAP1) {
                coin1.show();
                coin1.unpause();
            }
        }

        if(coin1.getYPos() > PLAY_Y_HEIGHT) {
            lasso.decreaseCoins();
            coin1.resetCoin(currTime);
        }
    }

    //coin 2
    if(level >= 2){
        coin2.nextStep(stepTime);
        if(coin2.isPaused()) {
            if((currTime-coin2.last_jump_end) >= COIN_GAP2) {
                coin2.show();
                coin2.unpause();
            }
        }

        if(coin2.getYPos() > PLAY_Y_HEIGHT) {
            lasso.decreaseCoins();
            coin2.resetCoin(currTime);
        }
    }

    //coin 3, bomb 2
    if(level >= 3){
        bomb2.nextStep(stepTime);
        if(bomb2.isPaused()) {
            if((currTime-bomb2.last_jump_end) >= COIN_GAP) {
                bomb2.show();
                bomb2.unpause();
            }
        }

        if(bomb2.getYPos() > PLAY_Y_HEIGHT) {
            bomb2.resetCoin(currTime);
        }

        coin3.nextStep(stepTime);
        if(coin3.isPaused()) {
            if((currTime-coin3.last_jump_end) >= COIN_GAP3) {
                coin3.show();
                coin3.unpause();
            }
        }

        if(coin3.getYPos() > PLAY_Y_HEIGHT) {
            lasso.decreaseCoins();
            coin3.resetCoin(currTime);
        }
    }

    //coin 4
    if(level >= 4){
        coin4.nextStep(stepTime);
        if(coin4.isPaused()) {
            if((currTime-coin4.last_jump_end) >= COIN_GAP4) {
                coin4.show();
                coin4.unpause();
            }
        }

        if(coin4.getYPos() > PLAY_Y_HEIGHT) {
            lasso.decreaseCoins();
            coin4.resetCoin(currTime);
        }
    }

    //coin 5
    if(level >= 5){
        bomb3.nextStep(stepTime);
        if(bomb3.isPaused()) {
            if((currTime-bomb3.last_jump_end) >= COIN_GAP) {
                bomb3.show();
                bomb3.unpause();
            }
        }

        if(bomb3.getYPos() > PLAY_Y_HEIGHT) {
            bomb3.resetCoin(currTime);
        }

        coin5.nextStep(stepTime);
        if(coin5.isPaused()) {
            if((currTime-coin5.last_jump_end) >= COIN_GAP5) {
                coin5.show();
                coin5.unpause();
            }
        }

        if(coin5.getYPos() > PLAY_Y_HEIGHT) {
            lasso.decreaseCoins();
            coin5.resetCoin(currTime);
        }
    }

    magnet.nextStep(stepTime);
    if(magnet.getYPos() > PLAY_Y_HEIGHT)
            magnet.resetCoin(currTime);

    //changing level for every 5 coins collected
    if(lasso.getNumCoins()%5 == 0 && level_okay){
        switch(lasso.getNumCoins()){
            case 5:
                level = 2;
                break;
            case 10:
                level = 3;
                break;
            case 15:
                level = 4;
                break;
            case 20:
                level = 5;
                break;
        }
        //magnet will appear when level increases
        magnet.show();
        magnet.unpause();

        level_okay = false; //will note change level when coins remain at multiple of 5
    }
    //once it moves past multiple of 5 we should be ready to change to next level when we reach next multiple of 5
    else if(lasso.getNumCoins()%5 != 0)
        level_okay = true;

    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    //cout << currTime << '\n';
    //wait(0.1*stepTime);
  } // End for(;;)

  Text t1(300, 300, "GAME OVER");
  Text t2(300, 330, "go to command line to save game");

  cout << "GAME OVER\n";


  //File management to store name and score in a file
  char ch, name[10];
  cout << "do you wish to save your score? (y/n): ";
  cin >> ch;
  if(ch == 'y'){
    cout << "name (no spaces): ";
    cin >> name;
    ofstream fout;
    fout.open("scores.txt", ios::app);
    fout << name << ' ' << "score: " << lasso.getNumCoins() << '\n';
    fout.close();
    cout << "score saved to file scores.txt";
  }

} // End main_program
