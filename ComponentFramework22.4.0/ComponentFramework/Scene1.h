#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene1 : public Scene, public Actor {
private:
	CameraActor* camera;
	LightActor* light;
	Actor* checkerBoard;
	std::vector<Actor*> checkerRedList, checkerBlackList;
	unsigned char data[4];
	int pickedID;
	int nextRow;
	float RowX, RowY;
public:
	explicit Scene1();
	virtual ~Scene1();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};
#endif // SCENE0_H