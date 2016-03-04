#ifndef __PLAIN_h_
#define __PLAIN_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <Simulator.h>
#include <OgreMeshManager.h>

using namespace Ogre;

class Plain {
protected:
	
public:
	btTransform groundTransform;
	btCollisionShape* groundShape;
	btCollisionObject* groundObject;
	SceneNode* groundNode;
	Entity* entGround;
	btRigidBody* groundBody;

	Plain(SceneManager* mSceneMgr, Vector3 normal, Vector3 up_vector, float x, float y, float offset, String node_name, String name, String path);
	~Plain(void);

	void set_origin(btVector3 origin);
	void set_bounding_box();
	void create_bounding_box(Simulator* simulator);
	// btRigidBody* get_rigid_body();
};

#endif