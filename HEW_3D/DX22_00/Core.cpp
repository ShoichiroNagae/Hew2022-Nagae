#include "Core.h"
#include <iostream>

void Game::Core::Update(void)
{
	// �X�^�b�N�̈�ԏ���m�F
	if (sceneStack.top()->WantToTransition()){
		std::unique_ptr<Game::Scene::Base>&& nextScene
			= sceneStack.top()->TransitionNextScene();
	}
}

void Game::Core::Draw(void)
{
}
