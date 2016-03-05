#include "Paddle.h"

Paddle::Paddle(SceneManager* mSceneMgr, String name, String node, Vector3 scale){
	paddleEntity = mSceneMgr->createEntity(name,"cube.mesh");
    paddleEntity->setMaterialName("Examples/Chrome");
    translationNode = mSceneMgr->getSceneNode("translate");
    paddleNode = mSceneMgr->getSceneNode("translate")->createChildSceneNode(node);
    paddleNode->setScale(scale);
    paddleNode->attachObject(paddleEntity);
    paddleNode->showBoundingBox(true);
}

		
Paddle::~Paddle(void){}

void Paddle::set_origin(btVector3 origin) {
	paddleTransform.setIdentity();
	paddleTransform.setOrigin(origin);
	this->set_bounding_box();
}

void Paddle::set_bounding_box() {
	AxisAlignedBox boundingB = paddleEntity->getBoundingBox();
    Vector3 size = Vector3::ZERO;
    size = boundingB.getSize();
    // std::cout << size.x *1.5 << ":" << size.y << ":" << size.z << std::endl;
    paddleShape = new btBoxShape(btVector3(size.x*1.5, size.y, size.z*.25));
}

void Paddle::create_bounding_box(Simulator* simulator, btScalar mass, btScalar restitution) {
	paddleShape->setUserPointer(paddleNode);
	paddleObject = new btCollisionObject();
	paddleObject->setCollisionShape(paddleShape);
	paddleObject->setWorldTransform(paddleTransform); //might need to remove or update somewhere else?
	paddleObject->forceActivationState(DISABLE_DEACTIVATION);
	simulator->getDynamicsWorld()->addCollisionObject(paddleObject);

	paddleMotionState = new OgreMotionState(paddleTransform, paddleNode);

	btVector3 localInertia(0,0,0);
	paddleShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo paddleRigidBodyInfo(mass, paddleMotionState, paddleShape, localInertia);

	paddleRigidBody = new btRigidBody(paddleRigidBodyInfo);
	paddleRigidBody->setRestitution(restitution);
	paddleRigidBody->setCollisionFlags(paddleRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	paddleRigidBody->setActivationState(DISABLE_DEACTIVATION);
	paddleRigidBody->setUserPointer(paddleNode);
	simulator->getDynamicsWorld()->addRigidBody(paddleRigidBody);
}

void Paddle::updateTransform() {
	Ogre::Vector3 pos = translationNode->_getDerivedPosition();
	paddleTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	Ogre::Quaternion qt = paddleNode->getOrientation();
	paddleTransform.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
	if(paddleMotionState)
		paddleMotionState->updateTransform(paddleTransform);
}

