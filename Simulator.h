#ifndef __Simulator_h_
#define __Simulator_h_

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <GameObject.h>
#include <deque>

class Simulator {
protected:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	std::deque<GameObject*> objList;

public:
	Simulator(void);
	~Simulator(void);

	void addObject(GameObject* o);
	void removeObject(GameObject* o, int index);
	//void stepSimulation(const Ogre::Real elapsedTime,
	//					int maxSubSteps = 1, const Ogre::Real fixedTimestep = 1.0f/60.0f);
	btDiscreteDynamicsWorld* getDynamicsWorld(void);
	btAlignedObjectArray<btCollisionShape*> getCollisionShapes(void);
};

#endif