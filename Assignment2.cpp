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
    menu9 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton9", "Back", 150);
    menu10 = mTrayMgr->createButton(OgreBites::TL_CENTER, "MyButton10", "Main Menu", 150);


    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(getScoreOne()));
    mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(getScoreTwo()));
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

    mTrayMgr->showCursor();
    menu1->hide();
    menu3->hide();
    menu6->hide();
    menu7->hide();
    menu8->hide();
    menu9->hide();
    menu10->hide();
    mTrayMgr->removeWidgetFromTray(menu1);
    mTrayMgr->removeWidgetFromTray(menu3);
    mTrayMgr->removeWidgetFromTray(menu6);
    mTrayMgr->removeWidgetFromTray(menu7);
    mTrayMgr->removeWidgetFromTray(menu8);
    mTrayMgr->removeWidgetFromTray(menu9);
    mTrayMgr->removeWidgetFromTray(menu10);
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

    createFrameListener();

    return true;
};
//---------------------------------------------------------------------------
bool Assignment2::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

    SceneNode* temp = mSceneMgr->getSceneNode("translateTwo");
    Vector3 bounds = temp->getPosition();


    if(!mPause && !main_menu && multi_player) {        
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
            if(moveUp){ temp->translate(0, 0.5, 0); }
            if(moveDown){ temp->translate(0, -0.5, 0); }
            if(moveLeft){ temp->translate(-0.5, 0, 0); }
            if(moveRight){ temp->translate(0.5, 0, 0); }
        }


        if(player_two_rotate && moveLeft){
            paddleTwo->paddleNode->setOrientation(Quaternion(Degree(rotate_x+=0.1), Vector3(0,1,0)));
        }
        if(player_two_rotate && moveRight){
            paddleTwo->paddleNode->setOrientation(Quaternion(Degree(rotate_x-=0.1), Vector3(0,1,0)));
        }
        if(player_two_pan && moveUp){
            paddleTwo->paddleNode->setOrientation(Quaternion(Degree(pan_y-=0.1), Vector3(1,0,0)));
        }
        if(player_two_pan && moveDown){
            paddleTwo->paddleNode->setOrientation(Quaternion(Degree(pan_y+=0.1), Vector3(1,0,0)));
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
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(getScoreOne()));            
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(getScoreTwo()));            
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool Assignment2::keyPressed( const OIS::KeyEvent &arg )
{
    if (arg.key == OIS::KC_ESCAPE && !main_menu){
        if(!mPause && !gameOver) {
            game_music.pause();
            menu_sound.play(0);
            mTrayMgr->showCursor();
            mPause=true;
            menu6->show();
            menu1->show();
            menu3->show();
            menu2->show();
            menu10->show();
            mTrayMgr->moveWidgetToTray(menu2, OgreBites::TL_CENTER, 0);
            mTrayMgr->moveWidgetToTray(menu10, OgreBites::TL_CENTER, 0);
            mTrayMgr->moveWidgetToTray(menu3, OgreBites::TL_CENTER, 0);
            mTrayMgr->moveWidgetToTray(menu1, OgreBites::TL_CENTER, 0);
            mTrayMgr->moveWidgetToTray(menu6, OgreBites::TL_CENTER, 0);
        }   
        else if(!gameOver){
            menu_sound.play(0);
            if(background_music)
                game_music.resume();
            mTrayMgr->hideCursor();
            mPause=false;
            mTrayMgr->removeWidgetFromTray(menu6);
            mTrayMgr->removeWidgetFromTray(menu1);
            mTrayMgr->removeWidgetFromTray(menu2);
            mTrayMgr->removeWidgetFromTray(menu3);
            mTrayMgr->removeWidgetFromTray(menu10);
            menu6->hide();
            menu1->hide();
            menu2->hide();
            menu3->hide();
            menu10->hide();
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


        current_x = current_x + arg.state.X.rel;
        current_y = current_y + arg.state.Y.rel;

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

void Assignment2::buttonHit(OgreBites::Button * button) {
    if(button->getName() == "MyButton1") {
        button_sound.play(0);
        mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(++mScore));
        mTrayMgr->removeWidgetFromTray(menu6);
        mTrayMgr->removeWidgetFromTray(menu1);
        mTrayMgr->removeWidgetFromTray(menu2);
        mTrayMgr->removeWidgetFromTray(menu3);
        mTrayMgr->removeWidgetFromTray(menu10);
        menu6->hide();
        menu1->hide();
        menu2->hide();
        menu3->hide();
        menu10->hide();
        mTrayMgr->hideCursor();
        mPause = false;
    }
    else if(button->getName()=="MyButton2"){
        button_sound.play(0);
        mShutDown = true;
    }
    else if(button->getName()=="MyButton3"){
        button_sound.play(0);
        background_music = !background_music;
    }
    else if(button->getName()=="MyButton5"){
        button_sound.play(0);
        menu2->hide();
        menu5->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        mTrayMgr->removeWidgetFromTray(menu2);
        mTrayMgr->removeWidgetFromTray(menu5);
        menu9->show();
        menu8->show();
        menu7->show();
        mTrayMgr->moveWidgetToTray(menu9, OgreBites::TL_CENTER, 0);
        mTrayMgr->moveWidgetToTray(menu8, OgreBites::TL_CENTER, 0);
        mTrayMgr->moveWidgetToTray(menu7, OgreBites::TL_CENTER, 0);
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
    }
    else if(button->getName() == "MyButton6") {
        button_sound.play(0);
        mTrayMgr->removeWidgetFromTray(menu6);
        mTrayMgr->removeWidgetFromTray(menu1);
        mTrayMgr->removeWidgetFromTray(menu2);
        mTrayMgr->removeWidgetFromTray(menu3);
        mTrayMgr->removeWidgetFromTray(menu10);
        menu6->hide();
        menu1->hide();
        menu2->hide();
        menu3->hide();
        menu10->hide();
        mTrayMgr->hideCursor();
        mPause = false;
        if(background_music)
                game_music.resume();
        ball->reset(mSceneMgr, ball, simulator);
    }
    else if(button->getName()=="MyButton7"){
        button_sound.play(0);
        single_player = true;
        multi_player = false;
        main_menu = false;
        mPause = false;
        gameOver = false;
        menu7->hide();
        menu8->hide();
        menu9->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        separator->hide();
        mTrayMgr->removeWidgetFromTray(menu7);
        mTrayMgr->removeWidgetFromTray(menu8);
        mTrayMgr->removeWidgetFromTray(menu9);
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
        mTrayMgr->removeWidgetFromTray(separator);
        mTrayMgr->hideCursor();
        resetScore();
        render_single_paddle();
    }
    else if(button->getName()=="MyButton8"){
        button_sound.play(0);
        single_player = false;
        multi_player = true;
        main_menu = false;
        mPause = false;
        gameOver = false;
        menu7->hide();
        menu8->hide();
        menu9->hide();
        gameOverLabel->hide();
        playerOneWins->hide();
        playerTwoWins->hide();
        separator->hide();
        mTrayMgr->removeWidgetFromTray(menu7);
        mTrayMgr->removeWidgetFromTray(menu8);
        mTrayMgr->removeWidgetFromTray(menu9);
        mTrayMgr->removeWidgetFromTray(gameOverLabel);
        mTrayMgr->removeWidgetFromTray(playerOneWins);
        mTrayMgr->removeWidgetFromTray(playerTwoWins);
        mTrayMgr->removeWidgetFromTray(separator);
        mTrayMgr->hideCursor();
        resetScore();
        render_multi_paddle();
    }
    else if(button->getName()=="MyButton9"){
        button_sound.play(0);
        menu7->hide();
        menu8->hide();
        menu9->hide();
        mTrayMgr->removeWidgetFromTray(menu7);
        mTrayMgr->removeWidgetFromTray(menu8);
        mTrayMgr->removeWidgetFromTray(menu9);
        menu2->show();
        menu5->show();
        mTrayMgr->moveWidgetToTray(menu2, OgreBites::TL_CENTER, 0);
        mTrayMgr->moveWidgetToTray(menu5, OgreBites::TL_CENTER, 0);
    }
    else if(button->getName()=="MyButton10"){
        button_sound.play(0);
        single_player = false;
        multi_player = false;
        main_menu = true;
        mPause = false;
        gameOver = false;
        menu1->hide();
        menu3->hide();
        menu6->hide();
        menu7->hide();
        menu8->hide();
        menu9->hide();
        menu10->hide();
        mTrayMgr->removeWidgetFromTray(menu1);
        mTrayMgr->removeWidgetFromTray(menu3);
        mTrayMgr->removeWidgetFromTray(menu6);
        mTrayMgr->removeWidgetFromTray(menu7);
        mTrayMgr->removeWidgetFromTray(menu8);
        mTrayMgr->removeWidgetFromTray(menu9);
        mTrayMgr->removeWidgetFromTray(menu10);
        menu2->show();
        menu5->show();
        mTrayMgr->moveWidgetToTray(menu2, OgreBites::TL_CENTER, 0);
        mTrayMgr->moveWidgetToTray(menu5, OgreBites::TL_CENTER, 0);
        ball->reset(mSceneMgr, ball, simulator);
    }
}



//---------------------------------------------------------------------------

