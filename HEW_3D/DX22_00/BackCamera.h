#pragma once
#include "Camera.h"
#include "GameObject.h"
class BackCamera :
    public Camera
{
public:
	// �Ǐ]���������s
	void Update() override;

	void SetTarget(GameObject* pTarget);

private:
	// �Ǐ]�Ώ�
	GameObject* mTarget;
};

