#include "PowerupSystem.h"

PowerupSystem::PowerupSystem(SceneManager* mSceneMgr){
    Powerup* blue = new Powerup(mSceneMgr, "blue", "ep1", "np1");
    Powerup* red = new Powerup(mSceneMgr, "red", "ep2", "np2");
    Powerup* yellow = new Powerup(mSceneMgr, "yellow", "ep3", "np3");

    blue->node->translate(-5000, 0, 0);
    red->node->translate(-5000, 0, 0);
    yellow->node->translate(-5000, 0, 0);
};


PowerupSystem::~PowerupSystem(){};

