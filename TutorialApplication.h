/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
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

#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "Assignment2.h"

//---------------------------------------------------------------------------

class TutorialApplication : public Assignment2
{
public:
  TutorialApplication(void);
  virtual ~TutorialApplication(void);

protected:
  //Ball* ball;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btBroadphaseInterface* overlappingPairCache;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamicsWorld;

  btScalar desired_velocity;
  btVector3 current_velocity_direction;
  // int player_one_score;
  // int player_two_score;

  bool music_running;
  bool already_detected;

  Mix_Chunk* file;
  Mix_Music* music;
  


  virtual void createScene(void);
  virtual void createCamera();
  virtual void createViewports();
  virtual bool frameStarted(const Ogre::FrameEvent& fe);
  virtual Ball* getBall();
  virtual void setBall(Ball *b);
  virtual Simulator* getSimulator();
  virtual int getScoreOne();
  virtual int getScoreTwo();
  virtual void resetScore();
  virtual Paddle* getPaddleOne();
  virtual Paddle* getPaddleTwo();
  virtual void setPaddleOne(Paddle* p);
  virtual void setPaddleTwo(Paddle* p);
  virtual void setLight(Light* l);
  virtual Light* getLight();
  virtual void setGoal(Plain* g);

  void updateTransform();
  void initializeGUI();
  void InjectOISMouseButton(bool bButtonDown, OIS::MouseButtonID inButton);
  bool quit(const CEGUI::EventArgs &e);
  // bool mouseMoved( const OIS::MouseEvent &arg );
  

private:
	bool processUnbufferedInput(const Ogre::FrameEvent& fe);
  String getName(const btCollisionObject* collObj);
  void checkBoundary();
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
