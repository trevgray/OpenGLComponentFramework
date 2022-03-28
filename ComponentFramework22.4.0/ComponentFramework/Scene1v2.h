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

class Scene1v2 : public Scene, public Actor {
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

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(Args&& ... args_) {
		/// Create the new object based on the template type and the argument list
		ComponentTemplate* componentObject = new ComponentTemplate(std::forward<Args>(args_)...);

		/// Just so you follow me...
		/// If a dynamic_cast succeeds, it returns a pointer of the new type,
		/// if it fails, it returns a nullptr. Meaning it wasn't inherited from Component
		if (dynamic_cast<Component*>(componentObject) == nullptr) {
			///Trying to add a component that is not a base class of Component class
			/// I don't think the compiler will let this happen anyway
#ifdef _DEBUG
			std::cerr << "WARNING:Trying to add a component that is not a base class of Component class - ignored\n";
#endif
			delete componentObject;
			componentObject = nullptr;
			return;
		}
		/// If nothing else is messed up, finish building the component and
		/// add the component to the list
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
		if (dynamic_cast<ComponentTemplate*>(components[objectNum])) {
				return dynamic_cast<ComponentTemplate*>(components[objectNum]);
		}
		return nullptr;
	}
};
#endif // SCENE0_H