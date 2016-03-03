#include "Paddle.h"

Paddle::Paddle(Camera* camera, SceneManager* mSceneMgr, Vector3 normal, Vector3 up_vector, 
		   	   float x, float y, float offset, String name, String node_name, String path) 
			 	: Plain(mSceneMgr, normal, up_vector, x, y, offset, name, node_name, path){}

		
Paddle::~Paddle(void){}
