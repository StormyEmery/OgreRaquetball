#ifndef __POWERUPSYSTEM_h_
#define __POWERUPSYSTEM_h_

#include <Simulator.h>
#include <OgreMeshManager.h>
#include <ctime>
#include <vector>
#include "Powerup.h"
#include "Ball.h"


using namespace Ogre;

class PowerupSystem{
public:

    clock_t timer;
    double current_time;
    String current_color;
    String old_color;


    float size;

    std::vector<String> colors;

    Powerup* blue;
    Powerup* red;
    Powerup* yellow;

	SceneNode* current_power_node;
    SceneNode* old_power_node;

    void timer_start();
    void choose_powerup();
    void set_random_powerup();
    void get_new_powerup();
    void set_nodes();
    void remove_powerup();
    void check_collision(Ball* b);

	PowerupSystem(SceneManager* mSceneMgr);
	~PowerupSystem();
private:

};

#endif
