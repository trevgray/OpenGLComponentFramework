//#include <iostream>
#include "MouseMoveableComponent.h"
#include "QMath.h"
#include "MMath.h"
#include <glew.h>
using namespace MATH;
MouseMoveableComponent::MouseMoveableComponent(Component* parent_):Component(parent_) {
	setWindowDimensions();
}


MouseMoveableComponent::~MouseMoveableComponent() {}

bool MouseMoveableComponent::OnCreate() {
	return true;
}
void MouseMoveableComponent::OnDestroy() {}

void MouseMoveableComponent::Update(const float deltaTime) {

}
void MouseMoveableComponent::Render()const {}

void MouseMoveableComponent::setWindowDimensions() {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	invNDC = MMath::inverse(MMath::viewportNDC(viewport[2], viewport[3]));
}

Vec3 MouseMoveableComponent::getMouseVector(int x, int y, Matrix4 projectionMatrix, Matrix4 viewMatrix) {
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	float x2 = (2.0f * x) / viewport[2] - 1.0f; //750 width
	float y2 = 1.0f - (2.0f * y) / viewport[3]; //1250 height
	float z2 = 1.0f;
	Vec3 ray_nds = Vec3(x2, y2, z2);
	Vec4 ray_clip = Vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	Vec4 ray_eye = MMath::inverse(projectionMatrix) * ray_clip;
	ray_eye = Vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	Vec3 ray_wor = (MMath::inverse(viewMatrix) * ray_eye);
	// don't forget to normalise the vector at some point
	ray_wor = VMath::normalize(ray_wor);
	return ray_nds;
}