#ifndef __BALL_h_
#define __BALL_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Simulator.h>
#include <OgreMeshManager.h>

using namespace Ogre;

class Ball {
protected:

public:
	btTransform ballTransform;
	btCollisionShape* ballShape;
	SceneNode* ballNode;
	Entity* entBall;
	btRigidBody* ballRB;

	Ball(SceneManager* mSceneMgr, String name, String node_name);
	~Ball(void);

	void set_origin(btVector3 origin, btQuaternion rotation);
	void set_bounding_box(btScalar radius);
	void create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution);
	void reset(SceneManager* mSceneMgr, Ball* ball, Simulator* simulator);

	btRigidBody* get_rigidbody();


};

#endif