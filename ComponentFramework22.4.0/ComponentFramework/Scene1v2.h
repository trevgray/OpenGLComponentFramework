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
	CameraActor* camera;
	LightActor* light;
	Actor* checkerBoard;
	int nextRow;
	float RowX, RowY;
public:
	explicit Scene1v2();
	virtual ~Scene1v2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};
#endif // SCENE0_H