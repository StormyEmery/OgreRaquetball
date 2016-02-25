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
    Simulator* simulator = NULL;
    Ball* ball = NULL;
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

    Plain plain1(mSceneMgr, Vector3::UNIT_Y, Vector3::UNIT_Z, 1500, 1500, -750, "ground1", "node_ground1", "Examples/BumpyMetal");
    plain1.set_origin(btVector3(btScalar(0), btScalar(-750), btScalar(0)));
    plain1.set_bounding_box(btVector3(btScalar(1500), btScalar(0), btScalar(1500)));
    plain1.create_bounding_box(simulator);

    Plain plain2(mSceneMgr, Vector3::NEGATIVE_UNIT_Y, Vector3::UNIT_Z, 1500, 1500, -750, "ground2", "node_ground2", "Examples/BumpyMetal");
    plain2.set_origin(btVector3(btScalar(0), btScalar(750), btScalar(0)));
    plain2.set_bounding_box(btVector3(btScalar(1500), btScalar(0), btScalar(1500)));
    plain2.create_bounding_box(simulator);

    Plain plain3(mSceneMgr, Vector3::UNIT_X, Vector3::UNIT_Y, 1500, 1500, -750, "ground3", "node_ground3", "Examples/BumpyMetal");
    plain3.set_origin(btVector3(btScalar(-750), btScalar(0), btScalar(0)));
    plain3.set_bounding_box(btVector3(btScalar(0), btScalar(1500), btScalar(1500)));
    plain3.create_bounding_box(simulator);

    Plain plain4(mSceneMgr, Vector3::NEGATIVE_UNIT_X, Vector3::UNIT_Y, 1500, 1500, -750, "ground4", "node_ground4", "Examples/BumpyMetal");
    plain4.set_origin(btVector3(btScalar(750), btScalar(0), btScalar(0)));
    plain4.set_bounding_box(btVector3(btScalar(0), btScalar(1500), btScalar(1500)));
    plain4.create_bounding_box(simulator);

    Plain plain5(mSceneMgr, Vector3::UNIT_Z, Vector3::UNIT_X, 1500, 1500, -750, "ground5", "node_ground5", "Examples/BumpyMetal");
    plain5.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(-750)));
    plain5.set_bounding_box(btVector3(btScalar(1500), btScalar(1500), btScalar(0)));
    plain5.create_bounding_box(simulator);

    Plain plain6(mSceneMgr, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_X, 1500, 1500, -750, "ground6", "node_ground6", "Examples/BumpyMetal");
    plain6.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(750)));
    plain6.set_bounding_box(btVector3(btScalar(1500), btScalar(1500), btScalar(0)));
    plain6.create_bounding_box(simulator);

    ball = new Ball(mSceneMgr, "ball", "node_ball");
    btVector3 origin = btVector3(btScalar(0), btScalar(500), btScalar(0));
    btQuaternion rotation = btQuaternion(1.0f, 1.0f, 1.0f, 0);
    ball->set_origin(origin, rotation);
    ball->set_bounding_box(50.0f);
    btVector3 inertia = btVector3(0.0f, 0.0f, 0.0f);
    btScalar restitution = .95f;
    ball->create_bounding_box(simulator, .2f, inertia, restitution);

    ball->get_rigidbody()->applyCentralForce(btVector3(btScalar(2500.0f), btScalar(0.0f), btScalar(2500.0f)));
   
    //ball
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
    if(!mPause){
     if(simulator != NULL) {
        simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f);
        btTransform trans;
        ball->get_rigidbody()->getMotionState()->getWorldTransform(trans);
 
        // ball->get_rigidbody()->applyCentralForce(btVector3(btScalar(-25.0f), btScalar(25.0f), btScalar(0.0f)));
        void* userPointer = ball->get_rigidbody()->getUserPointer();
        if(userPointer) {
            btQuaternion orientation = trans.getRotation();
            SceneNode* sceneNode = static_cast<SceneNode*>(userPointer);
            sceneNode->setPosition(Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));            // std::cout << "Position: " << sceneNode->getPosition() << "\n";
            sceneNode->setOrientation(Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
        }
     }
 }

    return ret;
}

bool TutorialApplication::processUnbufferedInput(const FrameEvent& fe) {
    static Real move = 5;
    return true;
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
