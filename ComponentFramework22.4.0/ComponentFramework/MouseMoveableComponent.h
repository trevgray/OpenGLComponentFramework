#pragma once
#include "Component.h"
#include "Matrix.h"
using namespace MATH;
class MouseMoveableComponent : public Component {
private:
	Matrix4 invNDC;			/// the inverse of the viewportNDC matrix
public:
	MouseMoveableComponent(Component* parent_);
	~MouseMoveableComponent();
	bool OnCreate();
	void OnDestroy();
	void Update(const float deltaTime_);
	void Render() const;

	void setWindowDimensions();

	Vec3 getMouseVector(int x, int y, Matrix4 projectionMatrix, Matrix4 viewMatrix);


};

