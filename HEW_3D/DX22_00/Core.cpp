#include "Core.h"
#include <iostream>

void Game::Core::Update(void)
{
	// スタックの一番上を確認
	if (sceneStack.top()->WantToTransition()){
		std::unique_ptr<Game::Scene::Base>&& nextScene
			= sceneStack.top()->TransitionNextScene();
	}
}

void Game::Core::Draw(void)
{
}
