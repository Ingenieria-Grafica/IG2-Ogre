#include "SinbadMAn.h"


using namespace Ogre;

SinbadMan::SinbadMan(SceneNode * sceneNode) : sceneNode_(sceneNode), EntityMan(sceneNode)
{
	entity_ = sceneNode_->getCreator()->createEntity("entSinbad", "Sinbad.mesh");
	//node->setPosition(0, 0, 25);
	sceneNode_->scale(5, 5, 5);
	//node->showBoundingBox(true);
	//node->roll(Ogre::Degree(-45));
	//sceneNode_->attachObject(entity_);
	setEntMan(entity_);


	///ANIMACION
	// animState_Walk = entity_->getAnimationState("BaseWalk");
	// animState_Walk->setLoop(true);
	// animState_Walk->setEnabled(true);

	//CICLO DE CORRER: Parte alta y piernas.
	animState_Run = entity_->getAnimationState("RunBase");
	animState_Run->setLoop(true);
	animState_Run->setEnabled(true);

	animState_RunTop = entity_->getAnimationState("RunTop");
	animState_RunTop->setLoop(true);
	animState_RunTop->setEnabled(true);

	//Ciclo de caminado en cuadrado.

	animation_Walk = sceneNode_->getCreator()->createAnimation("animSinbadWalk", duracion);
	trackWalk = animation_Walk->createNodeTrack(0);
	trackWalk->setAssociatedNode(sceneNode_);


	espada1_ = sceneNode_->getCreator()->createEntity("entEspada1", "Sword.mesh");
	entity_->attachObjectToBone("Sheath.L", espada1_);
	espada2_ = sceneNode_->getCreator()->createEntity("entEspada2", "Sword.mesh");
	entity_->attachObjectToBone("Handle.R", espada2_);

	


	entity_->setQueryFlags(-1); //nani??????????????????????????????????


	animCaminadoCuadrado(); //Construimos los keyframes del ciclo de caminado.

	animationState_Walk = sceneNode_->getCreator()->createAnimationState("animSinbadWalk");
	animationState_Walk->setLoop(true);
	animationState_Walk->setEnabled(true);

}


SinbadMan::~SinbadMan()
{
	// MyApplicationContext::removeInputListener(list);
}

void SinbadMan::animCaminadoCuadrado() {
	Vector3 keyframePos(-50, 0, 50), scale(5, 5, 5);

	int movimiento = 80;

	Real longitudPaso = duracion / 4.0;
	TransformKeyFrame * transformKeyFrame_;

	// En total habra 8 keyFrames. 4 para las posiciones y 4 para las rotaciones
	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0
	keyframePos += Ogre::Vector3::UNIT_X* movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(-1, 0, 0)));

	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1
	keyframePos -= Ogre::Vector3::UNIT_X * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(-1, 0, 0)));


	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 1); // Keyframe 2
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(0, 0, -1)));

	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 2); // Keyframe 3
	keyframePos -= Ogre::Vector3::UNIT_Z * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(0, 0, -1)));

	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 2); // Keyframe 4
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(1, 0, 0)));

	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 3); // Keyframe 5
	keyframePos += Ogre::Vector3::UNIT_X * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(1, 0, 0)));


	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 3); // Keyframe 6
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);

	transformKeyFrame_ = trackWalk->createNodeKeyFrame(longitudPaso * 4); // Keyframe 7
	keyframePos += Ogre::Vector3::UNIT_Z * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);

}

/*void SinbadMan::animCaminadoBomba() {

}*/