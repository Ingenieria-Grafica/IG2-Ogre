#include "MyApplicationContext.h"

#include "OgreGpuProgramManager.h"
#include "OgreConfigFile.h"
#include "OgreLogManager.h"
#include "OgreDataStream.h"
#include "OgreBitesConfigDialog.h"

#include <SDL_video.h>
#include <SDL_syswm.h>  

//-----------------------------------------------------------------------------------------------

MyApplicationContext::MyApplicationContext(const Ogre::String& appName, bool grabInput)
{
    mAppName = appName;
    mGrabInput = grabInput;
    mFSLayer = new Ogre::FileSystemLayer(mAppName); // crea un directorio en Mis documentos, pero los archivos en bin
    mRoot = nullptr;
    mWindow = nullptr;
    mOverlaySystem = nullptr;
    mSDLWindow = nullptr;
    mFirstRun = true;
    mMaterialMgrListener = nullptr;
    mShaderGenerator = nullptr;
}

MyApplicationContext::~MyApplicationContext()
{
    delete mFSLayer;
}

void MyApplicationContext::initApp()
{
    createRoot();
    if (oneTimeConfig())
    { 
      setup();
    }
}

void MyApplicationContext::closeApp()
{
    mRoot->saveConfig();  
    shutdown();
   
    delete mRoot;
    mRoot = nullptr;
}

void MyApplicationContext::createRoot()
{
  Ogre::String pluginsPath;
  Ogre::String homePath;

  pluginsPath = mFSLayer->getConfigFilePath("plugins.cfg");
  homePath = pluginsPath;
  homePath.erase(homePath.find_last_of("\\") + 1, pluginsPath.size() - 1);

  mFSLayer->setHomePath(homePath);   // para que no se creen en Mis documentos (el directorio se crea pero los archivos no)

  mRoot = new Ogre::Root(pluginsPath, mFSLayer->getWritablePath("ogre.cfg"), mFSLayer->getWritablePath("ogre.log"));

}

bool MyApplicationContext::oneTimeConfig()
{
  if (!mRoot->restoreConfig())
  {
    return mRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
  }
  else return true;
}

void MyApplicationContext::setup()
{
  mOverlaySystem = new Ogre::OverlaySystem();
  mWindow = createWindow(mAppName);

  setupInput();
  locateResources();

  initialiseRTShaderSystem();

  loadResources();

  // adds context as listener to process context-level (above the sample level) events
  mRoot->addFrameListener(this);
}

void MyApplicationContext::shutdown()
{
  // Destroy the RT Shader System.
  destroyRTShaderSystem();

  // remove window event listener before destroying it
  if (mWindow != nullptr)
  {
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    mRoot->destroyRenderTarget(mWindow);
    mWindow = nullptr;
  }

  delete mOverlaySystem;
  mOverlaySystem = nullptr;

  if (mSDLWindow != nullptr) {
    SDL_DestroyWindow(mSDLWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    mSDLWindow = nullptr;
  }

}

bool MyApplicationContext::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for (std::set<OgreBites::InputListener*>::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
  {
    (*it)->frameRendered(evt);
  }

  return true;
}

bool MyApplicationContext::initialiseRTShaderSystem()
{
  if (Ogre::RTShader::ShaderGenerator::initialize())
  {
      mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

      // Core shader libs not found -> shader generating will fail.
      if (mRTShaderLibPath.empty())
         return false;

      // Create and register the material manager listener if it doesn't exist yet.
      if (mMaterialMgrListener == nullptr)       
      {
        mMaterialMgrListener = new OgreBites::SGTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
      }
  }
  return true;
}

void MyApplicationContext::destroyRTShaderSystem()
{
  mShaderGenerator->removeAllShaderBasedTechniques();   
  mShaderGenerator->flushShaderCache();                 

  // Restore default scheme.
  Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

  // Unregister the material manager listener.
  if (mMaterialMgrListener != nullptr)       
  {
    Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
    delete mMaterialMgrListener;
    mMaterialMgrListener = nullptr;
  }

  // Destroy RTShader system.
  if (mShaderGenerator != nullptr)
  {
    Ogre::RTShader::ShaderGenerator::destroy();
    mShaderGenerator = nullptr;
  }
}

void MyApplicationContext::pollEvents()  // frameStarted
{
  //if (mSDLWindow == nullptr) return;  // SDL events not initialized

  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      mRoot->queueEndRendering();
      break;
    case SDL_WINDOWEVENT:
      if (event.window.windowID == SDL_GetWindowID(mSDLWindow))  {

        if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          //mWindow->resize(event.window.data1, event.window.data2);  // ERROR
          mWindow->windowMovedOrResized();
          windowResized(mWindow);
        }
      }
      break;
    default:
      _fireInputEvent(event);
      break;
    }
  }

}

void MyApplicationContext::_fireInputEvent(const OgreBites::Event& event) const
{
    for (std::set<OgreBites::InputListener*>::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it) 
    {
      OgreBites::InputListener& l = **it;

      switch (event.type)
      {
      case SDL_KEYDOWN:
        //if (event.key.repeat) break; // Ignore repeated signals from key being held down.
        l.keyPressed(event.key);
        break;
      case SDL_KEYUP:
        l.keyReleased(event.key);
        break;
      case SDL_MOUSEBUTTONDOWN:
        l.mousePressed(event.button);
        break;
      case SDL_MOUSEBUTTONUP:
        l.mouseReleased(event.button);
        break;
      case SDL_MOUSEWHEEL:
        l.mouseWheelRolled(event.wheel);
        break;
      case SDL_MOUSEMOTION:
        l.mouseMoved(event.motion);
        break;
      case SDL_FINGERDOWN:
        // for finger down we have to move the pointer first
        l.touchMoved(event.tfinger);
        l.touchPressed(event.tfinger);
        break;
      case SDL_FINGERUP:
        l.touchReleased(event.tfinger);
        break;
      case SDL_FINGERMOTION:
        l.touchMoved(event.tfinger);
        break;
      }
    }
}

void MyApplicationContext::setupInput()
{
  if (mSDLWindow == nullptr)
  { // throw Exception
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
      "you must create a SDL window first", "SampleContext::setupInput");
  }

  SDL_bool grab = SDL_bool(mGrabInput);
  SDL_ShowCursor(grab);
  SDL_SetWindowGrab(mSDLWindow, grab);
  SDL_SetRelativeMouseMode(grab);
}

void MyApplicationContext::loadResources()
{
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

Ogre::RenderWindow * MyApplicationContext::createWindow(const Ogre::String& name)
{
  mRoot->initialise(false);
  Ogre::NameValuePairList miscParams;

  Ogre::ConfigOptionMap ropts = mRoot->getRenderSystem()->getConfigOptions();

  Ogre::uint32 w, h;

  std::istringstream mode(ropts["Video Mode"].currentValue);
  Ogre::String token;
  mode >> w; // width
  mode >> token; // 'x' as seperator between width and height
  mode >> h; // height

  miscParams["FSAA"] = ropts["FSAA"].currentValue;
  miscParams["vsync"] = ropts["VSync"].currentValue;

  if (!SDL_WasInit(SDL_INIT_VIDEO))
  {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
  }

  mSDLWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);

  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(mSDLWindow, &wmInfo);

  miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

  return mRoot->createRenderWindow(name, w, h, false, &miscParams);
}

void MyApplicationContext::locateResources()
{
  // load resource paths from config file
  Ogre::ConfigFile cf;

  cf.load(mFSLayer->getConfigFilePath("resources.cfg"));

  Ogre::String sec, type, arch;
  // go through all specified resource groups
  Ogre::ConfigFile::SettingsBySection_::const_iterator seci;
  for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
  {
    sec = seci->first;
    const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
    Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

    // go through all resource paths
    for (i = settings.begin(); i != settings.end(); i++)
    {
      type = i->first;
      arch = Ogre::FileSystemLayer::resolveBundlePath(i->second);
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
    }
  }

  sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
  const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);

  OgreAssert(!genLocs.empty(), ("Resource Group '" + sec + "' must contain at least one entry").c_str());

  arch = genLocs.front()->archive->getName();

  arch = Ogre::StringUtil::replaceAll(arch, "Media/../../Tests/Media", "");
  arch = Ogre::StringUtil::replaceAll(arch, "media/../../Tests/Media", "");

  type = genLocs.front()->archive->getType();

  // Add locations for supported shader languages
  if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSLES", type, sec);
  }
  else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL120", type, sec);

    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl150"))
    {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL150", type, sec);
    }
    else
    {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL", type, sec);
    }

    if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl400"))
    {
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/GLSL400", type, sec);
    }
  }
  else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch + "/materials/programs/HLSL", type, sec);
  }

  mRTShaderLibPath = arch + "/RTShaderLib";
  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/materials", type, sec);

  if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsles"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSLES", type, sec);
  }
  else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("glsl"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/GLSL", type, sec);
  }
  else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("hlsl"))
  {
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mRTShaderLibPath + "/HLSL", type, sec);
  }

}

//------------------------------------------------------------------------------------------------
