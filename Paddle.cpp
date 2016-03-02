#include "Paddle.h"


Paddle::Paddle(SceneManager* mSceneMgr, Vector3 normal, Vector3 up_vector, float x, float y, float offset, String name, String node_name, String path){
	Plane plane(normal, offset);
	MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, x, y, 20, 20, true, 1, 5, 5, up_vector);	

	entGround = mSceneMgr->createEntity(name);
    entGround->setMaterialName(path);
    groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);

    groundNode->attachObject(entGround);
}
