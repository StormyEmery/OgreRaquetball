#ifndef __Assignment2_h_
#define __Assignment2_h_

#include <OgreMotionState.h>
#include <OgreCamera.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreManualObject.h>
#include <OgreRenderOperation.h>
#include <OgreAxisAlignedBox.h>
#include <OgreParticleSystem.h>
#include <OgreMotionState.h>
#include <OgreNode.h>
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <SDL/SDL_mixer.h>
#include "NetManager.h"

#include <Simulator.h>
#include "Plain.h"
#include "Ball.h"
#include "Sound.h"
#include "Paddle.h"
#include "PaddleAI.h"
#include "Powerup.h"

#include <iostream>
#include <time.h>
#include <ctime>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#  include <OGRE/SdkTrays.h>
#  include <OGRE/SdkCameraMan.h>
#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

#  include <SdkTrays.h>
#  include <SdkCameraMan.h>
#endif

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

class Assignment2 : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    Assignment2(void);
    virtual ~Assignment2(void);

    virtual void go(void);

    protected:
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);


    virtual Ball* getBall()=0;
    virtual void setBall(Ball* b)=0;
    virtual Simulator* getSimulator()=0;
    virtual int getScoreOne()=0;
    virtual int getScoreTwo()=0;
    virtual void resetScore()=0;
    virtual void setPaddleOne(Paddle* p)=0;
    virtual void setPaddleTwo(Paddle* p)=0;
    virtual void setGoal(Plain* g)=0;
    virtual void render_single_paddle();
    virtual void render_multi_paddle();

    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;
    CEGUI::OgreRenderer*        mRenderer;

    Ogre::OverlaySystem*        mOverlaySystem;


    Ogre::SceneNode*            mBaller;

    NetManager                  netManager;

    // OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*  mTrayMgr;
    OgreBites::SdkCameraMan*    mCameraMan;         // Basic camera controller
    OgreBites::ParamsPanel*     mDetailsPanel;      // Sample details panel
    OgreBites::ParamsPanel*     mDetailsPanel2;      // Sample details panel
    OgreBites::Button*          mMenu;
    bool                        mCursorWasVisible;  // Was cursor visible before dialog appeared?
    bool                        mShutDown;
    bool                        mPause;
    bool                        gameOver;
    bool                        oneFrame;
    bool                        leftPressed;
    bool                        rightPressed;
    bool                        moveLeft;
    bool                        moveRight;
    bool                        moveUp;
    bool                        moveDown;
    bool                        player_two_rotate;
    bool                        player_two_pan;
    bool                        isServer;
    bool                        isClient;
    Real                        rotate_x;
    Real                        pan_y;
    Real                        x_pos;
    Real                        y_pos;
    bool                        single_player;
    bool                        multi_player;
    Real                        rel_mouse_state_x;
    Real                        rel_mouse_state_y;
    Real                        old_rel_x;
    Real                        old_rel_y;
    Real                        current_x;
    Real                        current_y;
    String                      message_sent;
    String                      message_received;
    int                         player_one_score;
    int                         player_two_score;
    bool                        stopped;
    bool                        new_multiplayer_game;
    bool                        first;
    int                         rS;
    float                       const_velocity;



    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;
    Sound game_music;
    Sound menu_sound;
    Sound button_sound;
    Sound score_sound;
    Sound wall_collision_sound;
    Sound paddle_collision_sound;
    Sound test;

    Ball* ball;
    Paddle* paddleOne;
    Paddle* paddleTwo;
    Plain* goal;
    PaddleAI* paddleAI;
    Powerup* power_up;
    //btTransform transform;
    //OgreMotionState* paddleMotionState;
    //btRigidBody* paddleBody;
    
    Simulator* simulator;
    clock_t start;
    clock_t sound_clock;
    clock_t sound_clock2;
    double single_duration;
    double multi_duration;
    double sound_duration;
    bool single_score_ok;
    bool multi_score_ok;
    bool sound_ok;
    bool background_music;
    bool main_menu;

    OgreBites::Button* menu1;
    OgreBites::Button* menu2;
    OgreBites::Button* menu3;
    OgreBites::Button* menu5;
    OgreBites::Button* menu6;
    OgreBites::Button* menu7;
    OgreBites::Button* menu8;
    OgreBites::Button* menu9;
    OgreBites::Button* menu10;
    OgreBites::Button* menu11;
    OgreBites::Button* menu12;
    OgreBites::Button* menu13;
    OgreBites::Label* gameOverLabel;
    OgreBites::Label* playerOneWins;
    OgreBites::Label* playerTwoWins;
    OgreBites::Separator* separator;
    int mScore;
    CEGUI::Window *sheet;
    virtual bool continue1 (const CEGUI::EventArgs &e);
    virtual bool exit2 (const CEGUI::EventArgs &e);
    virtual bool toggle_music3 (const CEGUI::EventArgs &e);
    virtual bool new_game5 (const CEGUI::EventArgs &e);
    virtual bool reset_ball6 (const CEGUI::EventArgs &e);
    virtual bool single_player7 (const CEGUI::EventArgs &e);
    virtual bool multiplayer8 (const CEGUI::EventArgs &e);
    virtual bool back1_9 (const CEGUI::EventArgs &e);
    virtual bool main_menu10 (const CEGUI::EventArgs &e);
    virtual bool start_server11 (const CEGUI::EventArgs &e);
    virtual bool start_client12 (const CEGUI::EventArgs &e);
    virtual bool back2_13 (const CEGUI::EventArgs &e);
    bool Handle_TextSubmitted(const CEGUI::EventArgs &e);      // Handle when we press Enter after typing  
    void ParseText(CEGUI::String inMsg);
    void startNewMultiplayerGame();
    void goToMainMenu(); 
    String              ipEntered;

    





#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};

//---------------------------------------------------------------------------

#endif // #ifndef __Assignment2_h_

//---------------------------------------------------------------------------
