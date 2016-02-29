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

void Simulator::addObject(GameObject* o) {
	objList.push_back(o);
	dynamicsWorld->addRigidBody(o->getBody());
}

void Simulator::removeObject(GameObject* o, int index) {
	objList.erase(objList.begin() + index);
	dynamicsWorld->removeRigidBody(o->getBody());
	delete &o;
	o = NULL;
}
