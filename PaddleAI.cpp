#include "PaddleAI.h"


PaddleAI::PaddleAI(Paddle *p) {
	paddle = p;
}

Ogre::Vector3 PaddleAI::Seek(Ogre::Vector3 target_pos) {
	Ogre::Vector3 v = target_pos - paddle->translationNode->getPosition();


	Ogre::Vector3 desired_velocity = v.normalisedCopy() * paddle->max_speed;

	// Ogre::Vector3 vd = desired_velocity;
 //    std::cout << "PaddleTwo Position: (" << vd.x << ", " << vd.y << ", " << vd.z << ")" << std::endl;

	Ogre::Vector3 actual_velocity = convertVectorBulletToOgre(paddle->paddleRigidBody->getLinearVelocity());

	return (desired_velocity - actual_velocity);
}

Ogre::Vector3 PaddleAI::Pursuit(Ball* b) {
	Ogre::Vector3 to_ball = b->ballNode->getPosition() - paddle->translationNode->getPosition();
	// Ogre::Vector3 v = to_ball;
 //    std::cout << "PaddleTwo Position: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	btScalar relative_heading = paddle->heading().dot(b->heading());

	// std::cout << "relative_heading: " << relative_heading << std::endl;

	btVector3 to_ball_bt = convertVectorOgreToBullet(to_ball);

	//if paddle is ahead of ball
	if((to_ball_bt.dot(paddle->heading()) > 0) && relative_heading < -0.95) 
		return Seek(b->ballNode->getPosition());

	btVector3 ball_velocity = b->ballRB->getLinearVelocity();
		
	btScalar look_ahead_time = to_ball_bt.length() / (paddle->max_speed + ball_velocity.length());

	std::cout << "look_ahead_time: " << ball_velocity.length() << std::endl;

	Ogre::Vector3 ball_velocity_ogre = convertVectorBulletToOgre(ball_velocity).normalisedCopy();

	return Seek(b->ballNode->getPosition() + (ball_velocity_ogre * look_ahead_time));
}

btVector3 PaddleAI::convertVectorOgreToBullet(Ogre::Vector3 v) {
	return btVector3(v.x, v.y, v.z);
}

Ogre::Vector3 PaddleAI::convertVectorBulletToOgre(btVector3 v) {
	return Ogre::Vector3(v.x(), v.y(), v.z());
}