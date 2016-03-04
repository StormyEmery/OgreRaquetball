/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"


using namespace Ogre;

const int dimensions = 1000;
const int radius = 50;
const float speed = 250.0;

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
    
    already_detected = false;
    score = 0;
    
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{

    simulator = new Simulator();

    mSceneMgr->setAmbientLight(ColourValue(0.25,0.25,0.25));

    Plain plain1(mSceneMgr, Vector3::UNIT_Y, Vector3::UNIT_Z, 1500, 3000, -500, "ground1", "node_ground1", "Examples/BumpyMetal");
    plain1.set_origin(btVector3(btScalar(0), btScalar(-500), btScalar(0)));
    plain1.create_bounding_box(simulator);

    Plain plain2(mSceneMgr, Vector3::NEGATIVE_UNIT_Y, Vector3::UNIT_Z, 1500, 3000, -500, "ground2", "node_ground2", "Examples/BumpyMetal");
    plain2.set_origin(btVector3(btScalar(0), btScalar(500), btScalar(0)));
    plain2.create_bounding_box(simulator);

    Plain plain3(mSceneMgr, Vector3::UNIT_X, Vector3::UNIT_Y, 3000, 1000, -750, "ground3", "node_ground3", "Examples/BumpyMetal");
    plain3.set_origin(btVector3(btScalar(-750), btScalar(0), btScalar(0)));
    plain3.create_bounding_box(simulator);

    Plain plain4(mSceneMgr, Vector3::NEGATIVE_UNIT_X, Vector3::UNIT_Y, 3000, 1000, -750, "ground4", "node_ground4", "Examples/BumpyMetal");
    plain4.set_origin(btVector3(btScalar(750), btScalar(0), btScalar(0)));
    plain4.create_bounding_box(simulator);

    Plain plain5(mSceneMgr, Vector3::UNIT_Z, Vector3::UNIT_X, 1000, 1500, -1500, "ground5", "node_ground5", "Examples/BumpyMetal");
    plain5.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(-1500)));
    plain5.create_bounding_box(simulator);


    Plain plain6(mSceneMgr, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_X, 1000, 1500, -1500, "ground6", "node_ground6", "Examples/BumpyMetal");
    plain6.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(1500)));
    plain6.create_bounding_box(simulator);
                                                           //W  //H
    Plain goal(mSceneMgr, Vector3::UNIT_Z, Vector3::UNIT_X, 400, 750, -1450, "goal", "node_goal", "Examples/Chrome");
    goal.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(-1450)));
    goal.create_bounding_box(simulator);
    
    Paddle paddle(mCamera, mSceneMgr, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_X, 350, 350, -750, "paddle", "node_paddle", "");
    paddle.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(750)));
    paddle.create_bounding_box(simulator);

    //paddle.groundNode->attachObject(mCamera);

/*    SceneNode* player = mSceneMgr->getRootSceneNode()->createChildSceneNode("Player_Paddle");
    player->createChildSceneNode("Camera_Node")->attachObject(mCamera);
    player->createChildSceneNode("Player_Node")->attachObject(paddle.entGround);
    // printf("%s\n", paddle.groundNode->getName());*/
    // Ogre::Entity* PaddleCube = mSceneMgr->createEntity("PaddleCube", "cube.mesh");
    // SceneNode* PadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PadNode");
    // PadNode->attachObject(PaddleCube);
    // PadNode->attachObject(mCamera);

    Ball* b = new Ball(mSceneMgr, "node_ball");
    b->reset(mSceneMgr, b, simulator);
    setBall(b);

    srand(time(NULL));

    Light* light = mSceneMgr->createLight("MainLight");
    light->setDiffuseColour(153, 0, 0);
    light->setSpecularColour(1,1,1);
    light->setType(Light::LT_SPOTLIGHT);
    light->setPosition(0,dimensions,0);
    light->setDirection(0, -1, 0);
    light->setSpotlightRange(Degree(50), Degree(75));

    Light* light2 = mSceneMgr->createLight("SecondaryLight");
    light2->setDiffuseColour(0, 102, 204);
    light2->setSpecularColour(1,1,1);
    light2->setType(Light::LT_SPOTLIGHT);
    light2->setPosition(0,-dimensions,0);
    light2->setDirection(0, 1, 0);
    light2->setSpotlightRange(Degree(50), Degree(75));

    Light* light1 = mSceneMgr->createLight("ThirdLight");
    light1->setDiffuseColour(1,1,1);
    light1->setPosition(0,50,50);
}

void TutorialApplication::createCamera() {
    mCamera = mSceneMgr->createCamera("PlayerCam");
    mCamera->setPosition(Vector3(0, 250, 2000));
    mCamera->lookAt(Vector3(0,0,0));
    mCamera->setNearClipDistance(5);
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
    // paddlePlayer->groundNode->attachObject(mCamera);
 }

void TutorialApplication::createViewports() {
     Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    mCamera->setAspectRatio(
        Real(vp->getActualWidth() /
        Real(vp->getActualHeight())));
}

bool TutorialApplication::frameStarted(const FrameEvent& fe) {
    bool ret = Assignment2::frameRenderingQueued(fe);
    
    desired_velocity = ball->ballRB->getLinearVelocity().length();

    if(background_music && !main_menu)
        game_music.play(-1);
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    sound_duration = ( std::clock() - sound_clock ) / (double) CLOCKS_PER_SEC;
    
    if(duration > .1){
        score_ok = true;    
    }

    if(simulator != NULL) {
        if(oneFrame) {
            mPause = false; 
        }
        if(!mPause && !main_menu){
             if(simulator != NULL) {
                simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f);
                int numManifolds = simulator->getDispatcher()->getNumManifolds();
                for(int i = 0; i < numManifolds; i++) {
                    btPersistentManifold* contactManifold = simulator->getDispatcher()->getManifoldByIndexInternal(i);
                    const btCollisionObject* obOne = contactManifold->getBody0();
                    const btCollisionObject* obTwo = contactManifold->getBody1();

                    const String obOneName = getName(obOne);
                    const String obTwoName = getName(obTwo);

                    std::cout << "Collision Object One: " << obOneName << "\n";
                    std::cout << "Collision Object Two: " << obTwoName << "\n";
                    if(obOneName == "node_ground1" && obTwoName == "node_ball" && sound_duration > .1) {
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else{ sound_clock = clock(); }

                    if(obOneName == "node_ground2" && obTwoName == "node_ball" && sound_duration > .1) { 
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else { sound_clock = clock(); }

                    if(obOneName == "node_ground3" && obTwoName == "node_ball" && sound_duration > .1) { 
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else { sound_clock = clock(); }

                    if(obOneName == "node_ground4" && obTwoName == "node_ball" && sound_duration > .1) { 
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else { sound_clock = clock(); }

                    if(obOneName == "node_ground5" && obTwoName == "node_ball" && sound_duration > .1) { 
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else { sound_clock = clock(); }

                    if(obOneName == "node_ground6" && obTwoName == "node_ball" && sound_duration > .1) { 
                        wall_collision_sound.play(0); 
                        sound_clock = clock();
                        break; 
                    }
                    else { sound_clock = clock(); }

                    if(obOneName == "node_paddle" && obTwoName == "node_ball"){
                        paddle_collision_sound.play(0);
                        //ball->a();
                    } 

                    if(obOneName == "node_goal" && obTwoName == "node_ball" && score_ok) {
                            start = clock();
                            score_sound.play(0);
                            score++;
                            score_ok = false;
                            break;
                    }
                }

                btTransform trans;
                ball->get_rigidbody()->getMotionState()->getWorldTransform(trans);
         
                void* userPointer = ball->get_rigidbody()->getUserPointer();
                if(userPointer) {
                    btQuaternion orientation = trans.getRotation();
                    SceneNode* sceneNode = static_cast<SceneNode*>(userPointer);
                    sceneNode->setPosition(Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));            
                    sceneNode->setOrientation(Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
                }
            }
        }
        if(oneFrame) {
            mPause = true;
            oneFrame = false;
        }

        btVector3 current_velocity_direction = ball->ballRB->getLinearVelocity();
        btScalar current_velocity = current_velocity_direction.length();
        if(current_velocity < desired_velocity) {
            current_velocity_direction *= desired_velocity/current_velocity;
            ball->ballRB->setLinearVelocity(current_velocity_direction);
        }

        return ret;
    }     
}   

bool TutorialApplication::processUnbufferedInput(const FrameEvent& fe) {
    static Real move = 5;
    return true;
}

Ball* TutorialApplication::getBall(){
    return ball;
}

void TutorialApplication::setBall(Ball* b){
    ball = b;
}

Simulator* TutorialApplication::getSimulator() {
    return simulator;
}

String TutorialApplication::getName(const btCollisionObject* collObj) {
    const btCollisionShape* shapeObj = collObj->getCollisionShape();
    void* shapeObjUserPointer = shapeObj->getUserPointer();
    SceneNode* shapeObjSceneNode = static_cast<SceneNode*>(shapeObjUserPointer);
    return shapeObjSceneNode->getName();
}

int TutorialApplication::getScore() {
    return score;
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------