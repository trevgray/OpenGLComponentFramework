#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene1.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "MouseMoveableComponent.h"
#include "QMath.h"

Scene1::Scene1(): Actor(nullptr), camera(nullptr), checkerBoard(nullptr), light(nullptr), RowX(0), RowY(0), nextRow(0) {
	Debug::Info("Created Scene0: ", __FILE__, __LINE__);
}

Scene1::~Scene1() {
	Debug::Info("Deleted Scene0: ", __FILE__, __LINE__);
	
}

bool Scene1::OnCreate() {
	Debug::Info("Loading assets Scene0: ", __FILE__, __LINE__);
	AddComponent<CameraActor>(camera = new CameraActor(nullptr));
	camera->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.0f,-12.0f), Quaternion());
	camera->OnCreate();
	//AddComponent<Actor>(light = new LightActor(nullptr, LightStyle::DirectionLight, Vec3(0.0f,10.0f,0.0f), Vec4(0.8f,0.8f,0.8f,0.0f)));
	light = new LightActor(nullptr, LightStyle::DirectionLight, Vec3(0.0f, 10.0f, 0.0f), Vec4(0.8f, 0.8f, 0.8f, 0.0f));
	light->OnCreate();
	//AddComponent<Actor>(checkerBoard = new Actor(nullptr)); //0.87f, -0.5f, 0.0f, 0.0f
	checkerBoard = new Actor(nullptr);
	checkerBoard->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f,1.0f,1.0f));
	checkerBoard->AddComponent<MeshComponent>(nullptr, "meshes/Plane.obj");
	checkerBoard->AddComponent<ShaderComponent>(nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl");
	checkerBoard->AddComponent<MaterialComponent>(nullptr, "textures/8x8_checkered_board.png");
	checkerBoard->OnCreate();

	//Red Checker creation loop
	RowX = RowY = nextRow = 0;

	for (int x = 0; x <= 11; x++) {
		checkerRedList.push_back(new Actor(checkerBoard));
		checkerRedList[x]->AddComponent<TransformComponent>(nullptr, Vec3(-4.5 + RowX, -4.3 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		checkerRedList[x]->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		checkerRedList[x]->AddComponent<MaterialComponent>(nullptr, "textures/redCheckerPiece.png"); //think about removing these
		checkerRedList[x]->AddComponent<MouseMoveableComponent>(nullptr);
		checkerRedList[x]->OnCreate();
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
	for (int x = 0; x <= 11; x++) {
		checkerBlackList.push_back(new Actor(checkerBoard));
		checkerBlackList[x]->AddComponent<TransformComponent>(nullptr, Vec3(-3.225 + RowX, 4.4 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		checkerBlackList[x]->AddComponent<MeshComponent>(nullptr, "meshes/CheckerPiece.obj"); //think about removing these
		checkerBlackList[x]->AddComponent<MaterialComponent>(nullptr, "textures/blackCheckerPiece.png"); //think about removing these
		checkerBlackList[x]->OnCreate();
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

void Scene1::OnDestroy() {
	Debug::Info("Deleting assets Scene0: ", __FILE__, __LINE__);
	if (camera) delete camera;
	if (checkerBoard) delete checkerBoard;
	if (light) delete light;
}

void Scene1::HandleEvents(const SDL_Event &sdlEvent) {
	switch( sdlEvent.type ) {
    case SDL_KEYDOWN:
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(1.0, 0.0, 0.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(-1.0, 0.0, 0.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_UP) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, 1.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			camera->GetComponent<TransformComponent>()->SetPosition(camera->GetComponent<TransformComponent>()->GetPosition() + Vec3(0.0, 0.0, -1.0));
			camera->UpdateViewMatrix();
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_E) {
			checkerBoard->GetComponent<TransformComponent>()->SetTransform(checkerBoard->GetComponent<TransformComponent>()->GetPosition(), checkerBoard->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(-2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
			checkerBoard->GetComponent<TransformComponent>()->SetTransform(checkerBoard->GetComponent<TransformComponent>()->GetPosition(), checkerBoard->GetComponent<TransformComponent>()->GetQuaternion() * QMath::angleAxisRotation(2.0f, Vec3(0.0f, 1.0f, 0.0f)));
		}
		break;

	case SDL_MOUSEMOTION:
		checkerRedList[0]->GetComponent<TransformComponent>()->SetPosition(checkerRedList[0]->GetComponent<MouseMoveableComponent>()->getMouseVector(sdlEvent.button.x, sdlEvent.button.y, checkerRedList[0], camera->GetProjectionMatrix()));
		//checkerRedList[0]->GetComponent<MouseMoveableComponent>()->getMouseVector(sdlEvent.button.x, sdlEvent.button.y, checkerRedList[0]);
		//std::cout << sdlEvent.motion.x << " " << sdlEvent.motion.y << std::endl;
		break;

	case SDL_MOUSEBUTTONDOWN:    
		//checkerRedList[0]->GetComponent<MouseMoveableComponent>()->SetMousePos2(sdlEvent.button.x, sdlEvent.button.y);
		break; 

	case SDL_MOUSEBUTTONUP:            
	break;

	default:
		break;
    }
}

void Scene1::Update(const float deltaTime) {
	
}

void Scene1::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderComponent* shader = checkerBoard->GetComponent<ShaderComponent>();
	MeshComponent* mesh = checkerBoard->GetComponent<MeshComponent>();
	MaterialComponent* texture = checkerBoard->GetComponent<MaterialComponent>();
	if (shader == nullptr || mesh == nullptr || texture == nullptr) {
		return;
	}
	glUseProgram(shader->GetProgram());
	glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBoard->GetModelMatrix());
	glBindBuffer(GL_UNIFORM_BUFFER, GetComponent<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, light->GetLightID());//GetComponent<LightActor>()->GetLightID());
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	mesh->Render(GL_TRIANGLES);

	for (int x = 0; x <= checkerRedList.size() - 1; x++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerRedList[x]->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, checkerRedList[x]->GetComponent<MaterialComponent>()->getTextureID());
		checkerRedList[x]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	for (int x = 0; x <= checkerBlackList.size() - 1; x++) {
		glUniformMatrix4fv(shader->GetUniformID("modelMatrix"), 1, GL_FALSE, checkerBlackList[x]->GetModelMatrix());
		glBindTexture(GL_TEXTURE_2D, checkerBlackList[x]->GetComponent<MaterialComponent>()->getTextureID());
		checkerBlackList[x]->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}