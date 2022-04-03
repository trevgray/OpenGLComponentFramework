#ifndef SCENE_H
#define SCENE_H

#include "Actor.h"

union SDL_Event;

class Scene{
private:
	std::vector<std::shared_ptr<Component>> components;
public:	
	virtual ~Scene() = default;
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event &sdlEvent) = 0;
	
	int GetComponentVectorSize() const { return components.size(); }

	template<typename ComponentTemplate, typename ... Args> void AddComponent(Args&& ... args_) {
		components.push_back(std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...));
	}

	template<typename ComponentTemplate> void AddComponent(Ref<ComponentTemplate> component_) {
		components.push_back(component_);
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent() const {
		for (auto c : components) {
			if (dynamic_cast<ComponentTemplate*>(c.get())) {
				//https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast dynamic cast designed for shared_ptr's
				return std::dynamic_pointer_cast<ComponentTemplate>(c);
			}
		}
		return Ref<ComponentTemplate>(nullptr);
	}

	template<typename ComponentTemplate> Ref<ComponentTemplate> GetComponent(int objectNum) const {
		if (dynamic_cast<ComponentTemplate*>(components[objectNum].get())) { //check if it is the type we want
			return std::dynamic_pointer_cast<ComponentTemplate>(components[objectNum]);
		}
		return nullptr;
	}
};
#endif