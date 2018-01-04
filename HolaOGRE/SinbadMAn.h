#ifndef	_H_SinbadMan_H_
#define _H_SinbadMan_H_

#include "EntityMan.h"
#include "KnotMan.h"

class SinbadMan : public OgreBites::InputListener, public EntityMan
{
public:
	SinbadMan(Ogre::SceneNode * sceneNode);
	virtual ~SinbadMan();

	bool mousePicking(const OgreBites::MouseButtonEvent& event) {
		//Cambiar entre que se mueva o no.
		return true; 
	}
	void frameRendered(const Ogre::FrameEvent& event){
		animState_Run->addTime(event.timeSinceLastFrame);
		animationState_->addTime(event.timeSinceLastFrame);
	}
private:
	OgreBites::InputListener * list = new OgreBites::InputListener();
	Ogre::SceneNode* sceneNode_;
	Ogre::Entity* entity_;
	Ogre::Entity* espada1_;
	Ogre::Entity* espada2_;

	Ogre::Entity* knotFly_; //La puta mosca
	
	Ogre::Animation* animation_;
	// Ogre::AnimationState*  animState_Walk;
	Ogre::AnimationState*  animState_Run;
	Ogre::AnimationState*  animationState_;

};
#endif