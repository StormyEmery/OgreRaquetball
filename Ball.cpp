#include "Ball.h"


Ball::Ball(SceneManager* mSceneMgr, String node_name) {
	
	entBall = mSceneMgr->createEntity("sphere.mesh");
	entBall->setMaterialName("Examples/Chrome");
	ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
	ballNode->setScale(.5, .5, .5);
	ballNode->attachObject(entBall);

	jet_particle = mSceneMgr->createParticleSystem("jet", "Examples/JetEngine1");
	particleNode = ballNode->createChildSceneNode("Particle");
	particleNode->attachObject(jet_particle);
}

void Ball::set_origin(btVector3 origin, btQuaternion rotation) {
	ballTransform.setIdentity();
	ballTransform.setOrigin(origin);
	ballTransform.setRotation(rotation);
}

void Ball::set_bounding_box(btScalar radius) {
	ballShape = new btSphereShape(radius);
}

void Ball::create_bounding_box(Simulator* simulator, btScalar mass, btVector3 inertia, btScalar restitution) {
	simulator->getCollisionShapes()->push_back(ballShape);
	ballShape->calculateLocalInertia(mass, inertia);
	ballShape->setUserPointer(ballNode);
	// ballObject = new btCollisionObject();
 //    ballObject->setCollisionShape(ballShape);
 //    ballObject->setWorldTransform(ballTransform);
 //    // ballObject->forceActivationState(DISABLE_DEACTIVATION);
 //    simulator->getDynamicsWorld()->addCollisionObject(ballObject);

	btDefaultMotionState* ballMotionState = new btDefaultMotionState(ballTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ballMotionState, ballShape, inertia);
	ballRB = new btRigidBody(rbInfo);
	ballRB->activate(true);
	ballRB->setRestitution(restitution);
	ballRB->setUserPointer(ballNode);

	simulator->getDynamicsWorld()->addRigidBody(ballRB);
}

void Ball::reset(SceneManager* mSceneMgr, Ball* ball, Simulator* simulator) {
    btVector3 origin = btVector3(btScalar(0), btScalar(0), btScalar(0));
    btQuaternion rotation = btQuaternion(1.0f, 1.0f, 1.0f, 0);
    ball->set_origin(origin, rotation);
    ball->set_bounding_box(50.0f);
    btVector3 inertia = btVector3(0.0f, 0.0f, 0.0f);
    btScalar restitution = 1.0f;
    ball->create_bounding_box(simulator, .09f, inertia, restitution);

    //will need to change this later
    ball->get_rigidbody()->applyCentralForce(btVector3(btScalar(0), btScalar(0), btScalar(-1000)));
}

void Ball::a(){
	ballRB->applyCentralForce(btVector3(btScalar(0), btScalar(0), btScalar(-100)));
}

btRigidBody* Ball::get_rigidbody() {
	return ballRB;
}

Ball::~Ball(){
	delete ballShape;
	delete ballObject;
	delete ballNode;
	delete entBall;
	delete ballRB;
}