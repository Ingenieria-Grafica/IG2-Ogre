#include "BombMan.h"

using namespace Ogre;

BombMan::BombMan(SceneNode* sceneNode, SinbadMan * sinBad) : 
sceneNode_(sceneNode), sinbad_(sinBad), EntityMan(sceneNode)
{
	sceneNode_->setPosition(0, 40, 0); //Por ejemplo
	// no funciona
	sceneNode_->setScale(0.05, 0.05, 0.05);

	entity_Bomb = sceneNode_->getCreator()->createEntity("entBomb", "uv_sphere.mesh");
	entity_Bomb->getSubEntity(0)->setMaterialName("uv_sphere", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);	//meterla en apps
	//sceneNode_->attachObject(entity_Bomb); //añadir la entidad a la escena
	setEntMan(entity_Bomb);


	entity_Bomb->setQueryFlags(1); // 1 == selecccionable 

	//Setup de las particulas
	pSystem = sceneNode_->getCreator()->createParticleSystem("parSys", "Smoke");
	
	sceneNode_->attachObject(pSystem);

	pSystem->setEmitting(false);				//Falso por inercia, cuando lo pulsas debería de ponerse a true -> en el onclick


	//ANIMACION//
	bombAnimation_ = sceneNode_->getCreator()->createAnimation("animBomb", duracion);
	NodeAnimationTrack * trackBomb = bombAnimation_->createNodeTrack(0);
	trackBomb->setAssociatedNode(sceneNode_);														//Asocia el tracking a la escena.

	bombAnimationState_ = sceneNode_->getCreator()->createAnimationState("animBomb");
	bombAnimationState_->setLoop(true);
	bombAnimationState_->setEnabled(true);
	
	particleAnimation(trackBomb);
}


BombMan::~BombMan()
{
}

// FVCK THIS SHIT, no puedes poner la animacion en la constructora LMAO xD
// Tiene que ir en un metodo para llamarlo en cada frame que se necesite jajaja
// KEYFRAMES //
// 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
void BombMan::particleAnimation(Ogre::NodeAnimationTrack * trackBomb) {
	TransformKeyFrame * bombKeyFrame;
	
	Vector3 scaleBomb(0.05, 0.05, 0.05);
	Vector3 rotationBomb(10, 0, 0);
	Vector3 kfPosBomb(0, 10, 0);
	
	//Keyframe 0: Origen.
	bombKeyFrame = trackBomb->createNodeKeyFrame(longitudPasoBomb * 0);
	kfPosBomb += Ogre::Vector3::UNIT_Y * desplazamiento;
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb);

	// Keyframe 1: arriba.
	bombKeyFrame = trackBomb->createNodeKeyFrame(longitudPasoBomb * 1);
	kfPosBomb -= Ogre::Vector3::UNIT_Y * desplazamiento;
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb);

	// Keyframe 2: origen. 
	bombKeyFrame = trackBomb->createNodeKeyFrame(longitudPasoBomb * 2);
	kfPosBomb += Ogre::Vector3::UNIT_Y * desplazamiento; //-=?
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb);

	// Keyframe 3: abajo. 
	bombKeyFrame = trackBomb->createNodeKeyFrame(longitudPasoBomb * 3);
	kfPosBomb += Ogre::Vector3::UNIT_Y * desplazamiento * -2;
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb); // Abajo

	// Keyframe 4: origen.
	bombKeyFrame = trackBomb->createNodeKeyFrame(longitudPasoBomb * 4);
	kfPosBomb += Ogre::Vector3::UNIT_Y * desplazamiento;
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb); // Origen.


	
}