#pragma once

#include <initializer_list>
#include <compare>
#include <stack>

#include "Base.h"

namespace Game {
	class Core
	{
	private:
		// シーン管理スタック
		std::stack<Game::Scene::Base*> sceneStack;
	public:
		void Update(void);
		void Draw(void);
	};
}
