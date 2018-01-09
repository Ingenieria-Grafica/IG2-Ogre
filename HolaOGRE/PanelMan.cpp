#include "PanelMan.h"

using namespace Ogre;

PanelMan::PanelMan(SceneNode* sceneNode, TexturePtr texturePtr, SceneNode * CamNode) : EntityMan(sceneNode),
camNode_(CamNode), texturePtr_(texturePtr)
{
	sceneNode_ = sceneNode;
	sceneNode_->scale(0.3, 0.3, 0.3);
	//sceneNode_->setPosition(0, -25, -50);
	sceneNode_->pitch(Ogre::Degree(-90));


	cameraRef_ = sceneNode_->getCreator()->createCamera("RefCam");

	cameraRef_->enableReflection(Plane(Vector3::UNIT_Y, 0));
	cameraRef_->enableCustomNearClipPlane(Plane(Vector3::UNIT_Y, 0));

	cameraRef_->setNearClipDistance(1);
	cameraRef_->setFarClipDistance(10000);
	cameraRef_->setAutoAspectRatio(true);

	camNode_->attachObject(cameraRef_);

	entity_ = sceneNode_->getCreator()->createEntity("entFondo", "mFondo");
	
	// material del plano
	MyApplicationContext::addInputListener(list_);
	entity_->getSubEntity(0)->setMaterialName("panel", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	setEntMan(entity_);
	
	RenderTexture* renderTexture = texturePtr_->getBuffer()->getRenderTarget();
	Viewport * v = renderTexture->addViewport(cameraRef_);
	v->setClearEveryFrame(true);
	v->setBackgroundColour(ColourValue::Black);

	TextureUnitState* t = entity_->getSubEntity(0)->getMaterial()->
		getTechnique(0)->getPass(0)->
		createTextureUnitState("texRtt");
	t->setColourOperation(LBO_ADD); // backgroundColour -> black
	// LBO_MODULATE / LBO_REPLACE / LBO_ALPHA_BLEND;
	t->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
	t->setProjectiveTexturing(true, cameraRef_);

	entity_->setQueryFlags(0);
}


PanelMan::~PanelMan()
{
	// MyApplicationContext::removeInputListener(list_);
}
