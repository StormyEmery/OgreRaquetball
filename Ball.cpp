#include "Ball.h"

Ball::Ball(SceneManager* mSceneMgr, String name, String node_name) {
	entBall = mSceneMgr->createEntity(name, "sphere.mesh");
	ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
	ballNode->setScale(.5, .5, .5);
	ballNode->attachObject(entBall);
}

void Ball::set_origin(btVector3 origin, btQuaternion rotation) {
	ballTransform.setIdentity();
	ballTransform.setRotation(rotation);
	ballTransform.setOrigin(origin);
}

void Ball::set_bounding_box(btScalar radius) {
	ballShape = new btSphereShape(radius);
}

void Ball::create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution) {
	simulator->getCollisionShapes().push_back(ballShape);
	ballShape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* ballMotionState = new btDefaultMotionState(ballTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ballMotionState, ballShape, inertia);
	ballRB = new btRigidBody(rbInfo);
	ballRB->activate(true);
	ballRB->setRestitution(restitution);
	ballRB->setUserPointer(ballNode);

	simulator->getDynamicsWorld()->addRigidBody(ballRB);
}

btRigidBody* Ball::get_rigidbody() {
	return ballRB;
}