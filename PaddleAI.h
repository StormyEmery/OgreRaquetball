#ifndef __PADDLEAI_h_
#define __PADDLEAI_h_

#include "Ball.h"
#include "Paddle.h"

class PaddleAI {
private:
	Paddle *paddle;
	int rS;
	float difficulty;
	int minDetectionDistance;
	int maxDetectionDistance;

public:

	PaddleAI(Paddle *p, int rS);
	~PaddleAI();

	Ogre::Vector3 Seek(Ball* b);
	void setDifficulty(int d);
	void setMinDetectionDistance(int v);
	void setMaxDetectionDistance(int v);
};


#endif
