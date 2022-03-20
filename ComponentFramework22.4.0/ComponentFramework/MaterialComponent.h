#pragma once
#include "Component.h"
#include <glew.h>
class MaterialComponent: public Component {
	MaterialComponent(const MaterialComponent&) = delete;
	MaterialComponent(MaterialComponent&&) = delete;
	MaterialComponent& operator = (const MaterialComponent&) = delete;
	MaterialComponent& operator = (MaterialComponent&&) = delete;
public:
	MaterialComponent(Component* parent_, const char* filename_);
	virtual ~MaterialComponent();
	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render()const;

	inline GLuint getTextureID() const { return textureID; }
private:
	bool LoadImage(const char* filename);
	GLuint textureID;
	const char* filename;
};