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
    single_score_ok(true),
    multi_score_ok(true),
    sound_ok(true),
    single_duration(2),
    multi_duration(2),
    sound_duration(2),
    simulator(NULL),
    ball(NULL),
    background_music(true),
    main_menu(true),
    leftPressed(false),
    rightPressed(false),
    current_x(0),
    current_y(0),
    old_rel_x(0),
    old_rel_y(0),
    single_player(false),
    multi_player(false),
    moveUp(false),
    moveDown(false),
    moveLeft(false),
    moveRight(false),
    player_two_rotate(false),
    player_two_pan(false),
    rotate_x(0),
    pan_y(0),
    isServer(false),
    isClient(false),
    message_sent(""),
    message_received(""),
    stopped(false),
    firstClient(true),
    firstServer(true),

    //test("music/wall_collision.wav", 1),
    
    game_music("music/track1.wav", 0),
    menu_sound("music/menu_sound.wav", 1),
    button_sound("music/button_click.wav", 1),
    score_sound("music/score.wav", 1),
    wall_collision_sound("music/wall_collision.wav", 1),
    paddle_collision_sound("music/paddle_collision.wav", 1),

    mPause(false),
    gameOver(false),
    mMenu(0),
    mScore(0),
    oneFrame(false),
    mRenderer(0)

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

    mTrayMgr->hideCursor();
    // CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setInitialMousePosition(CEGUI::Vector2f(0, 0));  


    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("P1 Score");
    items.push_back("P2 Score");

    //Ogre::StringVector items2;
    // items2.push_back("P2 Score");

    gameOverLabel = mTrayMgr->createLabel(OgreBites::TL_CENTER, "Game Over", DisplayString("Game Over"), real(175.0));
    playerOneWins = mTrayMgr->createLabel(OgreBites::TL_CENTER, "Player One Wins", DisplayString("Player One Wins"), real(175.0));
    playerTwoWins = mTrayMgr->createLabel(OgreBites::TL_CENTER, "Player Two Wins", DisplayString("Player Two Wins"), real(175.0));
    separator = mTrayMgr->createSeparator(OgreBites::TL_CENTER, "Separator", real(175.0));

    menu1 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton1", "Continue", 150);
    menu5 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton5", "New Game", 150);
    menu2 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton2", "Exit", 150);
    menu3 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton3", "Toggle Music", 150);
    menu6 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton6", "Reset ball", 150);
    menu7 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton7", "Single Player", 150);
    menu8 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton8", "MultiPlayer", 150);
    menu9 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton9", "Back1", 150);
    menu10 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton10", "Main Menu", 150);
    menu11 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton11", "Start Server", 150);
    menu12 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton12", "Start Client", 150);
    menu13 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton13", "Back2", 150);


    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(player_one_score));
    mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(player_two_score));
    mDetailsPanel->show();


    mRoot->addFrameListener(this);

    gameOverLabel->hide();
    playerOneWins->hide();
    playerTwoWins->hide();
    mTrayMgr->removeWidgetFromTray(gameOverLabel);
    mTrayMgr->removeWidgetFromTray(playerOneWins);
    mTrayMgr->removeWidgetFromTray(playerTwoWins);

    separator->hide();
    mTrayMgr->removeWidgetFromTray(separator);

    //mTrayMgr->showCursor();
    menu2->hide();
    menu5->hide();
    menu1->hide();
    menu3->hide();
    menu6->hide();
    menu7->hide();
    menu8->hide();
    menu9->hide();
    menu10->hide();
    menu11->hide();
    menu12->hide();
    menu13->hide();
    mTrayMgr->removeWidgetFromTray(menu2);
    mTrayMgr->removeWidgetFromTray(menu5);
    mTrayMgr->removeWidgetFromTray(menu1);
    mTrayMgr->removeWidgetFromTray(menu3);
    mTrayMgr->removeWidgetFromTray(menu6);
    mTrayMgr->removeWidgetFromTray(menu7);
    mTrayMgr->removeWidgetFromTray(menu8);
    mTrayMgr->removeWidgetFromTray(menu9);
    mTrayMgr->removeWidgetFromTray(menu10);
    mTrayMgr->removeWidgetFromTray(menu11);
    mTrayMgr->removeWidgetFromTray(menu12);
    mTrayMgr->removeWidgetFromTray(menu13);
    mTrayMgr->setTrayPadding(12);
    mTrayMgr->hideBackdrop();


//=======================================================================
        //initializeGUI();
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");


    CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
 
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

    sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    CEGUI::Window *menuWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("main_menu.layout");
    sheet->addChild(menuWindow);
    menuWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.383,0), CEGUI::UDim(0.35,0)));
    menuWindow->setSize(CEGUI::USize(CEGUI::UDim(.2, 0), CEGUI::UDim(.33, 0)));
    menuWindow->setMouseCursor("TaharezLook/MouseArrow");
    
    for(int i=1; i<=13; i++){        
        sheet->addChild(menuWindow->getChild(i));
        sheet->getChild(i)->hide();
    }



    menuWindow->hide();
    sheet->getChild(1)->show();
    sheet->getChild(5)->show();
    sheet->getChild(1)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
    sheet->getChild(5)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));


    sheet->getChild(8)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::continue1, this));
    sheet->getChild(5)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::exit2, this));
    sheet->getChild(2)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::toggle_music3, this));
    sheet->getChild(1)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::new_game5, this));
    sheet->getChild(12)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::reset_ball6, this));
    sheet->getChild(4)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::single_player7, this));
    sheet->getChild(3)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::multiplayer8, this));
    sheet->getChild(10)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::back1_9, this));
    sheet->getChild(9)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::main_menu10, this));
    sheet->getChild(6)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::start_server11, this));
    sheet->getChild(7)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::start_client12, this));
    sheet->getChild(11)->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Assignment2::back2_13, this));
    sheet->getChild(13)->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&Assignment2::Handle_TextSubmitted,this));


//===========================================================================



}


CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:

        return CEGUI::LeftButton;
        break;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;
 
    default:
        return CEGUI::LeftButton;
        break;
    }
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

    createFrameListener();

    return true;
};
//---------------------------------------------------------------------------
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    SceneNode* temp = mSceneMgr->getSceneNode("translateTwo");
    Vector3 bounds = temp->getPosition();

    Vector3 paddle_one_coords = mSceneMgr->getSceneNode("translate")->getPosition();
    Quaternion paddle_one_quat = mSceneMgr->getSceneNode("translate")->getOrientation();

    Vector3 ball_coords = ball->ballNode->getPosition();
    Quaternion ball_quat = ball->ballNode->getOrientation();

    //Construct messages
    if(rel_mouse_state_x != 0 || rel_mouse_state_y != 0) {
        message_sent = "";
        message_sent += to_string(rel_mouse_state_x);
        message_sent += " ";
        message_sent += to_string(-rel_mouse_state_y);
        message_sent += " ";
        rel_mouse_state_x = 0;
        rel_mouse_state_y = 0;
    }
    else {
        message_sent = "";
        message_sent += to_string(0);
        message_sent += " ";
        message_sent += to_string(0);
        message_sent += " ";
    }

    message_sent += to_string(current_x);
    message_sent += " ";
    message_sent += to_string(current_y);
    message_sent += " ";

    if(leftPressed){
        message_sent += "1";
        message_sent += " ";
    }
    else{
        message_sent += "0";
        message_sent += " ";
    }
    if(rightPressed){
        message_sent += "1";
        message_sent += " ";
    }
    else{
        message_sent += "0";
        message_sent += " ";
    }

    if(isServer) {
        message_sent += to_string(ball_coords.x);
        message_sent += " ";
        message_sent += to_string(ball_coords.y);
        message_sent += " ";
        message_sent += to_string(ball_coords.z);
        message_sent += " ";
        message_sent += to_string(ball_quat.w);
        message_sent += " ";
        message_sent += to_string(ball_quat.x);
        message_sent += " ";
        message_sent += to_string(ball_quat.y);
        message_sent += " ";
        message_sent += to_string(ball_quat.z);
        message_sent += " ";
        message_sent += to_string(player_one_score);
        message_sent += " ";
        message_sent += to_string(player_two_score);
        message_sent += " ";

        if(mPause) 
            message_sent += to_string(1);
        else
            message_sent += to_string(0);

        message_sent += " ";

        if(stopped)
            message_sent += to_string(1);
        else
            message_sent += to_string(0);

        message_sent += " ";
    }


    if (isClient && !stopped) {
        netManager.messageServer(PROTOCOL_UDP, message_sent.c_str(), message_sent.length());
    }

    if(isServer && !stopped){
        netManager.messageClients(PROTOCOL_UDP, message_sent.c_str(), message_sent.length());
    }

    /*DO ALL POLLING HERE!! 
        serverpoll
            get client paddle data

        clieentpoll
            get ball data
            get server paddle data
    */

    if(!stopped && multi_player) {    

        if(isClient){
            if(netManager.pollForActivity(1)){
                istringstream stream(netManager.udpServerData[0].output);
                string sub;
                stream >> sub;
                float move_x = atof(sub.c_str());
                stream >> sub;
                float move_y = atof(sub.c_str());
                stream >> sub;
                float current_x = atof(sub.c_str());
                stream >> sub;
                float current_y = atof(sub.c_str());
                stream >> sub;
                float left_pressed = atof(sub.c_str());
                stream >> sub;
                float right_pressed = atof(sub.c_str());
                stream >> sub;

                /*Start of Stormy's bullshit*/
                float ball_pos_x = atof(sub.c_str());
                stream >> sub;
                float ball_pos_y = atof(sub.c_str());
                stream >> sub;
                float ball_pos_z = -atof(sub.c_str());
                stream >> sub;
                float ball_quat_w = atof(sub.c_str());
                stream >> sub;
                float ball_quat_x = atof(sub.c_str());
                stream >> sub;
                float ball_quat_y = atof(sub.c_str());
                stream >> sub;
                float ball_quat_z = atof(sub.c_str());

                stream >> sub;
                player_one_score = atoi(sub.c_str());
                stream >> sub;
                player_two_score = atoi(sub.c_str());
                stream >> sub;
                int isPaused = atoi(sub.c_str());
                stream >> sub;
                int isStopped = atoi(sub.c_str());

                if(isPaused)
                    mPause = true;
                else 
                    mPause = false;

                if(isStopped) 
                    stopped = true;
                else
                    stopped = false;


                ball->ballNode->setPosition(ball_pos_x, ball_pos_y, ball_pos_z);
                ball->ballNode->setOrientation(ball_quat_w, ball_quat_x, ball_quat_y, ball_quat_z);
                ball->updateTransform();

                temp->translate(move_x, move_y, 0);
                if(left_pressed){
                    paddleTwo->paddleNode->setOrientation(Quaternion(Degree(-current_x/2), Vector3(0,1,0)));
                }
                if(right_pressed){
                    paddleTwo->paddleNode->setOrientation(Quaternion(Degree(-current_y/2), Vector3(1,0,0)));
                }
            }
        }

        if(isServer){
            if(netManager.pollForActivity(1)){
                istringstream stream(netManager.udpClientData[0]->output);
                string sub;
                stream >> sub;
                float move_x = atof(sub.c_str());
                stream >> sub;
                float move_y = atof(sub.c_str());
                stream >> sub;
                float current_x = atof(sub.c_str());
                stream >> sub;
                float current_y = atof(sub.c_str());
                stream >> sub;
                float left_pressed = atof(sub.c_str());
                stream >> sub;
                float right_pressed = atof(sub.c_str());
                stream >> sub;
                int isPaused = atoi(sub.c_str());

                temp->translate(move_x, move_y, 0);
                if(left_pressed){
                    paddleTwo->paddleNode->setOrientation(Quaternion(Degree(-current_x/2), Vector3(0,1,0)));
                }
                if(right_pressed){
                    paddleTwo->paddleNode->setOrientation(Quaternion(Degree(-current_y/2), Vector3(1,0,0)));
                }
            }
        }


       if( bounds.x > 597 || bounds.x < -570 ||
        bounds.y > 398 || bounds.y < -398){
            if(bounds.x  > 597)
                temp->setPosition(597, bounds.y, bounds.z);
            else if(bounds.x < -570) 
                temp->setPosition(-570, bounds.y, bounds.z);
            else if(bounds.y > 398)
                temp->setPosition(bounds.x, 398, bounds.z);
            else if(bounds.y < 398)                                
                temp->setPosition(bounds.x, -398, bounds.z);
        }

        paddleTwo->updateTransform();
    }


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
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(player_one_score));            
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(player_two_score));            
        }
    }
    
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);

    if (arg.key == OIS::KC_ESCAPE && !main_menu){
        if(!mPause && !gameOver) {
            game_music.pause();
            menu_sound.play(0);
            //mTrayMgr->showCursor();
            CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
            mPause=true;
            sheet->getChild(12)->show();
            sheet->getChild(8)->show();
            sheet->getChild(2)->show();
            sheet->getChild(9)->show();
            sheet->getChild(5)->show();
            sheet->getChild(12)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
            sheet->getChild(8)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
            sheet->getChild(2)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(3*.051),0)));
            sheet->getChild(9)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(4*.051),0)));
            sheet->getChild(5)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(5*.051),0)));
        }   
        else if(!gameOver){
            menu_sound.play(0);
            if(background_music)
                game_music.resume();
            mTrayMgr->hideCursor();
            CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
            mPause=false;
            sheet->getChild(12)->hide();
            sheet->getChild(8)->hide();
            sheet->getChild(2)->hide();
            sheet->getChild(9)->hide();
            sheet->getChild(5)->hide();
           
        }
    }
    if (arg.key == OIS::KC_F12){
        mShutDown = true;
    }

    if (arg.key == OIS::KC_W) { moveUp = true; }
    if (arg.key == OIS::KC_A) { moveLeft = true; }
    if (arg.key == OIS::KC_S) { moveDown = true; }
    if (arg.key == OIS::KC_D) { moveRight = true; }
    if (arg.key == OIS::KC_LSHIFT) { player_two_rotate = true; }
    if (arg.key == OIS::KC_SPACE) { player_two_pan = true; }

   
    paddleOne->updateTransform();

    if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    if(mPause) return false;
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::keyReleased(const OIS::KeyEvent &arg)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    if (arg.key == OIS::KC_W) { moveUp = false; }
    if (arg.key == OIS::KC_A) { moveLeft = false; }
    if (arg.key == OIS::KC_S) { moveDown = false; }
    if (arg.key == OIS::KC_D) { moveRight = false; }
    if (arg.key == OIS::KC_LSHIFT) { player_two_rotate = false; }
    if (arg.key == OIS::KC_SPACE) { player_two_pan = false; }

    if(mPause) return false;
    mCameraMan->injectKeyUp(arg);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mouseMoved(const OIS::MouseEvent &arg)
{

    CEGUI::System &sys = CEGUI::System::getSingleton();

    sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);  
  

    if (mTrayMgr->injectMouseMove(arg)) return true;
    SceneNode* temp = mSceneMgr->getSceneNode("translate");
    Vector3 bounds = temp->getPosition();


    if(!mPause && !main_menu ) {       
        if( bounds.x > 597 || bounds.x < -570 ||
        bounds.y > 398 || bounds.y < -398){
            if(bounds.x  > 597)
                temp->setPosition(597, bounds.y, bounds.z);
            else if(bounds.x < -570) 
                temp->setPosition(-570, bounds.y, bounds.z);
            else if(bounds.y > 398)
                temp->setPosition(bounds.x, 398, bounds.z);
            else if(bounds.y < 398)                                
                temp->setPosition(bounds.x, -398, bounds.z);

        }else{
            temp->translate(arg.state.X.rel, -arg.state.Y.rel, 0);
        }

        rel_mouse_state_x = arg.state.X.rel;
        rel_mouse_state_y = arg.state.Y.rel;
        current_x = current_x + rel_mouse_state_x;
        current_y = current_y + rel_mouse_state_y;


        if(leftPressed){
            paddleOne->paddleNode->setOrientation(Quaternion(Degree(current_x/2), Vector3(0,1,0)));
        }
        if(rightPressed){
            paddleOne->paddleNode->setOrientation(Quaternion(Degree(current_y/2), Vector3(1,0,0)));
        }
        paddleOne->updateTransform();
    }
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertButton(id));
    std::cout << sheet->getChild(1)->isMouseContainedInArea() << std::endl; 

    if(id == 0){ leftPressed = true; }
    if(id == 1){ rightPressed = true; }

    if (mTrayMgr->injectMouseDown(arg, id)) {
        return true;
    }
    if(!mPause)
        mCameraMan->injectMouseDown(arg, id);
    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{

    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertButton(id));
    std::cout << sheet->getChild(1)->isMouseContainedInArea() << std::endl; 
    if(id == 0){ leftPressed = false; }
    if(id == 1){ rightPressed = false; }

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

void Assignment2::render_single_paddle(){
    mSceneMgr->getSceneNode("translate")->setPosition(0,0,1000);

    if(mSceneMgr->getSceneNode("translate")->numAttachedObjects() == 0){
        mSceneMgr->getSceneNode("translate")->attachObject(mCamera);
        mCameraMan->setTarget(mSceneMgr->getSceneNode("translate"));
        mCameraMan->setYawPitchDist(Degree(0), Degree(45), 1000);
    }


    mSceneMgr->getSceneNode("translateTwo")->setPosition(10000,0,-1000);


    mSceneMgr->getSceneNode("translateThree")->setPosition(0,0,-1490);


    paddleOne->updateTransform();
    paddleTwo->updateTransform();
    goal->updateTransform();
}

void Assignment2::render_multi_paddle(){
    mSceneMgr->getSceneNode("translate")->setPosition(0,0,1000);

    if(mSceneMgr->getSceneNode("translate")->numAttachedObjects() == 0){
        mSceneMgr->getSceneNode("translate")->attachObject(mCamera);
        mCameraMan->setTarget(mSceneMgr->getSceneNode("translate"));
        mCameraMan->setYawPitchDist(Degree(0), Degree(45), 1000);
    }


   mSceneMgr->getSceneNode("translateTwo")->setPosition(0,0,-1000);

   mSceneMgr->getSceneNode("translateThree")->setPosition(10000,0,0);


   paddleOne->updateTransform();
   paddleTwo->updateTransform();
   goal->updateTransform();

}

bool Assignment2::continue1 (const CEGUI::EventArgs &e){

        button_sound.play(0);
        mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(++mScore));

        sheet->getChild(12)->hide();
        sheet->getChild(8)->hide();
        sheet->getChild(5)->hide();
        sheet->getChild(2)->hide();
        sheet->getChild(9)->hide();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        mPause = false;
    return true;
}



bool Assignment2::exit2 (const CEGUI::EventArgs &e){
    button_sound.play(0);
    mShutDown = true;
    return true;
}

bool Assignment2::toggle_music3 (const CEGUI::EventArgs &e){
    button_sound.play(0);
    background_music = !background_music;
    return true;
}
bool Assignment2::new_game5 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        sheet->getChild(5)->hide();
        sheet->getChild(1)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        sheet->getChild(4)->show();
        sheet->getChild(3)->show();
        sheet->getChild(10)->show();
        sheet->getChild(4)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
        sheet->getChild(3)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
        sheet->getChild(10)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(3*.051),0)));
        separator->hide();        
        mTrayMgr->removeWidgetFromTray(separator);
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
    return true;
}


bool Assignment2::reset_ball6 (const CEGUI::EventArgs &e){
        button_sound.play(0);

        sheet->getChild(12)->hide();
        sheet->getChild(8)->hide();
        sheet->getChild(5)->hide();
        sheet->getChild(2)->hide();
        sheet->getChild(9)->hide();

        mTrayMgr->hideCursor();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        mPause = false;
        if(background_music)
                game_music.resume();
        ball->reset(mSceneMgr, ball, simulator);

    return true;
}

bool Assignment2::single_player7 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        single_player = true;
        multi_player = false;
        main_menu = false;
        mPause = false;
        gameOver = false;
        sheet->getChild(4)->hide();
        sheet->getChild(3)->hide();
        sheet->getChild(10)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        separator->hide();
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
        mTrayMgr->removeWidgetFromTray(separator);
        mTrayMgr->hideCursor();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        resetScore();
        render_single_paddle();
    return true;
}
bool Assignment2::multiplayer8 (const CEGUI::EventArgs &e){

        button_sound.play(0);
        sheet->getChild(4)->hide();
        sheet->getChild(3)->hide();
        sheet->getChild(10)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        sheet->getChild(6)->show();
        sheet->getChild(7)->show();
        sheet->getChild(11)->show();
        sheet->getChild(13)->show();
        sheet->getChild(13)->activate();
        sheet->getChild(6)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
        sheet->getChild(7)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
        sheet->getChild(11)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(3*.051),0)));
        sheet->getChild(13)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25,0), CEGUI::UDim(0.35+(2*.051),0)));

    return true;
}
bool Assignment2::back1_9 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        sheet->getChild(3)->hide();
        sheet->getChild(4)->hide();
        sheet->getChild(10)->hide();
        sheet->getChild(1)->show();
        sheet->getChild(5)->show();
        sheet->getChild(1)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
        sheet->getChild(5)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
    return true;
}
bool Assignment2::main_menu10 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        single_player = false;
        multi_player = false;
        main_menu = true;
        mPause = false;
        gameOver = false;
        sheet->getChild(8)->hide();
        sheet->getChild(2)->hide();
        sheet->getChild(4)->hide();
        sheet->getChild(3)->hide();
        sheet->getChild(10)->hide();
        sheet->getChild(12)->hide();
        sheet->getChild(9)->hide();
        sheet->getChild(1)->show();
        sheet->getChild(5)->show();
        sheet->getChild(1)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
        sheet->getChild(5)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
        ball->reset(mSceneMgr, ball, simulator);
    return true;
}
bool Assignment2::start_server11 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        single_player = false;
        multi_player = true;
        main_menu = false;
        mPause = false;
        gameOver = false;
        sheet->getChild(6)->hide();
        sheet->getChild(7)->hide();
        sheet->getChild(11)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        separator->hide();
        sheet->getChild(13)->hide();
        sheet->getChild(13)->deactivate();
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
        mTrayMgr->removeWidgetFromTray(separator);
        mTrayMgr->hideCursor();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        resetScore();
        render_multi_paddle();
        if(firstServer) {
            netManager.initNetManager();
            netManager.addNetworkInfo(PROTOCOL_UDP, NULL, 51215);
            firstServer = false;
        }

        netManager.startServer();
        netManager.acceptConnections();
        isServer = true;
        isClient = false;
        stopped = false;
    return true;
}
bool Assignment2::start_client12 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        single_player = false;
        multi_player = true;
        main_menu = false;
        mPause = false;
        gameOver = false;
        ball->reset(mSceneMgr, ball, simulator);
        netManager.close();
        stopped = true;
        sheet->getChild(6)->hide();
        sheet->getChild(7)->hide();
        sheet->getChild(11)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        sheet->getChild(13)->hide();
        sheet->getChild(13)->deactivate();
        mTrayMgr->hideCursor();
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
        render_multi_paddle();
        netManager.initNetManager();
        netManager.addNetworkInfo(PROTOCOL_UDP, ipEntered.c_str(), 51215);
        netManager.startClient();
        isServer = false;
        isClient = true;
    return true;
}
bool Assignment2::back2_13 (const CEGUI::EventArgs &e){
        button_sound.play(0);
        sheet->getChild(6)->hide();
        sheet->getChild(7)->hide();
        sheet->getChild(11)->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        sheet->getChild(13)->hide();
        sheet->getChild(13)->deactivate();
        sheet->getChild(4)->show();
        sheet->getChild(3)->show();
        sheet->getChild(10)->show();
        sheet->getChild(4)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(1*.051),0)));
        sheet->getChild(3)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(2*.051),0)));
        sheet->getChild(10)->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425,0), CEGUI::UDim(0.35+(3*.051),0)));
    return true;
}

bool Assignment2::Handle_TextSubmitted(const CEGUI::EventArgs &e)
{
    // The following line of code is not really needed in this particular example, but is good to show.  The EventArgs by itself 
     // only has limited uses. You will find it more useful to cast this to another type of Event.  In this case WindowEventArgs
     // could be much more useful as we are dealing with a CEGUI::Window.  Notably, this will allow you access to the .window
     // member of the argument, which will have a pointer to the window which called the event.  You can imagine that would be
     // useful!
    const CEGUI::WindowEventArgs* args = static_cast<const CEGUI::WindowEventArgs*>(&e);
 
    // Now we need to get the text that is in the edit box right now.
    CEGUI::String Msg = sheet->getChild(13)->getText();
 
    // Since we have that string, lets send it to the TextParser which will handle it from here
    (this)->ParseText(Msg);
 
    // Now that we've finished with the text, we need to ensure that we clear out the EditBox.  This is what we would expect
      // To happen after we press enter
    sheet->getChild(13)->setText("");
 
    return true;
}
void Assignment2::ParseText(CEGUI::String inMsg)
{
      ipEntered = inMsg.c_str();
}



//---------------------------------------------------------------------------

