#ifndef __PADDLE_h_
#define __PADDLE_h_

#include "Plain.h"

class Paddle : public Plain{
public:
	Paddle(Camera* camera, SceneManager* mSceneMgr, Vector3 normal, Vector3 up_vector, 
		   float x, float y, float offset, String name, String node_name, String path);

	~Paddle(void);

protected:

};

#endif