#include "PowerupSystem.h"

PowerupSystem::PowerupSystem(SceneManager* mSceneMgr){
    blue = new Powerup(mSceneMgr, "blue", "ep1", "np1");
    red = new Powerup(mSceneMgr, "red", "ep2", "np2");
    yellow = new Powerup(mSceneMgr, "yellow", "ep3", "np3");

    blue->node->translate(0, 0, 0);
    red->node->translate(-5000, 0, 0);
    yellow->node->translate(-5000, 0, 0);

    current = "blue";
    size = blue->entity->getBoundingBox().getSize().x/2;

    timer = clock();

};

void PowerupSystem::timer_start(){
    time = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
};

void PowerupSystem::choose_powerup(){
    if(time >= 4){
        if(current == "blue"){
            blue->node->translate(-5000, 0, 0);
            red->node->translate(5000, 0, 0);
            current = red->get_color();
            timer = clock();
        }
        else if(current == "red"){
            red->node->translate(-5000, 0, 0);
            yellow->node->translate(5000, 0, 0);
            current = yellow->get_color();
            std::cout << current << std::endl;
            timer = clock();
        }
        else if(current == "yellow"){
            yellow->node->translate(-5000, 0, 0);
            blue->node->translate(5000, 0, 0);
            current = blue->get_color();
            std::cout << current << std::endl;
            timer = clock();
        }
    }
};

void PowerupSystem::check_collision(Ball* b){

    if(b->ballNode->getPosition().x > blue->node->getPosition().x - size && b->ballNode->getPosition().x < blue->node->getPosition().x + size){
        if(b->ballNode->getPosition().y > blue->node->getPosition().y - size && b->ballNode->getPosition().y < blue->node->getPosition().y + size){
            if(b->ballNode->getPosition().z > blue->node->getPosition().z - size && b->ballNode->getPosition().z < blue->node->getPosition().z + size){
                std::cout << "hit" << std::endl; 
            }
        }
    }

}


PowerupSystem::~PowerupSystem(){};

