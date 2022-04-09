#ifndef SCENE_H
#define SCENE_H

#include "Actor.h"
#include <unordered_map>

union SDL_Event;

class Scene{
private:
	//std::vector<Ref<Component>> actors; //make an unordered map
	//std::unordered_map <const char*, Ref<Component>> actorGraph;
public:	
	std::unordered_map <const char*, Ref<Actor>> actorGraph;
	virtual ~Scene() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	
	int GetComponentVectorSize() const { return actorGraph.size(); }

	template<typename ComponentTemplate, typename ... Args> void AddComponent(const char* name, Args&& ... args_) { //rename to AddActor - also use actortemplate
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		actorGraph[name] = t;
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent() const {
		for (auto actor : actorGraph) {
			if (dynamic_cast<ComponentTemplate*>(actor.second.get())) {
				//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
				return std::dynamic_pointer_cast<ComponentTemplate>(actor.second);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(const char* name) {
		auto id = actorGraph.find(name);
#ifdef _DEBUG
		if (id == actorGraph.end()) {
			Debug::Error("Can't find requested component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}

	//template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const { //old version
	//	if (dynamic_cast<ComponentTemplate*>(actorGraph[objectNum].get())) { //check if it is the type we want
	//		return std::dynamic_pointer_cast<ComponentTemplate>(actorGraph[objectNum]);
	//	}
	//	return nullptr;
	//}
};
#endif