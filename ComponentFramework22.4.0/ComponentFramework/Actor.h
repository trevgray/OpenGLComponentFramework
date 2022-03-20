#pragma once
#include <vector> 
#include <iostream>
#include "Component.h"
#include "TransformComponent.h"
class Actor: public Component {
	/// Unless you know what these do don't allow them
	/// to be created implicitly 
	Actor(const Actor&) = delete;
	Actor(Actor&&) = delete;
	Actor& operator=(const Actor&) = delete;
	Actor& operator=(Actor&&) = delete;

private:
	std::vector<Component*> components;
	Matrix4 modelMatrix;
public:
	Actor(Component* parent_);
	~Actor();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	
	/// Footnote to those who think you can't write code in the header file - this is true
	/// with a few exceptions. (1) You can't inline code (implicitly or not) unless it is in 
	/// the header file and (2) templates must be in the header file
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

		/// This part is tricky, 
		/// before you add the component ask if you have the component in the list already,
		/// if so - don't add a second one. 
		if (GetComponent<ComponentTemplate>()) {
			///Trying to add a component type that is already added
#ifdef _DEBUG
			std::cerr << "WARNING: Trying to add a component type that is already added - ignored\n";
#endif
			delete componentObject;
			componentObject = nullptr;
			return;
		}
		/// If nothing else is messed up, finish building the component and
		/// add the component to the list
		components.push_back(componentObject);
		//componentObject
	}

	template<typename ComponentTemplate>
	ComponentTemplate* GetComponent() const {
		for (auto component : components) {
			if (dynamic_cast<ComponentTemplate*>(component)) {
				return dynamic_cast<ComponentTemplate*>(component);
			}
		}
		return nullptr;
	}

	template<typename ComponentTemplate>
	void RemoveComponent() {
		for (unsigned int i = 0; i < components.size(); i++) {
			if (dynamic_cast<ComponentTemplate*>(components[i]) != nullptr) {
				components[i]->OnDestroy();
				delete components[i];
				components[i] = nullptr;
				///This removes the component from the vector list
				components.erase(components.begin() + i);
				break;
			}
		}
	}
	void RemoveAllComponents();
	void ListComponents() const;
	Matrix4 GetModelMatrix();
};

