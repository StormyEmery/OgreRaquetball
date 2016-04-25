#include "Powerup.h"

Powerup::Powerup(SceneManager* mSceneMgr, String c, String entity_name, String node_name){
    color = c;

	entity = mSceneMgr->createEntity(entity_name,"cube.mesh");
    if(color == "blue")
        entity->setMaterialName("usermade/blue");
    if(color == "red")
        entity->setMaterialName("usermade/red");
    if(color == "yellow")
        entity->setMaterialName("usermade/yellow");
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode(node_name);
    node->setScale(2, 2, 2);
    node->attachObject(entity);
};

String Powerup::get_color(){
    return color;
}


Powerup::~Powerup(){};

