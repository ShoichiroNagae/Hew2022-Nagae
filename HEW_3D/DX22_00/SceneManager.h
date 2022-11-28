#pragma once
#include "Scene_Base.h"

class SceneManager
{
public:
	enum class SCENE
	{
		TITLE,
		GAME,
		CLEAR,
		SCENENUM
	};

	static void ChangeScene(SCENE scene);
	static void Update();
	static void Draw();
private:
	static Scene_Base* pScene;
};

