#include "HolaApp.h"


using namespace Ogre;

void HolaApp::frameRendered(const FrameEvent &  evt)
{
	 for (size_t i = 0; i < vecManagement.size(); i++)
	 {
	 	vecManagement[i]->frameRendered(evt);
	 }
}

bool HolaApp::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	if (evt.keysym.sym == SDLK_ESCAPE)
		mRoot->queueEndRendering();
	else if (evt.keysym.sym == SDLK_t){
		input = !input;
		if (input)
			camMan->setTarget(nodeSinbad);
		else
			camMan->setTarget(scnMgr->getRootSceneNode());
	}


	return true;
}

bool HolaApp::mousePressed(const OgreBites::MouseButtonEvent &  evt)
{
	rayScnQuery->setRay(cam->getCameraToViewportRay(
		evt.x / (Real)mWindow->getViewport(0)->getActualWidth(),
		evt.y / (Real)cam->getViewport()->getActualHeight()));
	// coordenadas normalizadas en [0,1]
	RaySceneQueryResult& qryResult = rayScnQuery->execute();
	RaySceneQueryResult::iterator it = qryResult.begin();

	 if (it != qryResult.end()) {
	// 
	// 	seeBoundingBox = !seeBoundingBox;
	// 	it->movable->getParentSceneNode()->showBoundingBox(seeBoundingBox);
		 UserControl* pCtrl = any_cast<UserControl*>(it->movable ->
			 getUserObjectBindings().getUserAny());
		 pCtrl->getEntity()->mousePicking(evt);
	 }

	return true;
}

bool HolaApp::mouseMoved(const OgreBites::MouseMotionEvent& evt){
  
	trayMgr->mouseMoved(evt); 
	return true;
}

void HolaApp::setupInput()
{
  // do not forget to call the base first
  MyApplicationContext::setupInput(); 
  // register for input events
  addInputListener(this);
}

void HolaApp::shutdown()
{
	for (size_t i = 0; i < vecManagement.size(); i++)
	{
		delete vecManagement[i];
	}
	// scnMgr->removeRenderQueueListener(mOverlaySystem);
	delete trayMgr;  trayMgr = nullptr;
	// do not forget to call the base 
	MyApplicationContext::shutdown();
}

void HolaApp::setup(void)
{
  // do not forget to call the base first
  MyApplicationContext::setup();

  scnMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

  // register our scene with the RTSS
  mShaderGenerator->addSceneManager(scnMgr);

  scnMgr->addRenderQueueListener(mOverlaySystem);

  trayMgr = new OgreBites::TrayManager("TrayGUISystem", mWindow);
  trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
  
  rayScnQuery = scnMgr->createRayQuery(Ray());	//Instancia el rayo

  setupScene();
}

void HolaApp::setupScene(void)
{
  // without light we would just get a black screen    
  Light* light = scnMgr->createLight("Light");
  light->setDirection(Ogre::Vector3::NEGATIVE_UNIT_Y); // !!! opngl <-> dirección a la fuente de luz
  lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  lightNode->setPosition(0, 100, 100);
  lightNode->attachObject(light);

  // also need to tell where we are
  camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  camNode->setPosition(0, 0, 100);
  camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);

 // Set de CAMARAS //
  cam = scnMgr->createCamera("Cam");
  cam->setNearClipDistance(1);
  cam->setFarClipDistance(10000);
  cam->setAutoAspectRatio(true);
  //cam->setPolygonMode(Ogre::PM_WIREFRAME);  // en material
  camNode->attachObject(cam);

  //CameraMan
  camMan = new OgreBites::CameraMan(camNode);
  addInputListener(camMan);
  camMan -> setStyle(OgreBites::CS_ORBIT);

  // and tell it to render into the main window
  Viewport* vp = getRenderWindow()->addViewport(cam);
  //vp->setBackgroundColour(ColourValue(0, 0, 0));
  //vp->setDimensions(1,1, 1, 1);

  //**************************************************************//
  // Sinbad
  nodeSinbad = scnMgr->getRootSceneNode()->createChildSceneNode("nSinbad");
  SinbadMan* sinbad = new SinbadMan(nodeSinbad);
  vecManagement.push_back(sinbad);
  // addInputListener(sinbad);

  //Bomba
  // ParticleSystem * pSys = scnMgr->createParticleSystem("partSys", "smoke"); // (nombre,nombre script)
  Ogre::SceneNode * nodeBomb = scnMgr->getRootSceneNode()->createChildSceneNode("nBomb");
  BombMan* bomba = new BombMan(nodeBomb, sinbad);
  vecManagement.push_back(bomba);
  // addInputListener(bomba);

  //Knotfly
  Ogre::SceneNode * nodeKnotFly = scnMgr->getEntity("entSinbad")->getParentSceneNode()->createChildSceneNode("nKnotFly");
  nodeKnotFly->setInheritOrientation(false);
  KnotMan* knotFly = new KnotMan(nodeKnotFly);
  vecManagement.push_back(knotFly);
  // addInputListener(knotFly);

  camMan->setTarget(nodeSinbad); //Para que siga a Sinbad.

  // Planos //
  TexturePtr rttTex = TextureManager::getSingleton().createManual(
	  "texRtt",
	  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	  TEX_TYPE_2D,
	  (Real)mWindow->getViewport(0)->getActualWidth(),
	  (Real)cam->getViewport()->getActualHeight(),
	  0, PF_R8G8B8, TU_RENDERTARGET);

  Ogre::SceneNode* nodePlane = scnMgr->getRootSceneNode()->createChildSceneNode("nPlane");
  MeshPtr plane = MeshManager::getSingleton().createPlane("mFondo",
	  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	  Plane(Vector3::UNIT_Z, 0),
	  (Real)mWindow->getViewport(0)->getActualWidth(),
	  (Real)cam->getViewport()->getActualHeight(),
	  10, 10, true, 1, 1.0, 1.0, Vector3::UNIT_Y);
  PanelMan* panel = new PanelMan(nodePlane, rttTex, camNode);
  vecManagement.push_back(panel);

  // nMgr->setSkyPlane(true, Plane(Vector3::UNIT_Z, -20),
	// "Ejemplo2", 1, 1, true, 4.0, 100, 100);
  scnMgr->setSkyPlane(true, Plane(Vector3::UNIT_Z, -100),
  "mandelbrot1", 1, 1, true, 6.0, 100, 100);

  // SceneQueries
  rayScnQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
  rayScnQuery->setQueryMask(MY_QUERY_MASK);
  rayScnQuery->setSortByDistance(true);
}

