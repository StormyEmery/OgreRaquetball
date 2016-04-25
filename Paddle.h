#ifndef __PADDLE_h_
#define __PADDLE_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Simulator.h>
#include <OgreMeshManager.h>

using namespace Ogre;

class Paddle {
public:
	btTransform paddleTransform;
	Entity* paddleEntity;
	SceneNode* paddleNode;
	SceneNode* translationNode;
	btCollisionShape* paddleShape;
	btCollisionObject* paddleObject;
	OgreMotionState* paddleMotionState;
	btRigidBody* paddleRigidBody;
	Simulator* s;
	float max_speed;

	Paddle(SceneManager* mSceneMgr, String name, String node, Vector3 scale, String tname);

	~Paddle(void);

	void set_origin(btVector3 origin);
	void set_bounding_box();
	void create_bounding_box(Simulator* simulator, btScalar mass, btScalar restitution);
	void updateTransform();
	btVector3 heading();

protected:

};

#endif