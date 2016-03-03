#include "Plain.h"

Plain::Plain(SceneManager* mSceneMgr, Vector3 normal, Vector3 up_vector, float x, float y, float offset, String name, String node_name, String path){
	Plane plane(normal, offset);
	MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, x, y, 20, 20, true, 1, 5, 5, up_vector);	


	entGround = mSceneMgr->createEntity(name);
    entGround->setMaterialName(path);
    groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);

    // if(name != "paddle")
    groundNode->attachObject(entGround);
}



void Plain::set_origin(btVector3 origin){
    groundTransform.setIdentity();
    groundTransform.setOrigin(origin);
};

void Plain::set_bounding_box(btVector3 bounding_box){
	groundShape = new btBoxShape(bounding_box);
};	

void Plain::create_bounding_box(Simulator* simulator){
    simulator->getCollisionShapes()->push_back(groundShape);
    groundShape->setUserPointer(groundNode);
    groundObject = new btCollisionObject();
    groundObject->setCollisionShape(groundShape);
    groundObject->setWorldTransform(groundTransform);
    // groundObject->forceActivationState(DISABLE_DEACTIVATION);
    simulator->getDynamicsWorld()->addCollisionObject(groundObject);
	btScalar groundMass(0.0);
    btVector3 localGroundInertia(0,0,0);
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

    groundShape->calculateLocalInertia(groundMass, localGroundInertia);
    btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
    btRigidBody* groundBody = new btRigidBody(groundRBInfo);
    groundBody->setRestitution(1.0f);
    groundBody->setCollisionFlags(groundBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    groundBody->setActivationState(DISABLE_DEACTIVATION);
    simulator->getDynamicsWorld()->addRigidBody(groundBody);
    groundBody->setUserPointer(groundNode);
};


Plain::~Plain(){}