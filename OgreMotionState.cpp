#include <OgreMotionState.h>



OgreMotionState::OgreMotionState(const btTransform &initialpos, Ogre::SceneNode* node) :
	mVisibleObj(node),
	mPos1(initialpos)
	{}


void OgreMotionState::setNode(Ogre::SceneNode* node) { 
	mVisibleObj = node;
}

void OgreMotionState::getWorldTransform(btTransform &worldTrans) const {
	worldTrans = mPos1;
}

void OgreMotionState::setWorldTransform(const btTransform & worldTrans) {
	if(mVisibleObj == NULL)
		return;

	btQuaternion rot = worldTrans.getRotation();
	mVisibleObj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
	btVector3 pos = worldTrans.getOrigin();
	mVisibleObj->setPosition(pos.x(), pos.y(), pos.z());
}

void OgreMotionState::updateTransform(btTransform& newpos){
	mPos1 = newpos;
}