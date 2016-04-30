
#ifndef __POWERUP_H_
#define __POWERUP_H_


#include <Simulator.h>
#include <OgreMeshManager.h>
#include <ctime>

using namespace Ogre;

class Powerup{

public:


	Entity* entity;
	SceneNode* node;
	String color;

    String get_color();
    bool active;

    clock_t timer;
    double current_time;

    void set_position(float x, float y, float z);
    Vector3 get_position();
    void activate(Light* light);
    void set_active();
    void set_inactive();
    bool is_active();
	Powerup(SceneManager* mSceneMgr, String c, String entity_name, String node_name);
	~Powerup();
private:

};

#endif
