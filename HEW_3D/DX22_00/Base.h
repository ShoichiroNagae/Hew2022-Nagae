#pragma once

class SceneManager;

// シーン管理の基底クラス
// (純粋仮想)
class Base
{
protected:
	SceneManager& _sceneManager;
public:
	void Draw(void);
	void Update(void);
};

