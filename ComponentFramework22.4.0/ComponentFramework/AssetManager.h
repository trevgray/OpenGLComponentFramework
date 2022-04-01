#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include "Component.h"
#include "Debug.h"

class AssetManager {
private:
	std::unordered_map <const char*, Ref<Component>> componentGraph;
public:
	AssetManager();
	~AssetManager();
	bool OnCreate();
	void RemoveAllComponents();
	//AddComponent
	//GetComponent
};

