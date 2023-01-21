#include "Model.h"
#include "Direct3D.h"

// 定数バッファ用変数
ID3D11Buffer* gpConstBuffer;

using namespace DirectX; // "DirectX"namespaceを今後省略する
extern DWORD gDeltaTime; // アニメーションで使う

void Model::Draw()
{
	DIRECT3D* d3d = Direct3D_Get();

	// 回転行列
	static float ry;

	// 描画で使用するテクスチャを指定する関数呼び出し
	// ここで指定されたテクスチャが、ピクセルシェーダーのグローバル変数にセットされる
	d3d->context->PSSetShaderResources(0, 1, &mModelData.mSRV);

	// 描画に使う頂点バッファを指定する
	UINT strides = sizeof(VERTEX3D);
	UINT offsets = 0;
	// 第三引数：頂点バッファ
	d3d->context->IASetVertexBuffers(0, 1,
		&mModelData.mVertexBuffer, &strides, &offsets);

	// 定数バッファ更新　→　Ｃ言語からシェーダーにデータを転送する
	D3D11_MAPPED_SUBRESOURCE pData;  // 定数バッファで確保されたメモリ領域の情報が格納される構造体
	// 定数バッファのメモリ領域にアクセスして、データを転送する処理
	// 定数バッファのメモリにアクセスするために、そのメモリ領域にロックをかける

	ConstBufferData cbdata;
	GetWVPRMatrix(cbdata);

	cbdata.uvOffset.x = mUvOffset.x;
	cbdata.uvOffset.y = mUvOffset.y;
	cbdata.diffuse = mDiffuse;

	if (mLightActive)
	{
		cbdata.lightPower = 1.0f;
		cbdata.lightVector = XMFLOAT3(1, 1, -1);
		cbdata.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	}
	else
	{
		cbdata.lightPower = 0.0f;
	}

	HRESULT hr = d3d->context->Map(gpConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		// 1: コピーしたい先のアドレス
		// 2: コピー先の確保されているデータサイズ
		// 3: コピーしたいデータが入っている先頭アドレス
		// 4: コピーしたいデータのサイズ
		const rsize_t dataSize = sizeof(cbdata);
		memcpy_s(pData.pData, pData.RowPitch, &cbdata, dataSize);

		d3d->context->Unmap(gpConstBuffer, 0);  // ロックしたメモリを解除する
	}

	// 定数バッファをシェーダーからアクセスできるようにセットする
	d3d->context->VSSetConstantBuffers(1, 1, &gpConstBuffer);// 頂点シェーダー
	d3d->context->PSSetConstantBuffers(1, 1, &gpConstBuffer);// ピクセルシェーダー

	// これまでの設定で実際に描画する  第一引数：実際に描画する頂点数
	d3d->context->Draw(mModelData.mNumVertex, 0);
}

void Model::SetModelData(ModelData model)
{
	mModelData = model;
}

void Model::ChangeTexData(const wchar_t* pTexFileName)
{
	Direct3D_LoadTexture(pTexFileName, &mModelData.mSRV);
}

void Model::SetScale(float newScale)
{
	mScale.x = newScale;
	mScale.y = newScale;
	mScale.z = newScale;
}

void Model::Setpos(float mx, float my, float mz)
{
	this->mPos.x = mx;
	this->mPos.y = my;
	this->mPos.z = mz;
}

void Model::GetWVPRMatrix(ConstBufferData& out)
{
	XMMATRIX mxRotY = XMMatrixRotationY(
		XMConvertToRadians(mRotate.y));  // Y軸回転
	XMMATRIX mxRotX = XMMatrixRotationX(
		XMConvertToRadians(mRotate.x));
	XMMATRIX mxRotZ = XMMatrixRotationZ(
		XMConvertToRadians(mRotate.z));

	// 平行移動行列
	XMMATRIX mxTrans = XMMatrixTranslation(
		mPos.x, mPos.y, mPos.z);  // 立体の現在位置を表す変数を使って、平行移動の行列を作る

	// 拡大縮小行列
	XMMATRIX mxScale = XMMatrixScaling(
		mScale.x, mScale.y, mScale.z);

	// ワールド変換行列を作成
	XMMATRIX mxRotate = mxRotZ * mxRotX * mxRotY;
	XMMATRIX mxWorld = mxScale * mxRotate * mxTrans;

	XMMATRIX mxView, mxProjection;

	if (m2dRender)
	{
		// 2D描画の場合はビューとプロジェクション変換行列を単位行列にする
		mxProjection = mxView = XMMatrixIdentity();
	}
	else
	{
		// カメラオブジェクトからビュー変換行列を取得
		mxView = Camera::mMainCamera->GetViewMatrix();//mCamera->GetViewMatrix();

		// プロジェクション変換行列を作成
		// 1: 視野角
		// 2: アスペクト比　→　基本、ウインドウサイズの縦横比を書けばよい
		// 3: near 見える範囲の一番手前の視点からの距離
		// 4: far  見える範囲の一番奥の視点からの距離
		mxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	}

	// 定数バッファの転送用構造体にコピー　※この際、行列は転置行列にする
	out.world = XMMatrixTranspose(mxWorld);
	out.view = XMMatrixTranspose(mxView);
	out.projection = XMMatrixTranspose(mxProjection);
	out.worldRotate = XMMatrixTranspose(mxRotate);
}

void Model::SetUVSplit(DirectX::XMFLOAT4 mSetUV)
{
	mUVSplit = mSetUV;
}

void Model::AnimationUpdate(int setState, int* setFlame)// アニメーション処理
{
	// アニメーションの時間をカウント
	mAnimTime += mAnimSpeed * gDeltaTime;

	int nowFlame = (int)mAnimTime;// 現在のフレーム

	if (setFlame[nowFlame] == -1)// アニメーションが最後まで行くとreset
	{
		mAnimTime = 0.0f;
		nowFlame = 0;
	}

	SetUVAnimation(setState, setFlame[nowFlame]);

}

void Model::SetUVAnimation(int mState, int mNowFlame)
{
	if (mUVSplit.z == 0.00f || mUVSplit.w == 0.00f)
	{
		// UV分割数がセットされていない不十分な時、処理を無視
	}
	else
	{
		uvWidth = mUVSplit.z;
		uvHeight = mUVSplit.w;

		float frameYoko = (float)mNowFlame; // テーブルから現在のコマを取得
		float frameTate = (float)mState; // キャラの向きを縦コマ番号とする

		float uLeft = frameYoko * uvWidth;
		float uRight = uLeft + uvWidth;
		float vTop = frameTate * uvHeight;
		float vBottom = vTop + uvHeight;

		// 位置を補正する
		if (fabs(uRight - 1.01f) <= 0.01f || uRight > 1.00f)//右端
		{
			uRight = 1.0f;
		}
		if (fabs(uLeft - 0.01f) <= 0.01f || uLeft < 0.00f)// 左端
		{
			uLeft = 0.0f;
		}
		if (fabs(vBottom - 1.01f) <= 0.01f || vBottom > 1.00f)// 下端
		{
			vBottom = 1.0f;
		}
		if (fabs(vTop - 0.01f) <= 0.01f || vTop < 0.00f)// 上端
		{
			vTop = 0.0f;
		}

		// モデル頂点データ作成
		const float w = mUVSplit.x / 2.0f;
		const float h = mUVSplit.y / 2.0f;

		ModelVertex vx[6];
		vx[0] = { -w,  h, 0, 1, 1, 1, 1, uLeft, vTop };      // 左上
		vx[1] = { w,  h, 0, 1, 1, 1, 1, uRight, vTop };    // 右上
		vx[2] = { w, -h, 0, 1, 1, 1, 1, uRight, vBottom };// 右下

		vx[3] = { w, -h, 0, 1, 1, 1, 1, uRight, vBottom };// 右下
		vx[4] = { -w, -h, 0, 1, 1, 1, 1,uLeft, vBottom };   // 左下
		vx[5] = { -w,  h, 0, 1, 1, 1, 1, uLeft, vTop };      // 左上
		mModelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // 頂点数を計算

		// モデル用の頂点バッファ作成
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = sizeof(vx);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		// バッファの初期値指定
		D3D11_SUBRESOURCE_DATA initialVertexData;
		// 頂点バッファの初期値
		initialVertexData.pSysMem = vx;
		// 頂点バッファでは使用しない
		initialVertexData.SysMemPitch = 0;
		initialVertexData.SysMemSlicePitch = 0;

		HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &mModelData.mVertexBuffer);

		if (FAILED(hr))
		{
			throw hr;
		}

	}
}