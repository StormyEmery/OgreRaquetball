#include "Powerup.h"

Powerup::Powerup(SceneManager* mSceneMgr, String c, String entity_name, String node_name){
    color = c;

	entity = mSceneMgr->createEntity(entity_name,"cube.mesh");
    entity->setMaterialName("Examples/Chrome");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
    node->setScale(2, 2, 2);
    node->attachObject(entity);
};

Powerup::~Powerup(){};

