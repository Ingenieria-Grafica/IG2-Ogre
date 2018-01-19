#include "KnotMan.h"

using namespace Ogre;

KnotMan::KnotMan(SceneNode* sceneNode) : EntityMan(sceneNode)
{
	sceneNode_ = sceneNode;

	//entity_Knot = sceneNode_->getCreator()->getRootSceneNode()->createChildSceneNode("nKnot");
	entity_Knot = sceneNode->getCreator()->createEntity("entKnot", "knot.mesh");
	entity_Knot->getSubEntity(0)->setMaterialName("knot_fly", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	setEntMan(entity_Knot);

	sceneNode->setPosition(2, 4, -2); //Por ejemplo
	sceneNode->scale(0.01, 0.01, 0.01); //Antes habia un set scale
	//sceneNode_->attachObject(entity_Knot); // Examples.material -> "2 – Default " -> MtlPlat2.jpg

	entity_Knot->setQueryFlags(-1); //Seleccionable aunque no haga nada (De momento)

}


KnotMan::~KnotMan()
{
}

void KnotMan::frameRendered(const Ogre::FrameEvent& event) {
	//LA gilipollez de que de vueltas. 
	sceneNode_->rotate(Vector3(0.0f, 1.0f, 0.0f), Radian(0.1f));
};