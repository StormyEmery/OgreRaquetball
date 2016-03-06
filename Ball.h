#ifndef __BALL_h_
#define __BALL_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Simulator.h>
#include <OgreMeshManager.h>
#include <OgreParticleSystem.h>
#include <OgreMotionState.h>

using namespace Ogre;

class Ball {
protected:

public:
	btTransform ballTransform;
	btCollisionShape* ballShape;
	btCollisionObject* ballObject;
	SceneNode* ballNode;
	Entity* entBall;
	btRigidBody* ballRB;
	ParticleSystem* jet_particle;
	SceneNode* particleNode;
	OgreMotionState* ballMotionState;
	Simulator* sim;

	Ball(SceneManager* mSceneMgr, String node_name);
	~Ball(void);

	void set_origin(btVector3 origin, btQuaternion rotation);
	void set_bounding_box(btScalar radius);
	void create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution);
	void reset(SceneManager* mSceneMgr, Ball* ball, Simulator* simulator);
	void a(btVector3 dir);
	void updateTransform();

	btRigidBody* get_rigidbody();


};

#endif