#ifndef __POWERUP_h_
#define __POWERUP_h_

#include <Simulator.h>
#include <OgreMeshManager.h>

using namespace Ogre;

class Powerup{
public:


	Entity* entity;
	SceneNode* node;
	String color;

	Powerup(SceneManager* mSceneMgr, String c, String entity_name, String node_name);
	~Powerup();
private:

};

#endif