#pragma once

class SceneManager;

// �V�[���Ǘ��̊��N���X
// (�������z)
class Base
{
protected:
	SceneManager& _sceneManager;
public:
	void Draw(void);
	void Update(void);
};

