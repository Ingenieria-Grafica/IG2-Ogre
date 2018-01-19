#include "SinbadMAn.h"


using namespace Ogre;

SinbadMan::SinbadMan(SceneNode * sceneNode) : EntityMan(sceneNode)
{
	sceneNode_ = sceneNode;
	sceneNode_->setPosition(-50, 25, 50);
	sceneNode_->scale(5, 5, 5);

	keyframePos = sceneNode_->getPosition(), scale = (5, 5, 5);

	//Booleanos
	camina = true;
	haciaLaBomba = false;
	muerto = false;

	entity_ = sceneNode_->getCreator()->createEntity("entSinbad", "Sinbad.mesh");
	setEntMan(entity_);
	
	//node->showBoundingBox(true);
	//node->roll(Ogre::Degree(-45));
	//sceneNode_->attachObject(entity_);
	


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

	entity_->setQueryFlags(1); 


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

void SinbadMan::animCaminadoBomba() {

	haciaLaBomba = true;
	camina = false;

	animationState_Walk->setEnabled(false);		//Desactivo la de caminado
	animState_Run->setEnabled(true);			//Activo la de base (REDUNDANTE, LO SÉ)
	animState_RunTop->setEnabled(false);		//Desactivo los brazos
	Vector3 vpSinbad(0, 0, 1);

	animation_Bomb = sceneNode_->getCreator()->createAnimation("animGoToBomb", duracion);//Duracion total de la animación
	trackBomb = animation_Bomb->createNodeTrack(0);
	trackBomb->setAssociatedNode(sceneNode_);

	entity_->detachObjectFromBone(espada1_);
	entity_->attachObjectToBone("Handle.L", espada1_);

	TransformKeyFrame * transformKeyFrame_;
	//CARGAR NODO BOMBA
	SceneNode * bombEnt = sceneNode_->getCreator()->getEntity("entBomb")->getParentSceneNode();
	//Tomamos la poisicion del Sinbad en este momento
	keyframePos = sceneNode_->getPosition();
	Real initialYPos = sceneNode_->getPosition().y;

	scale = { 5, 5, 5 };
	Vector3 rot = Vector3(bombEnt->getPosition().x - sceneNode_->getPosition().x, 0, bombEnt->getPosition().z - sceneNode_->getPosition().z);
	rot.normalise();
	Quaternion quat = vpSinbad.getRotationTo(rot);
	quat.normalise();

	transformKeyFrame_ = trackBomb->createNodeKeyFrame(0);
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(quat);

	transformKeyFrame_ = trackBomb->createNodeKeyFrame(duracion);
	keyframePos = Vector3(bombEnt->getPosition().x, 25, bombEnt->getPosition().z);	//LUGAR DE LA BOMBA
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(quat);

	animationState_Bomb = sceneNode_->getCreator()->createAnimationState("animGoToBomb");
	animationState_Bomb->setEnabled(true);
	animationState_Bomb->setLoop(false);

}

void SinbadMan::animMuerte(){
	camina = false;
	haciaLaBomba = false;
	muerto = true;

	animationState_Walk->setEnabled(false);
	animationState_Bomb->setEnabled(false);
	
	Vector3 vpSinbad(0, 0, 1);
	Real displacement = 100;

	animation_Die = sceneNode_->getCreator()->createAnimation("animDragSinbad", duracion*2);//Duracion total de la animación
	trackDie = animation_Die->createNodeTrack(0);
	trackDie->setAssociatedNode(sceneNode_);

	entity_->detachObjectFromBone(espada1_);
	entity_->attachObjectToBone("Handle.L", espada1_);

	TransformKeyFrame * transformKeyFrame_;
	//Tomamos la posicion del Sinbad en este momento
	keyframePos = sceneNode_->getPosition();
	keyframePos -= Vector3::UNIT_Y * 1; 

	scale = { 5, 5, 5 };
	Vector3* rot = new Vector3(0, 180, 0);
	rot->normalise();
	Quaternion quat = vpSinbad.getRotationTo(*rot);

	transformKeyFrame_ = trackDie->createNodeKeyFrame(0);
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(quat);

	transformKeyFrame_ = trackDie->createNodeKeyFrame(duracion * 2);
	keyframePos += Vector3::UNIT_X * displacement;
	transformKeyFrame_->setTranslate(keyframePos);
	transformKeyFrame_->setScale(scale);
	transformKeyFrame_->setRotation(quat);

	animationState_Die = sceneNode_->getCreator()->createAnimationState("animDragSinbad");
	animationState_Die->setEnabled(true);
	animationState_Die->setLoop(false);
}