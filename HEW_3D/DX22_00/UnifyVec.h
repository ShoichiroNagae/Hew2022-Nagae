#pragma once
#include <vector>
#include <map>
#include <string>

#include "GameObject.h"
#include "ModelData.h"

typedef std::vector<GameObject*> GameObjVector;

// �Q�[�����Ŏg�p����Vector���܂Ƃ߂��N���X
class UnifyVec
{
public:
	// GameObject�̕`��C�X�V�C���
	void Vector_DrawAll(GameObjVector vec);
	void Vector_UpdateAll(GameObjVector vec);
	void Vector_DeleteAll(GameObjVector vec);
private:
	GameObjVector gObjList;			// �I�u�W�F�N�g���X�g
	GameObjVector gShotManager;		// �e�Ǘ��p�z��
	GameObjVector gEffectManager;	// �G�t�F�N�g�Ǘ��p�z��

	std::map<std::string, ModelData> gModelManager;

};

