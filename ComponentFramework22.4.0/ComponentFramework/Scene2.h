#pragma once
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "LightActor.h"
#include "CameraActor.h"
#include "Actor.h"
#include "AssetManager.h"
using namespace MATH;

/// Forward declarations 
union SDL_Event;

class Scene2 : public Scene { //inherited from actor, but i'm not really using any of actor's functions
private:
	int nextRow;
	float RowX, RowY;
	AssetManager* assetManager;
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event &sdlEvent);
};