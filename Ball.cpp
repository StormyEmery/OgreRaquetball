#include "Ball.h"


Ball::Ball(SceneManager* mSceneMgr, String node_name) {
	
	entBall = mSceneMgr->createEntity("sphere.mesh");
	entBall->setMaterialName("Examples/Chrome");
	entBall->setCastShadows(true);
	ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
	ballNode->setScale(.5, .5, .5);
	ballNode->attachObject(entBall);

	jet_particle = mSceneMgr->createParticleSystem("jet", "Examples/JetEngine1");
	particleNode = ballNode->createChildSceneNode("Particle");
	particleNode->attachObject(jet_particle);

	ballRB = NULL;
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
	
	if(ballRB) {
		simulator->getDynamicsWorld()->removeRigidBody(ballRB);
		delete ballMotionState;
		delete ballRB;
	}

	simulator->getCollisionShapes()->push_back(ballShape);
	ballShape->calculateLocalInertia(mass, inertia);
	ballShape->setUserPointer(ballNode);

	ballMotionState = new OgreMotionState(ballTransform, ballNode);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ballMotionState, ballShape, inertia);
	ballRB = new btRigidBody(rbInfo);
	//ballRB->activate(true);
	ballRB->setRestitution(restitution);
	ballRB->setUserPointer(ballNode);
	ballRB->setFriction(0.0);

	simulator->getDynamicsWorld()->addRigidBody(ballRB);
}

void Ball::reset(SceneManager* mSceneMgr, Ball* ball, Simulator* simulator) {
    btVector3 origin = btVector3(btScalar(0), btScalar(0), btScalar(0));
    btQuaternion rotation = btQuaternion(1.0f, 1.0f, 1.0f, 0);
    ball->set_origin(origin, rotation);
    ball->set_bounding_box(50.0f);
    btVector3 inertia = btVector3(0.0f, 0.0f, 0.0f);
    btScalar restitution = 1.0f;
    ball->create_bounding_box(simulator, 1.0f, inertia, restitution);

    ball->get_rigidbody()->applyCentralForce(btVector3(btScalar(0), btScalar(0), btScalar(-15000)));
}

void Ball::a(btVector3 dir){
	//dir.setY(dir.getY() + .75);
	ballRB->setLinearVelocity(dir.normalize()*250);
}

btRigidBody* Ball::get_rigidbody() {
	return ballRB;
}

void Ball::updateTransform(){
	Ogre::Vector3 pos = ballNode->getPosition();
	ballTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = ballNode->getOrientation();
	ballTransform.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	if(ballMotionState)
		ballMotionState->updateTransform(ballTransform);
}

btVector3 Ball::heading() {
	return ballRB->getLinearVelocity().normalized();
}


Ball::~Ball(){
	delete ballShape;
	delete ballObject;
	delete ballNode;
	delete entBall;
	delete ballRB;
	delete jet_particle;
	delete particleNode;
	delete ballMotionState;
}