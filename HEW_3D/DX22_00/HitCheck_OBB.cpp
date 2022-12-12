#include "HitCheck_OBB.h"

void HitCheck_OBB::CreateOBB(OBB* obb)
{
	XMFLOAT4X4 matRot;

	// �ő�l�A�ŏ��l�̏����l�ݒ�
	XMFLOAT3 max = XMFLOAT3(-10000.0f, -10000.0f, -10000.0f);
	XMFLOAT3 min = XMFLOAT3(+10000.0f, +10000.0f, +10000.0f);

	// ���b�V���̒��_�f�[�^�擾
	VERTEX* vertexBuffer = NULL;
	//m_pMesh->LockVertexBuffer(0, (void**)&vertexBuffer);

	//// �ő�l�A�ŏ��l�擾���[�v
	//for (int i = 0; i < /*g_pMesh->GetNumVertices()*/; i++)
	//{
	//	XMFLOAT3 pos = &vertexBuffer[i].position;
	//	if (pos.x < min.x)min.x = pos.x;
	//	if (pos.x > max.x)max.x = pos.x;
	//	if (pos.y < min.y)min.y = pos.y;
	//	if (pos.y > max.y)max.y = pos.y;
	//	if (pos.z < min.z)min.z = pos.z;
	//	if (pos.z > max.z)max.z = pos.z;
	//}
	//g_pMesh->UnlockVertexBuffer();

	//// ���S�_�擾
	//obb->c = (min + max) * 0.5f + g_worldPos;

	//// �����x�N�g���擾
	//D3DXMatrixRotationYawPitchRoll(&matRot, g_angleY, g_angleX, g_angleZ);
	//obb->u[0] = D3DXVECTOR3(matRot._11, matRot._12, matRot._13);
	//obb->u[1] = D3DXVECTOR3(matRot._21, matRot._22, matRot._23);
	//obb->u[2] = D3DXVECTOR3(matRot._31, matRot._32, matRot._33);

	// �����擾
	obb->e.x = fabsf(max.x - min.x) * 0.5f;
	obb->e.y = fabsf(max.y - min.y) * 0.5f;
	obb->e.z = fabsf(max.z - min.z) * 0.5f;
}
