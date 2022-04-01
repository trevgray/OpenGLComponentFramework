#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1v2.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

Scene1v2::Scene1v2(): Actor(nullptr), RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene1v2::~Scene1v2() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
	
}

bool Scene1v2::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	//camera
	AddComponent<CameraActor>(new CameraActor(nullptr));
	GetComponent<CameraActor>()->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.0f,-12.0f), Quaternion());
	GetComponent<CameraActor>()->OnCreate();
	//light
	AddComponent<LightActor>(new LightActor(nullptr)); //there might be an error with your code scott, i have to make a constructor with nothing in add it, idk why
	GetComponent<LightActor>()->OnCreate();
	//checkerboard
	AddComponent<Actor>(new Actor(nullptr));
	GetComponent<Actor>(2)->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,1.0f));
	GetComponent<Actor>(2)->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	GetComponent<Actor>(2)->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	GetComponent<Actor>(2)->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	GetComponent<Actor>(2)->OnCreate(); //The checkerboard is the 3rd Actor in the Scene

	//Red Checker creation loop
	RowX = RowY = nextRow = 0;
	for (int x = 3; x <= 14; x++) {
		AddComponent<Actor>(new Actor(GetComponent<Actor>(2)));
		GetComponent<Actor>(x)->AddComponent<TransformComponent>(nullptr, Vec3(-4.5 + RowX, -4.3 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetComponent<Actor>(x)->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		GetComponent<Actor>(x)->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png"); //think about removing these
		GetComponent<Actor>(x)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0;
			RowY += 1.26f;
			if (RowY == 1.26f) {
				RowX = 1.27f;
			}
		}
	}
	//Black Checker creation loop
	RowX = RowY = nextRow = 0;
	for (int x = 15; x <= 27; x++) {
		AddComponent<Actor>(new Actor(GetComponent<Actor>(2)));
		GetComponent<Actor>(x)->AddComponent<TransformComponent>(nullptr, Vec3(-3.225 + RowX, 4.4 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetComponent<Actor>(x)->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		GetComponent<Actor>(x)->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png"); //think about removing these
		GetComponent<Actor>(x)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0;
			RowY -= 1.26f;
			if (RowY == -1.26f) {
				RowX = -1.27f;
			}
		}
	}
	return true;
}

void Scene1v2::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
}

void Scene1v2::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			GetComponent<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetComponent<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
			GetComponent<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			GetComponent<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetComponent<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
			GetComponent<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			GetComponent<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetComponent<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
			GetComponent<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			GetComponent<CameraActor>()->GetComponent<TransformComponent>()->SetPosition(GetComponent<CameraActor>()->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
			GetComponent<CameraActor>()->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			GetComponent<Actor>(2)->GetComponent<TransformComponent>()->SetTransform(GetComponent<Actor>(2)->GetComponent<TransformComponent>()->GetPosition(), GetComponent<Actor>(2)->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			GetComponent<Actor>(2)->GetComponent<TransformComponent>()->SetTransform(GetComponent<Actor>(2)->GetComponent<TransformComponent>()->GetPosition(), GetComponent<Actor>(2)->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		break;

	case SDL_MOUSEMOTION:          
		break;

	case SDL_MOUSEBUTTONDOWN:              
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene1v2::Update(const float deltaTime) {
	
}

void Scene1v2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(GetComponent<Actor>(2)->GetComponent<ShaderComponent>()->GetProgram());
	glBindBuffer(GL_UNIFORM_BUFFER, GetComponent<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetComponent<LightActor>()->GetLightID());

	for (int x = 2; x <= components.size() - 2; x++) { //-2 because the first 2 components are the camera and lighr actor - a smarter system is probably better like checking if the component is an actor
		glUniformMatrix4fv(GetComponent<Actor>(2)->GetComponent<ShaderComponent>()->GetUniformID("modelMatrix"), 1, GL_FALSE, GetComponent<Actor>(x)->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, GetComponent<Actor>(x)->GetComponent<MaterialComponent>()->getTextureID());
		GetComponent<Actor>(x)->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}