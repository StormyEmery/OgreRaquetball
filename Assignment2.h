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

#include <SDL/SDL_mixer.h>

#include <Simulator.h>
#include "Plain.h"
#include "Ball.h"
#include "Sound.h"
#include "Paddle.h"

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

    virtual void buttonHit(OgreBites::Button * button);

    virtual Ball* getBall()=0;
    virtual void setBall(Ball *b)=0;
    virtual Simulator* getSimulator()=0;
    virtual int getScore()=0;

    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;

    Ogre::OverlaySystem*        mOverlaySystem;


    Ogre::SceneNode*            mBaller;

    // OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*  mTrayMgr;
    OgreBites::SdkCameraMan*    mCameraMan;         // Basic camera controller
    OgreBites::ParamsPanel*     mDetailsPanel;      // Sample details panel
    OgreBites::Button*          mMenu;
    bool                        mCursorWasVisible;  // Was cursor visible before dialog appeared?
    bool                        mShutDown;
    bool                        mPause;
    bool                        oneFrame;

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
    
    Simulator* simulator;
    clock_t start;
    clock_t sound_clock;
    double duration;
    double sound_duration;
    bool score_ok;
    bool sound_ok;
    bool background_music;
    bool main_menu;

    Real y;
    Real x;
    Real z;
    OgreBites::Button* menu1;
    OgreBites::Button* menu2;
    OgreBites::Button* menu3;
    OgreBites::Button* menu5;
    int mScore;





#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};

//---------------------------------------------------------------------------

#endif // #ifndef __Assignment2_h_

//---------------------------------------------------------------------------
