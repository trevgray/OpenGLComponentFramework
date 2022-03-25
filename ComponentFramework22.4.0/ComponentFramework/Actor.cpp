#include "Actor.h"
Actor::Actor(Component* parent_):Component(parent_) {}

Actor::~Actor() {
	OnDestroy();
}

void Actor::OnDestroy() {
	RemoveAllComponents();
	isCreated = false;
}

bool Actor::OnCreate() {
	if (isCreated) return isCreated;
	for (auto component : components) {
		if (component->OnCreate() == false) {
			/*** Error detection needed ***/
			isCreated = false;
			return isCreated;
		}
	}
	isCreated = true;
	return isCreated;
}

void Actor::Update(const float deltaTime) {
	
}
void Actor::Render()const {}

Matrix4 Actor::GetModelMatrix() {
	TransformComponent* transform = GetComponent<TransformComponent>();
	if (transform) {
		modelMatrix = transform->GetTransformMatrix();
	}
	else {
		modelMatrix.loadIdentity();
	}
	if (parent) {
		modelMatrix = dynamic_cast<Actor*>(parent)->GetModelMatrix() * modelMatrix;
	}
	return modelMatrix;
}

void Actor::RemoveAllComponents() {
	for (auto currentComponent : components) {
		currentComponent->OnDestroy();
		delete currentComponent;
	}
	components.clear(); //clear the vector
}

void Actor::ListComponents() const {
	std::cout << typeid(*this).name() << " contains the following components:\n";
	for (auto component : components) {
		std::cout << typeid(*component).name() << std::endl;
	}
	std::cout << '\n';
}