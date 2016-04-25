#include "PaddleAI.h"


PaddleAI::PaddleAI(Paddle *p, int room_size) {
	paddle = p;
	rS = room_size;
	difficulty = 1.0;
	minDetectionDistance = -250;
	maxDetectionDistance = 2000;
}

Ogre::Vector3 PaddleAI::Seek(Ball *b) {
	float distance = b->ballNode->getPosition().z - paddle->translationNode->getPosition().z;

    if(distance > -minDetectionDistance && distance < maxDetectionDistance) {
        Vector3 newPos = b->ballNode->getPosition();
        newPos.z = -1000;
        Vector3 bounds = paddle->translationNode->getPosition();

        if( bounds.x > rS*1.2+97 || bounds.x < -rS*1.2-70 ||
            bounds.y > rS-150 || bounds.y < -(rS-150)){
                if(bounds.x  > rS*1.2+97)
                    paddle->translationNode->setPosition(rS*1.2+97, bounds.y, bounds.z);
                else if(bounds.x < -rS*1.2-70) 
                    paddle->translationNode->setPosition(-rS*1.2-70, bounds.y, bounds.z);
                else if(bounds.y > rS-150)
                    paddle->translationNode->setPosition(bounds.x, rS-150, bounds.z);
                else if(bounds.y < rS-150)                                
                    paddle->translationNode->setPosition(bounds.x, -(rS-150), bounds.z);
        }else{
            Vector3 to_ball = b->ballNode->getPosition() - paddle->translationNode->getPosition();
            to_ball.z = 0;
            to_ball = to_ball.normalisedCopy();
            paddle->translationNode->translate(to_ball*difficulty);
        }

        paddle->updateTransform();
    }
}

void PaddleAI::setDifficulty(int d) {
	difficulty = d;
}

void PaddleAI::setMinDetectionDistance(int v) {
	minDetectionDistance = v;
} 

void PaddleAI::setMaxDetectionDistance(int v) {
	maxDetectionDistance = v;
}
