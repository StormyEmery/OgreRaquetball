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

    

    //colors.push_back("blue");
    //colors.push_back("red");
    colors.push_back("yellow");


    current_color = colors[rand()%colors.size()];
    old_color = current_color; 
    set_nodes();

    timer = clock();

};

void PowerupSystem::timer_start(){
    current_time = ( std::clock() - timer ) / (double) CLOCKS_PER_SEC;
};

void PowerupSystem::set_random_powerup(){
    old_color = current_color;
    if(colors.size() != 0)
        current_color = colors[rand()%colors.size()];
}


void PowerupSystem::get_new_powerup(){
    float randx = (float)rand()/(float)RAND_MAX * 2000 - 1000;
    float randy = (float)rand()/(float)RAND_MAX * 1200 - 600;
    current_powerup->set_position(0, 0, 0);
    if(current_color != old_color){
        old_powerup->set_position(-5000, 0, 0);
    }
}


void PowerupSystem::set_nodes(){
    if(current_color == "blue") current_powerup = blue;
    if(current_color == "red") current_powerup =  red;
    if(current_color == "yellow") current_powerup = yellow;
    if(old_color == "blue") old_powerup = blue;
    if(old_color == "red") old_powerup = red;
    if(old_color == "yellow") old_powerup = yellow;
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
    current_powerup->set_position(-5000, 0, 0);
    timer = clock();
}


void PowerupSystem::activate_powerup(Paddle* one, Paddle* two, Light* light){
    if(current_powerup->is_active()){
        if(current_color == "blue"){
            std::cout << "blue" << std::endl;
        }
        if(current_color == "red"){
            std::cout << "red" << std::endl;
        }
        if(current_color == "yellow"){
            current_powerup->activate(light);
           // colors.erase("yellow");
        }
    }
}


void PowerupSystem::check_collision(Ball* b){

    float BX = b->ballNode->getPosition().x;
    float BY = b->ballNode->getPosition().y;
    float BZ = b->ballNode->getPosition().z;
    

    float PRX = current_powerup->get_position().x + size;
    float PLX = current_powerup->get_position().x - size;
    float PRY = current_powerup->get_position().y + size;
    float PLY = current_powerup->get_position().y - size;
    float PRZ = current_powerup->get_position().z + size;
    float PLZ = current_powerup->get_position().z - size;

    if(BX > PLX && BX < PRX){
        if(BY < PRY && BY > PLY){
            if(BZ < PRZ && BZ > PLZ){
                remove_powerup();
                current_powerup->set_active();
            }
        }
    }
}


PowerupSystem::~PowerupSystem(){};

