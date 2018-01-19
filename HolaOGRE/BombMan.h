#ifndef _H_BombMan_H_
#define	_H_BombMan_H_

#include "EntityMan.h";
#include "SinbadMAn.h";

// Nunca en el h...
// using namespace Ogre;

class BombMan : public EntityMan, public OgreBites::InputListener
{
public:
	BombMan(Ogre::SceneNode* sceneNode, SinbadMan * sinbad);
	~BombMan();

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event) { 

		//Cambiarle el movimiento a Sinbad, desde aqui
		//Cuando llegue -> ponerle tumbado (Esto se comprueba en Sinbad?)
		//HAcer que se mueva en una dirección. La mosca creo que se queda por ahí.
		pSystem->setEmitting(true);
		bombAnimationState_->setEnabled(false);
		sinbad_->animCaminadoBomba();                //Cambia la animacion de caminado por la de ir a la bomba
		return true; 
	};

	virtual	void frameRendered(const Ogre::FrameEvent& event) {
		bombAnimationState_->addTime(event.timeSinceLastFrame);
	};

protected:

	//Para crear un sistema de particulas a partir de un script
	Ogre::ParticleSystem * pSystem;		//Herededa de MovableObject. 
	SinbadMan * sinbad_;				//Puntero a Sinbad

	Ogre::SceneNode* sceneNode_;			//La escena
	Ogre::Entity* entity_Bomb;			//La entidad que contiene el script de la bomba

	OgreBites::InputListener * list = new OgreBites::InputListener();
	Ogre::Animation * bombAnimation_;					//La animacion de la bomba
	Ogre::AnimationState*  bombAnimationState_;	//Estado de la animacion

	// están aqui porque las necesitamos en la constructora
	// y en el metodo de la animacion
	float duracion = 10.0f, desplazamiento = 10.0f; //Por ejemplo.
	Ogre::Real longitudPasoBomb = duracion / 4.0;	//Igual que el sinbad, no estoy inspirado.

	void particleAnimation(Ogre::NodeAnimationTrack * trackBomb);
};
#endif
