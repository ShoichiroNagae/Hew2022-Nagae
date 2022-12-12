#pragma once
#include "Camera.h"
#include "GameObject.h"
class BackCamera :
    public Camera
{
public:
	// ’Ç]ˆ—‚ğÀs
	void Update() override;

	void SetTarget(GameObject* pTarget);

private:
	// ’Ç]‘ÎÛ
	GameObject* mTarget;
};

