#include <glew.h>
#include <iostream>
#include <SDL.h>
#include "Debug.h"
#include "Scene2.h"
#include "MMath.h"
#include "Debug.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "QMath.h"

Scene2::Scene2(): RowX(0), RowY(0), nextRow(0), assetManager(nullptr) {
	Debug::Info("Created Scene2: ", __FILE__, __LINE__);
}

Scene2::~Scene2() {
	Debug::Info("Deleted Scene2: ", __FILE__, __LINE__);
	OnDestroy();
}

bool Scene2::OnCreate() {
	Debug::Info("Loading assets Scene2: ", __FILE__, __LINE__);
	//assetManager
	assetManager = new AssetManager();
	assetManager->AddComponent<ShaderComponent>("TextureShader", nullptr, "shaders/textureVert.glsl", "shaders/textureFrag.glsl"); //im not sure this is how you are suppose to use the assetManager, but it works at not creating 32 checker meshes in memory
	assetManager->AddComponent<MeshComponent>("PlaneMesh", nullptr, "meshes/Plane.obj");
	assetManager->AddComponent<MeshComponent>("CheckerPieceMesh", nullptr, "meshes/CheckerPiece.obj");
	assetManager->AddComponent<MaterialComponent>("RedCheckerTexture", nullptr, "textures/redCheckerPiece.png");
	assetManager->AddComponent<MaterialComponent>("BlackCheckerTexture", nullptr, "textures/blackCheckerPiece.png");
	assetManager->AddComponent<MaterialComponent>("CheckerBoardTexture", nullptr, "textures/8x8_checkered_board.png");
	assetManager->OnCreate();
	//camera
	AddComponent<CameraActor>(new CameraActor(nullptr));
	GetComponent<CameraActor>()->AddComponent<TransformComponent>(nullptr,Vec3(0.0f,0.5f,-13.0f), Quaternion());
	GetComponent<CameraActor>()->OnCreate();
	//light
	AddComponent<LightActor>(new LightActor(nullptr)); //there might be an error with your code scott, i have to make a constructor with nothing in add it, idk why
	GetComponent<LightActor>()->OnCreate();
	//checkerboard
	AddComponent<Actor>(new Actor(nullptr));
	GetComponent<Actor>(2)->AddComponent<TransformComponent>(nullptr, Vec3(0.0f, 0.0f, 0.0f), Quaternion(0.95f, -0.31f, 0.0f, 0.0f), Vec3(1.0f,1.0f,1.0f));
	GetComponent<Actor>(2)->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("PlaneMesh"));
	GetComponent<Actor>(2)->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("CheckerBoardTexture"));
	GetComponent<Actor>(2)->OnCreate(); //The checkerboard is the 3rd Actor in the Scene

	//Red Checker creation loop
	RowX = RowY = nextRow = 0.0f;
	for (int x = 3; x <= 14; x++) {
		AddComponent<Actor>(new Actor(GetComponent<Actor>(2).get()));
		GetComponent<Actor>(x)->AddComponent<TransformComponent>(nullptr, Vec3(-4.5 + RowX, -4.3 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetComponent<Actor>(x)->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("CheckerPieceMesh"));
		GetComponent<Actor>(x)->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("RedCheckerTexture"));
		GetComponent<Actor>(x)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0.0f;
			RowY += 1.26f;
			if (RowY == 1.26f) {
				RowX = 1.27f;
			}
		}
	}
	//Black Checker creation loop
	RowX = RowY = nextRow = 0.0f;
	for (int x = 15; x <= 26; x++) {
		AddComponent<Actor>(new Actor(GetComponent<Actor>(2).get()));
		GetComponent<Actor>(x)->AddComponent<TransformComponent>(nullptr, Vec3(-3.225 + RowX, 4.4 + RowY, 0.0f), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(0.14f, 0.14f, 0.14f));
		GetComponent<Actor>(x)->AddComponent<MeshComponent>(assetManager->GetComponent<MeshComponent>("CheckerPieceMesh"));
		GetComponent<Actor>(x)->AddComponent<MaterialComponent>(assetManager->GetComponent<MaterialComponent>("BlackCheckerTexture"));
		GetComponent<Actor>(x)->OnCreate();
		RowX += 2.55f;
		nextRow++;
		if (nextRow == 4) {
			RowX = nextRow = 0.0f;
			RowY -= 1.26f;
			if (RowY == -1.26f) {
				RowX = -1.27f;
			}
		}
	}
	return true;
}

void Scene2::OnDestroy() {
	Debug::Info("Deleting assets Scene2: ", __FILE__, __LINE__);
	delete assetManager;
	assetManager = nullptr;
}

void Scene2::HandleEvents(const SDL_Event &sdlEvent) {
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

void Scene2::Update(const float deltaTime) {
	
}

void Scene2::Render() const {
	glEnable(GL_DEPTH_TEST);
	/// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_UNIFORM_BUFFER, GetComponent<CameraActor>()->GetMatriciesID());
	glBindBuffer(GL_UNIFORM_BUFFER, GetComponent<LightActor>()->GetLightID());

	glUseProgram(assetManager->GetComponent<ShaderComponent>("TextureShader")->GetProgram());
	for (int x = 0; x <= GetComponentVectorSize() - 1; x++) { //for (auto actor : actorGraph) //for each could also work - but the actorGraph is protected so doing a for loop is better in my opinion
		glUniformMatrix4fv(assetManager->GetComponent<ShaderComponent>("TextureShader")->GetUniformID("modelMatrix"), 1, GL_FALSE, GetComponent<Actor>(x)->GetModelMatrix());
		if (GetComponent<Actor>(x)->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
			glBindTexture(GL_TEXTURE_2D, GetComponent<Actor>(x)->GetComponent<MaterialComponent>()->getTextureID()); //this is also amazing because we can add as many actors as we want, and the render does not need to change
			GetComponent<Actor>(x)->GetComponent<MeshComponent>()->Render(GL_TRIANGLES);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}