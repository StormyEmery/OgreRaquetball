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
    player_one_score = 0;
    player_two_score = 0;
    
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{











 



    //===============================================================================================================================================================

    simulator = new Simulator();

    SceneNode* translateNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("translate");
    translateNode->translate(10000,0,1000);
    //translateNode->attachObject(mCamera);

    SceneNode* translateNodeTwo = mSceneMgr->getRootSceneNode()->createChildSceneNode("translateTwo");
    translateNodeTwo->translate(10000,0,-1000);
    //translateNodeTwo->attachObject(mCamera);

    SceneNode* translateNodeThree = mSceneMgr->getRootSceneNode()->createChildSceneNode("translateThree");
    translateNodeThree->translate(10000,0,0);

    mSceneMgr->setAmbientLight(ColourValue(0.25,0.25,0.25));

    Plain plain1(mSceneMgr, Vector3::UNIT_Y, Vector3::UNIT_Z, 1500, 3000, -500, "ground1", "node_ground1", "Examples/BumpyMetal");
    plain1.set_origin(btVector3(btScalar(0), btScalar(-500), btScalar(0)));
    plain1.create_bounding_box(simulator, 1.0f);

    Plain plain2(mSceneMgr, Vector3::NEGATIVE_UNIT_Y, Vector3::UNIT_Z, 1500, 3000, -500, "ground2", "node_ground2", "Examples/BumpyMetal");
    plain2.set_origin(btVector3(btScalar(0), btScalar(500), btScalar(0)));
    plain2.create_bounding_box(simulator, 1.0f);

    Plain plain3(mSceneMgr, Vector3::UNIT_X, Vector3::UNIT_Y, 3000, 1000, -750, "ground3", "node_ground3", "Examples/BumpyMetal");
    plain3.set_origin(btVector3(btScalar(-750), btScalar(0), btScalar(0)));
    plain3.create_bounding_box(simulator, 1.0f);

    Plain plain4(mSceneMgr, Vector3::NEGATIVE_UNIT_X, Vector3::UNIT_Y, 3000, 1000, -750, "ground4", "node_ground4", "Examples/BumpyMetal");
    plain4.set_origin(btVector3(btScalar(750), btScalar(0), btScalar(0)));
    plain4.create_bounding_box(simulator, 1.0f);

    Plain plain5(mSceneMgr, Vector3::UNIT_Z, Vector3::UNIT_X, 1000, 1500, -1500, "ground5", "node_ground5", "Examples/BumpyMetal");
    plain5.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(-1500)));
    plain5.create_bounding_box(simulator, 1.0f);

    Plain plain6(mSceneMgr, Vector3::NEGATIVE_UNIT_Z, Vector3::UNIT_X, 1000, 1500, -1500, "ground6", "node_ground6", "Examples/BumpyMetal");
    plain6.set_origin(btVector3(btScalar(0), btScalar(0), btScalar(1500)));
    plain6.create_bounding_box(simulator, 1.0f);

    


    Plain* g = new Plain(mSceneMgr, Vector3::UNIT_Z, Vector3::UNIT_X, 400, 750, 0, "goal", "node_goal", "Examples/Chrome", "translateThree");
    g->set_origin(btVector3(btScalar(0), btScalar(0), btScalar(10000)));
    g->create_bounding_box(simulator, 1.0f);
    setGoal(g);

    

    Paddle* paddleNew = new Paddle(mSceneMgr, "paddle", "node_paddle", Vector3(3, 2, .5), "translate");
    paddleNew->set_origin(btVector3(0, 0, 1000));
    paddleNew->create_bounding_box(simulator, 1.0f, 1.0f);
    setPaddleOne(paddleNew);


    Paddle* paddleNewTwo = new Paddle(mSceneMgr, "paddle2", "node_paddleTwo", Vector3(3, 2, .5), "translateTwo");
    paddleNewTwo->set_origin(btVector3(0, 0, 2000));
    paddleNewTwo->create_bounding_box(simulator, 1.0f, 1.0f);
    setPaddleTwo(paddleNewTwo);
    




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

bool TutorialApplication::quit(const CEGUI::EventArgs &e)
{
    mShutDown = true;
    return true;
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
    
    desired_velocity = ball->ballRB->getLinearVelocity().length();

    if(background_music && !main_menu)
        game_music.play(-1);
    single_duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    multi_duration = ( std::clock() - sound_clock2 ) / (double) CLOCKS_PER_SEC;
    sound_duration = ( std::clock() - sound_clock ) / (double) CLOCKS_PER_SEC;
    
    if(single_duration > .5){
        single_score_ok = true;    
    }

    if(multi_duration > .5){
        multi_score_ok = true;
    }

    if(simulator != NULL) {
        if(oneFrame) {
            mPause = false; 
        }
        if(!mPause && !main_menu){

            if(single_player){
                if(simulator != NULL) {
                    simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f);
                    int numManifolds = simulator->getDispatcher()->getNumManifolds();
                    for(int i = 0; i < numManifolds; i++) {
                        btPersistentManifold* contactManifold = simulator->getDispatcher()->getManifoldByIndexInternal(i);
                        const btCollisionObject* obOne = contactManifold->getBody0();
                        const btCollisionObject* obTwo = contactManifold->getBody1();

                        const String obOneName = getName(obOne);
                        const String obTwoName = getName(obTwo);

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

                        if(obOneName == "node_ground6" && obTwoName == "node_ball") { 
                            wall_collision_sound.play(0); 
                            sound_clock = clock();
                            mPause=true;
                            gameOver = true;
                            mTrayMgr->showCursor();
                            mTrayMgr->moveWidgetToTray(menu2,OgreBites::TL_CENTER, 0);
                            mTrayMgr->moveWidgetToTray(menu5,OgreBites::TL_CENTER, 0);
                            mTrayMgr->moveWidgetToTray(separator, OgreBites::TL_CENTER, 0);
                            mTrayMgr->moveWidgetToTray(gameOverLabel,OgreBites::TL_CENTER, 0);
                            menu2->show();
                            menu5->show();
                            separator->show();
                            gameOverLabel->show();

                            ball->reset(mSceneMgr, ball, simulator);
                        }
                        else { sound_clock = clock(); }

                        if(obOneName == "node_paddle" && obTwoName == "node_ball"){
                            paddle_collision_sound.play(0);
                            Quaternion qt = paddleOne->paddleNode->getOrientation();
                            Vector3 v = qt * Vector3::NEGATIVE_UNIT_Z;
                            ball->a(btVector3(-v.x, -v.y, -v.z));
                        }

                        if(obOneName == "node_goal" && obTwoName == "node_ball" && single_score_ok) {
                                start = clock();
                                score_sound.play(0);
                                player_one_score++;
                                single_score_ok = false;
                                break;
                        }
                    }

                    ball->updateTransform();

                }
            }
            if(multi_player){
                if(player_one_score == 8 || player_two_score == 8){
                    mPause=true;
                    gameOver = true;
                    mTrayMgr->showCursor();
                    mTrayMgr->moveWidgetToTray(menu2,OgreBites::TL_CENTER, 0);
                    mTrayMgr->moveWidgetToTray(menu5,OgreBites::TL_CENTER, 0);
                    mTrayMgr->moveWidgetToTray(separator, OgreBites::TL_CENTER, 0);
                    if(player_one_score == 8){
                        mTrayMgr->moveWidgetToTray(playerOneWins,OgreBites::TL_CENTER, 0);
                        playerOneWins->show();
                    }
                    else{
                        mTrayMgr->moveWidgetToTray(playerTwoWins,OgreBites::TL_CENTER, 0);
                        playerTwoWins->show();
                    }
                    menu2->show();
                    menu5->show();
                    separator->show();
                    
                    ball->reset(mSceneMgr, ball, simulator);
                }
                if(simulator != NULL) {
                    simulator->getDynamicsWorld()->stepSimulation(1.0f/60.0f);
                    int numManifolds = simulator->getDispatcher()->getNumManifolds();
                    for(int i = 0; i < numManifolds; i++) {
                        btPersistentManifold* contactManifold = simulator->getDispatcher()->getManifoldByIndexInternal(i);
                        const btCollisionObject* obOne = contactManifold->getBody0();
                        const btCollisionObject* obTwo = contactManifold->getBody1();

                        const String obOneName = getName(obOne);
                        const String obTwoName = getName(obTwo);

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

                        if(obOneName == "node_ground5" && obTwoName == "node_ball" && multi_score_ok) { 
                            wall_collision_sound.play(0); 
                            start = clock();
                            multi_score_ok = false;
                            sound_clock2 = clock();
                            player_one_score++;
                            break; 
                        }
                        else { sound_clock2 = clock(); }

                        if(obOneName == "node_ground6" && obTwoName == "node_ball" && multi_score_ok) { 
                            wall_collision_sound.play(0); 
                            start = clock();
                            multi_score_ok = false;
                            sound_clock2 = clock();
                            player_two_score++;
                            break; 
                        }
                        else { sound_clock2 = clock(); }

                        // if(obOneName == "node_ground6" && obTwoName == "node_ball"){
                        //     if(multi_duration > .5){
                        //         multi_score_ok = true;
                        //     }
                        //     else
                        //         sound_clock2 = clock();
                        // }
                        // if(obOneName == "node_ground5" && obTwoName == "node_ball"){
                        //     if(multi_duration > .5){
                        //         multi_score_ok = true;
                        //     }
                        //     else
                        //         sound_clock2 = clock();
                        // }

                        if(obOneName == "node_paddle" && obTwoName == "node_ball"){
                            score_sound.play(0);
                            Quaternion qt = paddleOne->paddleNode->getOrientation();
                            Vector3 v = qt * Vector3::NEGATIVE_UNIT_Z;
                            ball->a(btVector3(-v.x, -v.y, -v.z));
                        }

                        if(obOneName == "node_paddleTwo" && obTwoName == "node_ball"){
                            score_sound.play(0);
                            Quaternion qt = paddleTwo->paddleNode->getOrientation();
                            Vector3 v = qt * Vector3::NEGATIVE_UNIT_Z;
                            ball->a(btVector3(v.x, v.y, v.z));
                        }  
                    }

                    ball->updateTransform();

                }
            }
        }
        if(oneFrame) {
            mPause = true;
            oneFrame = false;
        }

        return ret;
    }     

}   

// bool TutorialApplication::mouseMoved( const OIS::MouseEvent &arg )
// {
//     // CEGUI::System &sys = CEGUI::System::getSingleton();
//     // sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
//     // return true;
// }

void TutorialApplication::InjectOISMouseButton(bool bButtonDown, OIS::MouseButtonID inButton)
{
    if (bButtonDown == true)
    {
        switch (inButton)
        {
        case OIS::MB_Left:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::LeftButton);
            break;
        case OIS::MB_Middle:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::MiddleButton);
            break;
        case OIS::MB_Right:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::RightButton);
            break;
        case OIS::MB_Button3:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::X1Button);
            break;
        case OIS::MB_Button4:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(CEGUI::X2Button);
            break;
        default:    
            break;
 
        }
    }
    else // bButtonDown = false
    {
        switch (inButton)
        {
        case OIS::MB_Left:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::LeftButton);
            break;
        case OIS::MB_Middle:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::MiddleButton);
            break;
        case OIS::MB_Right:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::RightButton);
            break;
        case OIS::MB_Button3:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::X1Button);
            break;
        case OIS::MB_Button4:
            CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(CEGUI::X2Button);
            break;
        default:    
            break;
        }
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

void TutorialApplication::setPaddleOne(Paddle* p) {
    paddleOne = p;
}

void TutorialApplication::setPaddleTwo(Paddle* p) {
    paddleTwo = p;
}

void TutorialApplication::setGoal(Plain* g){
    goal = g;
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

int TutorialApplication::getScoreOne() {
    return player_one_score;
}

int TutorialApplication::getScoreTwo() {
    return player_two_score;
}

void TutorialApplication::resetScore() {
    player_one_score = 0;
    player_two_score = 0;
}

void TutorialApplication::initializeGUI() {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    // CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    // CEGUI::Font::setDefaultResourceGroup("Fonts");
    // CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    // CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    // CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    // CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    // CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    // CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    // CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("TaharezLook.layout"); 
    // CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(guiRoot);

    // CEGUI::Window *quit = wmgr.createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    // quit->setText("Quit");
    // quit->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

    // sheet->addChild(quit);
    // CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
    // CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    
    // //Specify resource groups
    // rp->setResourceGroupDirectory("schemes", "./ceed/data/schemes/");
    // rp->setResourceGroupDirectory("imagesets", "/lusr/opt/cegui-0.8.4/share/cegui-0/imagesets/");
    // rp->setResourceGroupDirectory("fonts", "./ceed/data/fonts/");
    // rp->setResourceGroupDirectory("layouts", "./ceed/data/layouts/");
    // rp->setResourceGroupDirectory("looknfeels", "./ceed/data/looknfeel/");
    // rp->setResourceGroupDirectory("lua_scripts", "./ceed/data/lua_scripts/");

    // //Specify which resources groups for the system to use
    // CEGUI::Scheme::setDefaultResourceGroup("schemes");
    // CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");

    // CEGUI::Font::setDefaultResourceGroup("fonts");
    // CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    // CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    // CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    // CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont( "DejaVuSans-10" );
    // CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage( "TaharezLook/MouseArrow" );

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