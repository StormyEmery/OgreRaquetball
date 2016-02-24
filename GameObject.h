#ifndef __GameObject_h_
#define __GameObject_h_

#include <OgreMotionState.h>

class Simulator;

class GameObject {
protected:
	Ogre::String name;
	Ogre::SceneManager* sceneMgr;
	Simulator* simulator;
	Ogre::SceneNode* rootNode;
	Ogre::Entity* entity;
	btCollisionShape* shape;
	btScalar mass;
	btRigidBody* body;
	btTransform transform;
	btVector3 inertia;
	OgreMotionState* motionState;

public:
	GameObject(Ogre::String n, Ogre::SceneManager* sM, Simulator* sim, Ogre::SceneNode* sN,
			   Ogre::Entity* ent, btCollisionShape* s, btScalar m, btRigidBody* b,
			   btTransform tr, btVector3 inert, OgreMotionState* mS);
	~GameObject(void);

	Ogre::String getName(void);
	Ogre::SceneManager* getSceneManager(void);
	Simulator* getSimulator(void);
	Ogre::SceneNode* getSceneNode(void);
	Ogre::Entity* getEntity(void);
	btCollisionShape* getShape(void);
	btScalar getMass(void);
	btRigidBody* getBody(void);
	btTransform getTransform(void);
	btVector3 getInertia(void);
	OgreMotionState* getMotionState(void);
};

#endif