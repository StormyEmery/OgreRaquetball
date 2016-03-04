#ifndef __OgreMotionState_h_
#define __OgreMotionState_h_


#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreSceneNode.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class OgreMotionState : public btMotionState 
{
protected:
	Ogre::SceneNode* mVisibleObj;
	btTransform mPos1;

public:

	OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node);

	virtual ~OgreMotionState() {}

	//Provides flexibility in terms of object visibility
	void setNode(Ogre::SceneNode* node);
	
	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);
	void updateTransform(btTransform &newpos);
};

#endif