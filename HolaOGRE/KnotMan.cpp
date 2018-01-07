#include "KnotMan.h"

using namespace Ogre;

KnotMan::KnotMan(SceneNode* sceneNode): EntityMan(sceneNode)
{
	sceneNode_ = sceneNode;

	//entity_Knot = sceneNode_->getCreator()->getRootSceneNode()->createChildSceneNode("nKnot");
	entity_Knot = sceneNode->getCreator()->createEntity("entKnot", "knot.mesh"); 
	setEntMan(entity_Knot);

	entity_Knot->getSubEntity(0)->setMaterialName("knot_fly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	

	sceneNode->setPosition(0, -10, 0); //Por ejemplo
	sceneNode->setScale(0.1, 0.1, 0.1);
	//sceneNode_->attachObject(entity_Knot); // Examples.material -> "2 – Default " -> MtlPlat2.jpg
	
	

	entity_Knot->setQueryFlags(-1); //Seleccionable skrr
	
}


KnotMan::~KnotMan()
{
}

void KnotMan:: frameRendered(const Ogre::FrameEvent& event) {
//LA gilipollez de que de vueltas. 
	sceneNode_->rotate(Vector3(0.0f, 1.0f, 0.0f), Radian(0.1f));
};