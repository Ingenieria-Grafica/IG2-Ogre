#ifndef _H_KnotMan_H_
#define	_H_KnotMan_H_

#include "EntityMan.h"

// Nunca en el h bis...
// using namespace Ogre;

class KnotMan : public EntityMan, public OgreBites::InputListener
{
public:
	KnotMan(Ogre::SceneNode* sceneNode);
	~KnotMan();

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event) { return true; };
	virtual void frameRendered(const Ogre::FrameEvent& event);

private:
	OgreBites::InputListener * list = new OgreBites::InputListener(); //Porque debería ser clickable pero no hace nada xd
	Ogre::SceneNode* sceneNode_;			//La escena
	Ogre::Entity* entity_Knot;			//La entidad que contiene el mesh de la mosca.

};
#endif
