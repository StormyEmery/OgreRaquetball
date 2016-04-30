#include "PowerupSystem.h"

PowerupSystem::PowerupSystem(SceneManager* mSceneMgr){
    srand(time(0));

    blue = new Powerup(mSceneMgr, "blue", "ep1", "np1");
    red = new Powerup(mSceneMgr, "red", "ep2", "np2");
    yellow = new Powerup(mSceneMgr, "yellow", "ep3", "np3");

    blue->node->translate(-5000, 0, 0);
    red->node->translate(-5000, 0, 0);
    yellow->node->translate(-5000, 0, 0);

    size = blue->entity->getBoundingBox().getSize().x/2;

    

    colors.push_back("blue");
    colors.push_back("red");
    colors.push_back("yellow");


    current_color = colors[rand()%3];
    old_color = current_color; 
    set_nodes();
    std::cout << current_color << std::endl;


    timer = clock();

};

void PowerupSystem::timer_start(){
    current_time = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
};

void PowerupSystem::set_random_powerup(){
    old_color = current_color;
    current_color = colors[rand()%3];
    std::cout << current_color << std::endl;
}


void PowerupSystem::get_new_powerup(){
    float randx = (float)rand()/(float)RAND_MAX * 2000 - 1000;
    float randy = (float)rand()/(float)RAND_MAX * 1200 - 600;
    current_power_node->setPosition(randx, randy, 0);
    if(current_color != old_color){
        old_power_node->setPosition(-5000, 0, 0);
    }
}


void PowerupSystem::set_nodes(){
    if(current_color == "blue") current_power_node = blue->node;
    if(current_color == "red") current_power_node =  red->node;
    if(current_color == "yellow") current_power_node = yellow->node;
    if(old_color == "blue") old_power_node = blue->node;
    if(old_color == "red") old_power_node = red->node;
    if(old_color == "yellow") old_power_node = yellow->node;
}


void PowerupSystem::choose_powerup(){
    if(current_time >= 4){
        set_random_powerup();
        set_nodes();
        get_new_powerup();
        timer = clock();
    }
};


void PowerupSystem::remove_powerup(){
    current_power_node->setPosition(-5000, 0, 0);
    timer = clock();
}

void PowerupSystem::check_collision(Ball* b){

    float BX = b->ballNode->getPosition().x;
    float BY = b->ballNode->getPosition().y;
    float BZ = b->ballNode->getPosition().z;
    

    float PRX = current_power_node->getPosition().x + size;
    float PLX = current_power_node->getPosition().x - size;
    float PRY = current_power_node->getPosition().y + size;
    float PLY = current_power_node->getPosition().y - size;
    float PRZ = current_power_node->getPosition().z + size;
    float PLZ = current_power_node->getPosition().z - size;

    if(BX > PLX && BX < PRX){
        if(BY < PRY && BY > PLY){
            if(BZ < PRZ && BZ > PLZ){
                remove_powerup();
            }
        }
    }
}


PowerupSystem::~PowerupSystem(){};

