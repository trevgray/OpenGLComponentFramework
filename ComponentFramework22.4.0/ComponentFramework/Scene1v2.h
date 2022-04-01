#ifndef SCENE1V2_H
#define SCENE1V2_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene1v2 : public Scene, public Actor { //inherited from actor, but i'm not really using any of actor's functions
private:
	int nextRow;
	float RowX, RowY;
	std::vector<Component*> components;
public:
	explicit Scene1v2();
	virtual ~Scene1v2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);

	//Overwriting the AddComponent and GetComponent functions in Actor so more than 1 type of Actor can be added to the Scene

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		ComponentTemplate* componentObject = new ComponentTemplate(std::forward<Args>(args_)...);
		if (dynamic_cast<Component*>(componentObject) == nullptr) {
#ifdef _DEBUG
			std::cerr << "WARNING:Trying to add a component that is not a base class of Component class - ignored\n";
#endif
			delete componentObject;
			componentObject = nullptr;
			return;
		}
		components.push_back(componentObject);
	}

	template<typename ComponentTemplate>
	ComponentTemplate* GetComponent() const  {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component)) {
				return dynamic_cast<ComponentTemplate*>(component);
			}
		}
		return nullptr;
	}

	template<typename ComponentTemplate>
	ComponentTemplate* GetComponent(int objectNum) const {
		if (dynamic_cast<ComponentTemplate*>(components[objectNum])) { //check if it is the type we want
				return dynamic_cast<ComponentTemplate*>(components[objectNum]);
		}
		return nullptr;
	}
};
#endif // SCENE0_H