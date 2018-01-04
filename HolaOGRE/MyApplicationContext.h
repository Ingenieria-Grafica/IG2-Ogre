#ifndef __MyApplicationContext_H__
#define __MyApplicationContext_H__

#include "Ogre.h""
#include "OgreRoot.h"
#include "OgreRenderWindow.h"
#include "OgreFrameListener.h"
#include "OgreWindowEventUtilities.h"
#include "OgreInput.h"
#include "OgreOverlaySystem.h"
#include "OgreFileSystemLayer.h"
#include "OgreSGTechniqueResolverListener.h"

//#include "OgreBitesPrerequisites.h"
//#include "OgreBuildSettings.h"
//#include "OgrePlugin.h"
//#include "OgreStaticPluginLoader.h"

class MyApplicationContext : public Ogre::FrameListener, public Ogre::WindowEventListener
{
  public:
    explicit MyApplicationContext(const Ogre::String& appName = OGRE_VERSION_NAME, bool grabInput = false);  // true -> false
    virtual ~MyApplicationContext();
    // This function initializes the render system and resources.
    void initApp();
    //This function closes down the application - saves the configuration then shutdowns.
    void closeApp();
    
    Ogre::RenderWindow* getRenderWindow() const { return mWindow; }
    Ogre::Root* getRoot() const { return mRoot; }
    Ogre::OverlaySystem* getOverlaySystem() const { return mOverlaySystem; }

    // attach input listener
    void addInputListener(OgreBites::InputListener* lis) { mInputListeners.insert(lis); }
    // detach input listener
    void removeInputListener(OgreBites::InputListener* lis) { mInputListeners.erase(lis); }
   
  protected:

    // Create the render window to be used for this context here.
    // You must use SDL and not an auto-created window as SDL does not get the events otherwise. 
    virtual Ogre::RenderWindow* createWindow(const Ogre::String& name);

    // Initialize the RT Shader system.
    bool initialiseRTShaderSystem();
    // Destroy the RT Shader system.
    void destroyRTShaderSystem();

    // Creates the OGRE root.
    virtual void createRoot();

    // Configures the startup settings for OGRE. I use the config dialog here, but you can also restore from a config file. 
    // Note that this only happens when you start the context, and not when you reset it. 
    virtual bool oneTimeConfig();

    // Sets up the context after configuration.
    virtual void setup();
    // Cleans up and shuts down the context.
    virtual void shutdown();

    // Sets up SDL input.
    virtual void setupInput();

    // Finds context-wide resource groups. I load paths from a config file here,
    // but you can choose your resource locations however you want. 
    virtual void locateResources();

    // Loads context-wide resource groups. I chose here to simply initialise all
    // groups, but you can fully load specific ones if you wish. 
    virtual void loadResources();

    // poll for any events for the main window
    void pollEvents();
    //inspect the event and call one of the corresponding functions on the registered InputListener
    void _fireInputEvent(const OgreBites::Event& event) const;

    // callback interface copied from various listeners to be used by ApplicationContext
    virtual bool frameStarted(const Ogre::FrameEvent& evt) { pollEvents(); return true; }
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }
    virtual void windowMoved(Ogre::RenderWindow* rw)  {}
    virtual void windowResized(Ogre::RenderWindow* rw) {}
    virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
    virtual void windowClosed(Ogre::RenderWindow* rw) {}
    virtual void windowFocusChange(Ogre::RenderWindow* rw)  {}

    Ogre::RenderWindow* mWindow;    // render window
    SDL_Window* mSDLWindow;
    Ogre::Root* mRoot;    // OGRE root

    Ogre::OverlaySystem* mOverlaySystem;  // Overlay system
    Ogre::FileSystemLayer* mFSLayer; // File system abstraction layer
 
    bool mGrabInput;
    bool mFirstRun;
    Ogre::String mAppName;
   
    std::set<OgreBites::InputListener*> mInputListeners;
    Ogre::RTShader::ShaderGenerator* mShaderGenerator; // The Shader generator instance.
    OgreBites::SGTechniqueResolverListener* mMaterialMgrListener; // Shader generator material manager listener.
    Ogre::String mRTShaderLibPath;

    //Ogre::String mNextRenderer;     // name of renderer used for next run
};

#endif