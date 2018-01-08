#ifndef	_H_EntityMan_H_
#define _H_EntityMan_H_

#include "MyApplicationContext.h"
#include <OgreTrays.h>
#include <OgreCameraMan.h>

class EntityMan; //Declaraci�n adelantada

class UserControl { //Clase recubridora de EntityMan
public:
	EntityMan* getEntity() { return Entity_; };
	friend class EntityMan;	// Para usar los m�todos de EntityMan

protected:
	UserControl(EntityMan* ent) : Entity_(ent) {};
	~UserControl() { };
	EntityMan * Entity_;

};

class EntityMan
{
public:
	//static const Ogre::uint32 MY_QUERY_MASK = 1; //0 //Supongo que no hace falta, pero por si acaso
	// Si no est� a nullptr el parametro da error en las clases hijas
	EntityMan(Ogre::SceneNode * sceneNode) { 
		sceneNode_ = sceneNode;
		control = new UserControl(this);
	}

	virtual ~EntityMan() { 
		//Suponemos que solo puede tener controlador el primer objeto adjunto a un nodo
		UserControl* pCtrl = Ogre::any_cast<UserControl*>
			(sceneNode_->getAttachedObject(0)->getUserObjectBindings().getUserAny());
		delete pCtrl;
	}

	virtual bool mousePicking(const OgreBites::MouseButtonEvent& event){ return true; };
	virtual void frameRendered(const Ogre::FrameEvent& event) {};

	//M�todo que comprueba que es el primer objeto que se adjunta al nodo.
	virtual void setEntMan(Ogre::MovableObject * mObj) {

		if (sceneNode_->getAttachedObjects().size() == 0)
		{
			sceneNode_->attachObject(mObj);
			sceneNode_->getAttachedObject(0)->getUserObjectBindings().setUserAny(Ogre::Any(control));
		}
	}


protected:
	Ogre::SceneNode * sceneNode_ = nullptr;
	Ogre::RenderTexture* renderTexture_;
	UserControl* control = nullptr;
	OgreBites::InputListener* list_;
};
#endif