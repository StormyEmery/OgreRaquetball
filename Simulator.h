#ifndef __Simulator_h_
#define __Simulator_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <OgreMotionState.h>


class Simulator {
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

public:
	Simulator(void);
	~Simulator(void);

	btDiscreteDynamicsWorld* getDynamicsWorld(void);
	btAlignedObjectArray<btCollisionShape*>* getCollisionShapes(void);
	btCollisionDispatcher* getDispatcher();
};

#endif