#ifndef	_H_PanelMan_H_
#define _H_PanelMan_H_

#include "EntityMan.h"

class PanelMan // : public MyApplicationContext, public EntityMan
{
public:
	PanelMan(Ogre::SceneNode* sceneNode, Ogre::TexturePtr texturePtr);
	virtual ~PanelMan();

	// bool mousePicking(const OgreBites::MouseButtonEvent& event) { return true; }
	// void frameRendered(const Ogre::FrameEvent& event) {}

private:
	Ogre::SceneNode* sceneNode_;
	Ogre::Entity* entity_;
	Ogre::Camera* cameraRef_; // Para el reflejo
	Ogre::TexturePtr texturePtr_;
	// OgreBites::InputListener* list_ = new OgreBites::InputListener();
};

#endif