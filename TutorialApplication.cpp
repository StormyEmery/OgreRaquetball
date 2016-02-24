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
    btRigidBody* body = NULL;
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

    Plane plane(Vector3::UNIT_Y, 0);
    MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
    
    Entity* entGround = mSceneMgr->createEntity("ground");
    entGround->setMaterialName("Examples/BumpyMetal");
    SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("groundNode");

    groundNode->attachObject(entGround);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,-50, 0));

    btScalar groundMass(0.0);
    btVector3 localGroundInertia(0,0,0);

    btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.0), btScalar(50.0), btScalar(50.0)));
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);

    groundShape->calculateLocalInertia(groundMass, localGroundInertia);
    btRigidBody::btRigidBodyConstructionInfo groundRBInfo(groundMass, groundMotionState, groundShape, localGroundInertia);
    btRigidBody* groundBody = new btRigidBody(groundRBInfo);
    simulator->getDynamicsWorld()->addRigidBody(groundBody);
    groundBody->setUserPointer(groundNode);
    //create gameobject, add it to list


    Entity* entity = mSceneMgr->createEntity("box", "sphere.mesh");
    SceneNode* newNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Cube");
    newNode->attachObject(entity);
    btScalar r = 100.0f;
    btCollisionShape *newRigidShape = new btSphereShape(r);
    simulator->getCollisionShapes().push_back(newRigidShape);

    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setRotation(btQuaternion(1.0f, 1.0f, 1.0f, 0));

    btScalar mass = 0.1f;
    btVector3 localInertia(0,0,0);

    startTransform.setOrigin(btVector3(0,500,0));
    newRigidShape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* ballMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, ballMotionState, newRigidShape, localInertia);
    body = new btRigidBody(rbInfo);
    body->setRestitution(1);
    body->setUserPointer(newNode);

    simulator->getDynamicsWorld()->addRigidBody(body);
    // simulator->trackRigidBodyWithName(body, "Sphere");

   
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

     if(simulator != NULL) {
        simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f);
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);

        void* userPointer = body->getUserPointer();
        if(userPointer) {
            btQuaternion orientation = trans.getRotation();
            SceneNode* sceneNode = static_cast<SceneNode*>(userPointer);
            sceneNode->setPosition(Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));            // std::cout << "Position: " << sceneNode->getPosition() << "\n";
            sceneNode->setOrientation(Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
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
