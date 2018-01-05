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
		animState_RunTop->addTime(event.timeSinceLastFrame);
		animationState_Walk->addTime(event.timeSinceLastFrame);
	}

	void animCaminadoCuadrado();
	//void animCaminadoBomba();

private:

	const int duracion = 10;

	OgreBites::InputListener * list = new OgreBites::InputListener();
	Ogre::SceneNode* sceneNode_;

	//Entidades
	Ogre::Entity* entity_;
	Ogre::Entity* espada1_;
	Ogre::Entity* espada2_;

	Ogre::Entity* knotFly_; //La puta mosca

	//Animaciones
	Ogre::Animation* animation_Walk;
	Ogre::Animation* animation_Bomb;

	///Anim States
	Ogre::AnimationState*  animState_Run;
	Ogre::AnimationState*  animState_RunTop;		//Ahora tambien mueve los brazos.
	Ogre::AnimationState*  animationState_Walk;
	Ogre::AnimationState*  animationState_Bomb;

	///Tracks
	NodeAnimationTrack * trackWalk;
	NodeAnimationTrack * trackBomb;

	//
	Vector3 keyframePos;
	Vector3 scale;

};
#endif