#include <memory>
#include "AssetManager.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "tinyxml2.h"

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {
	RemoveAllComponents();
}

bool AssetManager::OnCreate() {
	for (std::pair<std::string, Ref<Component>> c : componentGraph) {
		if (c.second->OnCreate() == false) {
#ifdef _DEBUG
			Debug::Error("Component failed to create", __FILE__, __LINE__);
#endif
			return false;
		}
	}
	return true;
}

void AssetManager::RemoveAllComponents() {
	componentGraph.clear();
}

void AssetManager::BuildSceneAssets(std::string XMLFile_, std::string SceneName_) {
	tinyxml2::XMLDocument XMLFile;
	XMLFile.LoadFile(XMLFile_.c_str()); //loading XML file
	if (XMLFile.Error()) { //Error detection in the xml
		std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
		return;
	}
	tinyxml2::XMLElement* sceneRoot = XMLFile.RootElement()->FirstChildElement(SceneName_.c_str()); //getting root of the scene
	tinyxml2::XMLElement* currentElement;
	//Component Loop - in Scene Scope
	currentElement = sceneRoot->FirstChildElement("Component"); //loading first component from Scene Scope
	bool componentLoop = true;
	while (componentLoop) {
		std::string componentCheck = currentElement->Name();
		if (componentCheck == "Component") { //check if the element is a component
		//component loop
			std::string componentType = currentElement->FirstChildElement("Type")->Attribute("type");
			if (componentType == "Mesh") { //create mesh component
				AddComponent<MeshComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Mesh")->Attribute("filename"));
			}
			else if (componentType == "Material") { //create material component
				AddComponent<MaterialComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Material")->Attribute("filename"));
			}
			else if (componentType == "Shader") { //create shader component
				AddComponent<ShaderComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Shader")->Attribute("vertFileName"), currentElement->FirstChildElement("Shader")->Attribute("fragFileName"));
			}
			if (currentElement == sceneRoot->LastChildElement("Component")) { //stopping looping when the current element is the last element in Scene Scope - sceneRoot->LastChild() will also work, but stopping at the last component should be faster
				componentLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next component
	}
	//Actor Loop
	currentElement = sceneRoot->FirstChildElement("Actor"); //find first actor = currentElement 
	tinyxml2::XMLElement* currentComponent;
	bool actorLoop = true;
	while (actorLoop) {
		std::string actorCheck = currentElement->Name();
		if (actorCheck == "Actor") { //check if the element is a actor
			AddComponent<Actor>(currentElement->Attribute("name"), nullptr); //make the actor
			currentComponent = currentElement->FirstChildElement("Mesh"); //get the FirstChildElement component in currentElement
			GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<MeshComponent>(GetComponent<MeshComponent>(currentComponent->Attribute("name"))); //set the mesh to a component made in the first loop
			currentComponent = currentElement->FirstChildElement("Material");
			GetComponent<Actor>(currentElement->Attribute("name"))->AddComponent<MaterialComponent>(GetComponent<MaterialComponent>(currentComponent->Attribute("name"))); //set the material to a component made in the first loop
			if (currentElement == sceneRoot->LastChildElement("Actor")) { //exit when component is = to the LastChildElement in currentComponent
				actorLoop = false;
			}
		}
		currentElement = currentElement->NextSiblingElement(); //load next actor
	}
}
