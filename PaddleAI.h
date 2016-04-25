#ifndef __PADDLEAI_h_
#define __PADDLEAI_h_

#include "Ball.h"
#include "Paddle.h"

class PaddleAI {
private:
	Paddle *paddle;


public:

	PaddleAI(Paddle *p);
	~PaddleAI();

	Ogre::Vector3 Seek(Ogre::Vector3 target_pos);
	Ogre::Vector3 Pursuit(Ball* b);
	btVector3 convertVectorOgreToBullet(Ogre::Vector3 v);
	Ogre::Vector3 convertVectorBulletToOgre(btVector3 v);

};

#endif
