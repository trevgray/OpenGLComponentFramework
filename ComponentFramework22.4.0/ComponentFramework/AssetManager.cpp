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

void AssetManager::BuildSceneAssets(const char* XMLFile_, const char* SceneName_) {
	tinyxml2::XMLDocument XMLFile;
	XMLFile.LoadFile(XMLFile_); //loading XML file
	if (XMLFile.Error()) { //Error detection in the xml
		std::cout << XMLFile.ErrorIDToName(XMLFile.ErrorID()) << std::endl;
		return;
	}
	tinyxml2::XMLElement* sceneRoot = XMLFile.RootElement()->FirstChildElement(SceneName_); //getting root of the scene
	tinyxml2::XMLElement* currentElement = sceneRoot->FirstChildElement("Component"); //loading first component
	bool componentLoop = true;
	while (componentLoop) {
		//component loop
		std::string componentType = currentElement->FirstChildElement("Type")->Attribute("type");
		if (componentType == "Mesh") { //create mesh component
			std::string name = currentElement->Attribute("name");
			std::string filename = currentElement->FirstChildElement("Mesh")->Attribute("filename");
			AddComponent<MeshComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Mesh")->Attribute("filename"));
		}
		else if (componentType == "Material") { //create material component
			AddComponent<MaterialComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Material")->Attribute("filename"));
		}
		else if (componentType == "Shader") { //create shader component
			AddComponent<ShaderComponent>(currentElement->Attribute("name"), nullptr, currentElement->FirstChildElement("Shader")->Attribute("vertFileName"), currentElement->FirstChildElement("Shader")->Attribute("fragFileName"));
		}
		if (currentElement == sceneRoot->LastChild()) { //stopping looping when the current element is the last element
			componentLoop = false;
		}
		currentElement = currentElement->NextSiblingElement(); //loading the next component
	}
}
