#pragma once

#include "EntityMan.h";
#include "SinbadMAn.h";

using namespace Ogre;

class BombMan : public EntityMan, public OgreBites::InputListener
{
public:
	BombMan(SceneNode* sceneNode, SinbadMan * sinbad);
	~BombMan();

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event) { 

		//Cambiarle el movimiento a Sinbad, desde aqui
		//Cuando llegue -> ponerle tumbado (Esto se comprueba en Sinbad?)
		//HAcer que se mueva en una dirección. La mosca creo que se queda por ahí.

		pSystem->setEmitting(true);
	//	Sinbad_->animCaminadoBomba();

	
		return true; 
	};

	virtual	void frameRendered(const Ogre::FrameEvent& event) {
		bombAnimationState_->addTime(event.timeSinceLastFrame);
	};

private:
	//Para crear un sistema de particulas a partir de un script
	ParticleSystem * pSystem;		//Herededa de MovableObject. 
	SinbadMan * Sinbad_;				//Puntero a Sinbad

	SceneNode* sceneNode_;			//La escena
	Entity* entity_Bomb;			//La entidad que contiene el script de la bomba

	OgreBites::InputListener * list = new OgreBites::InputListener();
	Animation * bombAnimation_;					//La animacion de la bomba
	Ogre::AnimationState*  bombAnimationState_;	//Estado de la animacion
};

