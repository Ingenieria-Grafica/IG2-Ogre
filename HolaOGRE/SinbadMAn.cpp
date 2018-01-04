#include "SinbadMAn.h"


using namespace Ogre;

SinbadMan::SinbadMan(SceneNode * sceneNode) : sceneNode_(sceneNode)
{
	entity_ = sceneNode_->getCreator()->createEntity("entSinbad", "Sinbad.mesh");
	//node->setPosition(0, 0, 25);
	sceneNode_->scale(5, 5, 5);
	//node->showBoundingBox(true);
	//node->roll(Ogre::Degree(-45));
	sceneNode_->attachObject(entity_);
	// MyApplicationContext::addInputListener(list);

	///ANIMACION
	// animState_Walk = entity_->getAnimationState("BaseWalk");
	// animState_Walk->setLoop(true);
	// animState_Walk->setEnabled(true);

	animState_Run = entity_->getAnimationState("RunBase");
	animState_Run->setLoop(true);
	animState_Run->setEnabled(true);

	espada1_ = sceneNode_->getCreator()->createEntity("entEspada1", "Sword.mesh");
	entity_->attachObjectToBone("Handle.L", espada1_);
	espada2_ = sceneNode_->getCreator()->createEntity("entEspada2", "Sword.mesh");
	entity_->attachObjectToBone("Handle.R", espada2_);

	


	entity_->setQueryFlags(-1);

	int duracion = 10, movimiento = 80;
	Vector3 keyframePos(-50, 0, 50), scale(5, 5, 5); 

	animation_ = sceneNode_->getCreator()->createAnimation("animKnot", duracion);
	NodeAnimationTrack * track = animation_->createNodeTrack(0);
	track->setAssociatedNode(sceneNode_);

	Real longitudPaso = duracion / 4.0;
	TransformKeyFrame * transformKeyFrame_;

	// En total habra 8 keyFrames. 4 para las posiciones y 4 para las rotaciones
	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 0); // Keyframe 0
	keyframePos += Ogre::Vector3::UNIT_X* movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(-1, 0, 0)));

	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 1
	keyframePos -= Ogre::Vector3::UNIT_X * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(-1, 0, 0)));


	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 1); // Keyframe 2
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(0, 0, -1)));

	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 3
	keyframePos -= Ogre::Vector3::UNIT_Z * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(0, 0, -1)));

	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 2); // Keyframe 4
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(1, 0, 0)));

	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 5
	keyframePos += Ogre::Vector3::UNIT_X * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(Vector3(0, 0, 1).getRotationTo(Vector3(1, 0, 0)));


	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 3); // Keyframe 6
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);

	transformKeyFrame_ = track->createNodeKeyFrame(longitudPaso * 4); // Keyframe 7
	keyframePos += Ogre::Vector3::UNIT_Z * movimiento;
	transformKeyFrame_->setTranslate(keyframePos); // Arriba
	transformKeyFrame_->setScale(scale);


	animationState_ = sceneNode_->getCreator()->createAnimationState("animKnot");
	animationState_->setLoop(true);
	animationState_->setEnabled(true);

}


SinbadMan::~SinbadMan()
{
	// MyApplicationContext::removeInputListener(list);
}
