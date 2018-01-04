#ifndef	_H_EntityMan_H_
#define _H_EntityMan_H_

#include "MyApplicationContext.h"
#include <OgreTrays.h>
#include <OgreCameraMan.h>

class EntityMan
{
public:
	// Si no está a nullptr el parametro da error en las clases hijas
	EntityMan(Ogre::SceneNode * sceneNode = nullptr) { sceneNode_ = sceneNode; }

	virtual ~EntityMan() { }

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event) = 0;
	virtual void frameRendered(const Ogre::FrameEvent& event) = 0;

protected:
	Ogre::SceneNode * sceneNode_;
	Ogre::RenderTexture* renderTexture_;
};
#endif