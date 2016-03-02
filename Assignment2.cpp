#include "Assignment2.h"

Assignment2::Assignment2(void) :
    mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0),
    mBaller(0),
    x(0),
    y(0),
    z(0),


    //test("music/wall_collision.wav", 1),
    
    game_music("music/track1.wav", 0),
    menu_sound("music/menu_sound.wav", 1),
    button_sound("music/button_click.wav", 1),
    score_sound("music/score.wav", 1),
    wall_collision_sound("music/wall_collision.wav", 1),
    paddle_collision_sound("music/paddle_collision.wav", 1),

    mPause(false),
    mMenu(0),
    mScore(0),
    oneFrame(false)

{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}

//---------------------------------------------------------------------------
Assignment2::~Assignment2(void)
{
    if (mTrayMgr) delete mTrayMgr;
    if (mCameraMan) delete mCameraMan;
    if (mOverlaySystem) delete mOverlaySystem;

    // Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//---------------------------------------------------------------------------
bool Assignment2::configure(void)
{
    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise.
        // Here we choose to let the system create a default rendering window by passing 'true'.
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void Assignment2::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//---------------------------------------------------------------------------
void Assignment2::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(0,0,80));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,-300));
    mCamera->setNearClipDistance(5);

    mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default camera controller
}
//---------------------------------------------------------------------------
void Assignment2::createFrameListener(void)
{
    std::cout << "HELLO\n";
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("TestScore:");
    items.push_back("");
    items.push_back("Ball.pX");
    items.push_back("Ball.pY");
    items.push_back("Ball.pZ");

    menu1 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton1", "Reset Ball", 250);
    menu2 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton2", "Exit", 50);
    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(getScore()));
    mDetailsPanel->show();
    mRoot->addFrameListener(this);

    menu1->hide();
    menu2->hide();
    mTrayMgr->removeWidgetFromTray(menu1);
    mTrayMgr->removeWidgetFromTray(menu2);
    mTrayMgr->setTrayPadding(12);
    mTrayMgr->hideBackdrop();

}
//---------------------------------------------------------------------------
void Assignment2::destroyScene(void)
{
}
//---------------------------------------------------------------------------
void Assignment2::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void Assignment2::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//---------------------------------------------------------------------------
void Assignment2::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void Assignment2::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
void Assignment2::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // Clean up
    destroyScene();
}
//---------------------------------------------------------------------------
bool Assignment2::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

    // Create the scene
    createScene();
    // mBaller= getBall();

    createFrameListener();

    return true;
};
//---------------------------------------------------------------------------
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        mCameraMan->frameRenderingQueued(evt);   // If dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(getScore()));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mSceneMgr->getSceneNode("node_ball")->getPosition().x));
            mDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(mSceneMgr->getSceneNode("node_ball")->getPosition().y));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mSceneMgr->getSceneNode("node_ball")->getPosition().z));
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::keyPressed( const OIS::KeyEvent &arg )
{
    //if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_ESCAPE){
        if(!mPause) {
            //play sound here
            game_music.pause();
            menu_sound.play(0);
            mTrayMgr->showCursor();
            mPause=true;
            menu1->show();
            menu2->show();
            mTrayMgr->moveWidgetToTray(menu2, OgreBites::TL_CENTER, 0);
            mTrayMgr->moveWidgetToTray(menu1, OgreBites::TL_CENTER, 0);
        }   
        else{
            menu_sound.play(0);
            game_music.resume();
            mTrayMgr->hideCursor();
            mPause=false;
            mTrayMgr->removeWidgetFromTray(menu1);
            mTrayMgr->removeWidgetFromTray(menu2);
            menu1->hide();
            menu2->hide();
            
        }

    }



    // if(arg.key == OIS::KC_Q){
    //     test.play(0);
    // }
    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();

        //-------------------
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    // else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    // {
    //     Ogre::String newVal;
    //     Ogre::TextureFilterOptions tfo;
    //     unsigned int aniso;

    //     switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
    //     {
    //     case 'B':
    //         newVal = "Trilinear";
    //         tfo = Ogre::TFO_TRILINEAR;
    //         aniso = 1;
    //         break;
    //     case 'T':
    //         newVal = "Anisotropic";
    //         tfo = Ogre::TFO_ANISOTROPIC;
    //         aniso = 8;
    //         break;
    //     case 'A':
    //         newVal = "None";
    //         tfo = Ogre::TFO_NONE;
    //         aniso = 1;
    //         break;
    //     default:
    //         newVal = "Bilinear";
    //         tfo = Ogre::TFO_BILINEAR;
    //         aniso = 1;
    //     }

    //     Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
    //     Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
    //     mDetailsPanel->setParamValue(9, newVal);
    // }
    // else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    // {
    //     Ogre::String newVal;
    //     Ogre::PolygonMode pm;

    //     switch (mCamera->getPolygonMode())
    //     {
    //     case Ogre::PM_SOLID:
    //         newVal = "Wireframe";
    //         pm = Ogre::PM_WIREFRAME;
    //         break;
    //     case Ogre::PM_WIREFRAME:
    //         newVal = "Points";
    //         pm = Ogre::PM_POINTS;
    //         break;
    //     default:
    //         newVal = "Solid";
    //         pm = Ogre::PM_SOLID;
    //     }

    //     mCamera->setPolygonMode(pm);
    //     mDetailsPanel->setParamValue(10, newVal);
    // }
    // else if(arg.key == OIS::KC_F5)   // refresh all textures
    // {
    //     Ogre::TextureManager::getSingleton().reloadAll();
    // }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_F12)
    {
        mShutDown = true;
    }
    if(mPause) return false;
    mCameraMan->injectKeyDown(arg);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::keyReleased(const OIS::KeyEvent &arg)
{
    if(mPause) return false;
    mCameraMan->injectKeyUp(arg);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mouseMoved(const OIS::MouseEvent &arg)
{
   
    if (mTrayMgr->injectMouseMove(arg)) return true;
    if(!mPause)
        mCameraMan->injectMouseMove(arg);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
 
    if (mTrayMgr->injectMouseDown(arg, id)) return true;
    if(!mPause)
        mCameraMan->injectMouseDown(arg, id);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    if(!mPause)
        mCameraMan->injectMouseUp(arg, id);
    return true;
}
//---------------------------------------------------------------------------
// Adjust mouse clipping area
void Assignment2::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void Assignment2::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

void Assignment2::buttonHit(OgreBites::Button * button) {
    if(button->getName() == "MyButton1") {
        button_sound.play(0);
        // mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(++mScore));
        getBall()->reset(mSceneMgr, getBall(), getSimulator());
        oneFrame=true;
    }
    else if(button->getName()=="MyButton2"){
        button_sound.play(0);
        mShutDown = true;
    }
}



//---------------------------------------------------------------------------

