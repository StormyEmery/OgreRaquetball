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

};

#endif
