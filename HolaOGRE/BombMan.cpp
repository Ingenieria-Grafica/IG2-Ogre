#include "BombMan.h"



BombMan::BombMan(SceneNode* sceneNode)
{
	sceneNode_ = sceneNode;

	entity_Bomb = sceneNode->getCreator()->createEntity("entBomb", "uv_sphere.mesh");

	sceneNode->setPosition(0, -10, 0); //Por ejemplo
	sceneNode->setScale(0.1, 0.1, 0.1);	
	sceneNode->attachObject(entity_Bomb); //añadir la entidad a la escena

	entity_Bomb->getSubEntity(0)->setMaterialName("uv_sphere", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);	//meterla en apps

	entity_Bomb->setQueryFlags(-1);

	//Setup de las particulas
	pSystem = sceneNode_->getCreator()->createParticleSystem("parSys", "Smoke");
	sceneNode_->attachObject(pSystem);
	pSystem->setEmitting(false);				//Falso por inercia, cuando lo pulsas debería de ponerse a true -> en el onclick



	//ANIMACION//
	
	int duracion = 10, desplazamiento = 10; //Por ejemplo.

	bombAnimation_ = sceneNode_->getCreator()->createAnimation("animBomb", duracion);
	NodeAnimationTrack * trackBomb = bombAnimation_->createNodeTrack(0);
	trackBomb->setAssociatedNode(sceneNode_);														//Asocia el tracking a la escena.

	Real longitudPasoBomb = duracion / 4.0;	//Igual que el sinbad, no estoy inspirado.
	Vector3 scaleBomb(0.1, 0.1, 0.1);
	Vector3 rotationBomb(10, 0, 0);
	Vector3 kfPosBomb(0, -10, 0);


	// KEYFRAMES //
	// 5 keyFrames: origen(0), arriba, origen, abajo, origen(4)
	TransformKeyFrame * bombKeyFrame;

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
	bombKeyFrame = trackBomb->createNodeKeyFrame (longitudPasoBomb * 4);
	kfPosBomb += Ogre::Vector3::UNIT_Y * desplazamiento;
	bombKeyFrame->setScale(scaleBomb);
	bombKeyFrame->setTranslate(kfPosBomb); // Origen.


	bombAnimationState_ = sceneNode_->getCreator()->createAnimationState("animBomb");
	bombAnimationState_->setLoop(true);
	bombAnimationState_->setEnabled(true);


}


BombMan::~BombMan()
{
}
