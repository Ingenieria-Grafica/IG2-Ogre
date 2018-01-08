#ifndef __HolaApp_H__
#define __HolaApp_H__

#include "MyApplicationContext.h"
#include <OgreTrays.h>
#include <OgreCameraMan.h>
#include "EntityMan.h"
#include "SinbadMAn.h"
#include "PanelMan.h"
#include "BombMan.h"
#include "KnotMan.h"

class HolaApp :
	public MyApplicationContext, public OgreBites::InputListener
{
public:
  explicit HolaApp() : MyApplicationContext("HolaApp") { };
  virtual ~HolaApp(){ };   // delete ... los elementos creado con métodos factoria (create...) se destruyen automáticamente 
  
protected:
  virtual void setup();
  virtual void shutdown();
  virtual void setupInput();
  virtual void setupScene();
  virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
  virtual bool mousePressed(const OgreBites::MouseButtonEvent &  evt);
  virtual bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
  virtual void frameRendered(const Ogre::FrameEvent &  evt);

  Ogre::SceneManager* scnMgr = nullptr;
  OgreBites::TrayManager* trayMgr = nullptr;

  Ogre::SceneNode* lightNode = nullptr;
  Ogre::SceneNode* camNode = nullptr;
  Ogre::SceneNode * nodeSinbad = nullptr;
  
  OgreBites::CameraMan* camMan = nullptr;
  Ogre::RaySceneQuery * rayScnQuery = nullptr;
  Ogre::Camera * cam = nullptr;
  
  bool input = false;
 
  static const Ogre::uint32 MY_QUERY_MASK = 1; // << 0;
  static const Ogre::uint32 ZERO_QUERY_MASK = 0;

	/* const Ogre::uint32
	  NPC_MASK = 1 << 0, // 0000 0000 0000 0001
	  ENEMY_MASK = 1 << 1, // 0000 0000 0000 0010
	  ITEM_MASK = 1 << 2, // 0000 0000 0000 0100
	  NONHOSTILE_MASK = NPC_MASK | ITEM_MASK;
	*/

  std::vector<EntityMan*> vecManagement;
};
#endif
