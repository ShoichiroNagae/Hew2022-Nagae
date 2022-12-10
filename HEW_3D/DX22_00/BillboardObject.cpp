#include "BillboardObject.h"
#include "BillboardModel.h"

// ビルボード機能付きのGameObjectクラス

BillboardObject::BillboardObject()
{
	// ビルボード機能ありのModelクラスをnewする
	mModel = new BillboardModel();
}
