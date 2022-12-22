#include "UnifyVec.h"

void UnifyVec::Vector_DrawAll(GameObjVector vec)
{
	for (auto obj : vec) {
		obj->Draw();
	}
}

void UnifyVec::Vector_UpdateAll(GameObjVector vec)
{
	for (auto obj : vec) {
		obj->Update();
	}
}

void UnifyVec::Vector_DeleteAll(GameObjVector vec)
{
	for (auto obj : vec) {
		delete obj;
	}
}
