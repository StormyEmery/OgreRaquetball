#ifndef __PADDLE_h_
#define __PADDLE_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Simulator.h>
#include <OgreMeshManager.h>
#include "Plain.h"

using namespace Ogre;

class Paddle {
protected:

public:
	btTransform PaddleTransform;
	btCollisionShape* PaddleShape;
	btCollisionObject* PaddleObject;
	SceneNode* PaddleNode;
	Entity* entPaddle;
	btRigidBody* PaddleRB;

	Paddle(SceneManager* mSceneMgr, String name, String node_name);
	~Paddle(void);


	void create();

	void set_origin(btVector3 origin, btQuaternion rotation);
	void set_bounding_box(btScalar radius);
	void create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution);
	void reset(SceneManager* mSceneMgr, Paddle* Paddle, Simulator* simulator);

	btRigidBody* get_rigidbody();


};

#endif