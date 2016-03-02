#include "Paddle.h"

Paddle::Paddle(SceneManager* mSceneMgr, String name, String node_name) {
	entPaddle = mSceneMgr->createEntity(name, "sphere.mesh");
	paddleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
	paddleNode->setScale(.5, .5, .5);
	paddleNode->attachObject(entPaddle);
}


void Paddle:create(){
	Plain plain1(mSceneMgr, Vector3::UNIT_Y, Vector3::UNIT_Z, 1500, 2000, -750, "paddle", "node_paddle", "Examples/BumpyMetal");
    plain1.set_origin(btVector3(btScalar(0), btScalar(-750), btScalar(0)));
    plain1.set_bounding_box(btVector3(btScalar(1500), btScalar(0), btScalar(2000)));
    plain1.create_bounding_box(simulator);
}




void Paddle::set_origin(btVector3 origin, btQuaternion rotation) {
	paddleTransform.setIdentity();
	paddleTransform.setRotation(rotation);
	paddleTransform.setOrigin(origin);
}

void Paddle::set_bounding_box(btScalar radius) {
	paddleShape = new btSphereShape(radius);
}

void Paddle::create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution) {
	simulator->getCollisionShapes()->push_back(paddleShape);
	paddleShape->calculateLocalInertia(mass, inertia);
	paddleShape->setUserPointer(paddleNode);
	paddleObject = new btCollisionObject();
    paddleObject->setCollisionShape(paddleShape);
    paddleObject->setWorldTransform(paddleTransform);
    // paddleObject->forceActivationState(DISABLE_DEACTIVATION);
    simulator->getDynamicsWorld()->addCollisionObject(paddleObject);

	btDefaultMotionState* paddleMotionState = new btDefaultMotionState(paddleTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, paddleMotionState, paddleShape, inertia);
	paddleRB = new btRigidBody(rbInfo);
	paddleRB->activate(true);
	paddleRB->setRestitution(restitution);
	paddleRB->setUserPointer(ballNode);

	simulator->getDynamicsWorld()->addRigidBody(paddleRB);
}

void Paddle::reset(SceneManager* mSceneMgr, Paddle* paddle, Simulator* simulator) {
    btVector3 origin = btVector3(btScalar(0), btScalar(500), btScalar(0));
    btQuaternion rotation = btQuaternion(1.0f, 1.0f, 1.0f, 0);
    paddle->set_origin(origin, rotation);
    paddle->set_bounding_box(50.0f);
    btVector3 inertia = btVector3(0.0f, 0.0f, 0.0f);
    btScalar restitution = .95f;
    paddle->create_bounding_box(simulator, .2f, inertia, restitution);

    //will need to change this later
    // paddle->get_rigidbody()->applyCentralForce(btVector3(btScalar(rand()%5000 - 2500), btScalar(rand()%5000 - 2500), btScalar(rand()%5000 - 2500)));
    paddle->get_rigidbody()->applyCentralForce(btVector3(btScalar(0), btScalar(0), btScalar(-1000)));
}

btRigidBody* Paddle::get_rigidbody() {
	return paddleRB;
}