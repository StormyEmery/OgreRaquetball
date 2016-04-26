#ifndef __POWERUP_H_
#define __POWERUP_H_

#include <Simulator.h>
#include <OgreMeshManager.h>

using namespace Ogre;

class Powerup{
private:

public:

    String color;

	Entity* entity;
	SceneNode* node;

    Powerup(SceneManager* mSceneMgr, String C, String entity_name, String node_name);
    ~Powerup();
};

#endif
