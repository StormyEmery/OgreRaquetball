#include <GameObject.h>

GameObject::GameObject(Ogre::String n, Ogre::SceneManager* sM, Simulator* sim, Ogre::SceneNode* sN,
			   Ogre::Entity* ent, btCollisionShape* s, btScalar m, btRigidBody* b,
			   btTransform tr, btVector3 inert, OgreMotionState* mS) : 
	name(n), 
	sceneMgr(sM),
	simulator(sim),
	rootNode(sN),
	entity(ent),
	shape(s),
	mass(m),
	body(b),
	transform(tr),
	inertia(inert),
	motionState(mS)
{}

Ogre::String GameObject::getName(void) { return name;}

Ogre::SceneManager* GameObject::getSceneManager(void) { return sceneMgr;}

Simulator* GameObject::getSimulator(void) { return simulator;}

Ogre::SceneNode* GameObject::getSceneNode(void) { return rootNode;}

Ogre::Entity* GameObject::getEntity(void) { return entity;}

btCollisionShape* GameObject::getShape(void) { return shape;}

btScalar GameObject::getMass(void) {return mass;}

btRigidBody* GameObject::getBody(void) { return body;}

btTransform GameObject::getTransform(void) { return transform;}

btVector3 GameObject::getInertia(void) { return inertia;}

OgreMotionState* GameObject::getMotionState(void) { return motionState;}