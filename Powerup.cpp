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
    
    active = false;

    timer = clock();
};


void Powerup::set_position(float x, float y, float z){
    node->setPosition(x, y, z);
}

Vector3 Powerup::get_position(){
    return node->getPosition();
}

void Powerup::set_active(){
    timer = clock();
    active = true;
}

void Powerup::set_inactive(){
    active = false;
}

bool Powerup::is_active(){
    return active;
}

void Powerup::activate(Light* light){
    current_time = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
    light->setVisible(false);
    std::cout << "Ok" << std::endl;
    if(current_time >= 4){
        std::cout << "yes" << std::endl;
        light->setVisible(true);
        active = false;
    }
    
}    

String Powerup::get_color(){
    return color;
}

Powerup::~Powerup(){};

