#pragma once
#include "EntityMan.h"

using namespace Ogre;

class KnotMan : public EntityMan, public OgreBites::InputListener
{
public:
	KnotMan(SceneNode* sceneNode);
	~KnotMan();

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event) { return true; };
	virtual void frameRendered(const Ogre::FrameEvent& event);

private:
	OgreBites::InputListener * list = new OgreBites::InputListener(); //Porque debería ser clickable pero no hace nada xd
	SceneNode* sceneNode_;			//La escena
	Entity* entity_Knot;			//La entidad que contiene el mesh de la mosca.

};

