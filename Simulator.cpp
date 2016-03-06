#include <Simulator.h>

Simulator::Simulator() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
												overlappingPairCache,
												solver,
												collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
}

btDiscreteDynamicsWorld* Simulator::getDynamicsWorld() {
	return dynamicsWorld;
}

btAlignedObjectArray<btCollisionShape*>* Simulator::getCollisionShapes() {
	return &collisionShapes;
}

btCollisionDispatcher* Simulator::getDispatcher() {
	return dispatcher;
}

