#ifndef __POWERUPSYSTEM_h_
#define __POWERUPSYSTEM_h_

#include <Simulator.h>
#include <OgreMeshManager.h>
#include <time.h>
#include <ctime>
#include "Powerup.h"
#include "Ball.h"

using namespace Ogre;

class PowerupSystem{
public:

    clock_t timer;
    double time;
    String current;

    float size;


    Powerup* blue;
    Powerup* red;
    Powerup* yellow;

    void timer_start();
    void choose_powerup();
    void check_collision(Ball* b);

	PowerupSystem(SceneManager* mSceneMgr);
	~PowerupSystem();
private:

};

#endif
