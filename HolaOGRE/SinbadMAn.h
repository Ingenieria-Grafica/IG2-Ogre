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
		if (!haciaLaBomba && !muerto){
			camina = !camina;
			animationState_Walk->setEnabled(camina);
			animState_RunTop->setEnabled(camina);

		}
		//Quitar el enable de la animacion walk y base si queremos que se quede estático (idle)
		return true;
	}
	void frameRendered(const Ogre::FrameEvent& event){
		if (camina){
			animState_Run->addTime(event.timeSinceLastFrame);
			animState_RunTop->addTime(event.timeSinceLastFrame);
			animationState_Walk->addTime(event.timeSinceLastFrame);
		}
		else if (haciaLaBomba) {
			if (!animationState_Bomb->hasEnded()) {
				animationState_Bomb->addTime(event.timeSinceLastFrame);
				animState_RunTop->addTime(event.timeSinceLastFrame);
				animState_Run->addTime(event.timeSinceLastFrame);
			}

			else animMuerte();
		}
		else if (muerto) {
			animationState_Die->addTime(event.timeSinceLastFrame);
		}
	}

	void animCaminadoCuadrado();
	void animCaminadoBomba();
	void animMuerte();

private:

	const int duracion = 10;
	bool camina;
	bool haciaLaBomba;
	bool muerto;

	OgreBites::InputListener * list = new OgreBites::InputListener();
	// Ogre::SceneNode* sceneNode_;

	//Entidades
	Ogre::Entity* entity_;
	Ogre::Entity* espada1_;
	Ogre::Entity* espada2_;

	Ogre::Entity* knotFly_; //La puta mosca

	//Animaciones
	Ogre::Animation* animation_Walk;
	Ogre::Animation* animation_Bomb;
	Ogre::Animation* animation_Die;

	///Anim States
	Ogre::AnimationState*  animState_Run;
	Ogre::AnimationState*  animState_RunTop;		//Ahora tambien mueve los brazos.
	Ogre::AnimationState*  animationState_Walk;
	Ogre::AnimationState*  animationState_Bomb;
	Ogre::AnimationState*  animationState_Die;
	///Tracks
	Ogre::NodeAnimationTrack * trackWalk;
	Ogre::NodeAnimationTrack * trackBomb;
	Ogre::NodeAnimationTrack * trackDie;

	//
	Ogre::Vector3 keyframePos;
	Ogre::Vector3 scale;
	//

};
#endif