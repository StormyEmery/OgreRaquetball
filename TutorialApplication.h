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
  Simulator* simulator;
  Ball* ball;
  btDefaultCollisionConfiguration* collisionConfiguration;
  btCollisionDispatcher* dispatcher;
  btBroadphaseInterface* overlappingPairCache;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamicsWorld;



  virtual void createScene(void);
  virtual void createCamera();
  virtual void createViewports();
  virtual bool frameStarted(const Ogre::FrameEvent& fe);

private:
	bool processUnbufferedInput(const Ogre::FrameEvent& fe);
  void checkBoundary();
};

//---------------------------------------------------------------------------

#endif // #ifndef __TutorialApplication_h_

//---------------------------------------------------------------------------
