#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3Dモデルを表示させるクラス
class Model
{
public:
	virtual void Draw(); // 表示

	// このクラスで表示する3Dデータをセットする
	void SetModelData(ModelData model);

	// このクラスで表示するテクスチャデータを変更する
	void ChangeTexData(const wchar_t* pTexFileName);


	// 仮想世界（ワールド）上の座標
	DirectX::XMFLOAT3 mPos;
	// 回転角度
	DirectX::XMFLOAT3 mRotate;
	// 拡大縮小率
	DirectX::XMFLOAT3 mScale
		=DirectX::XMFLOAT3(1.0f,1.0f,1.0f);

	// 拡大縮小率のxyzを同じ値で設定する
	void SetScale(float newScale);

	// ワールド上の座標を設定する
	void Setpos(float mx, float my, float mz);

	// このモデルに適用するカメラオブジェクト
	Camera* mCamera;

	// 2d or 3d
	bool m2dRender = false;

	// 2D描画に切り替えるかどうか
	void Set2dRender(bool is2D) {
		m2dRender = is2D;
		mLightActive = !is2D;
	}

	// モデルのディフューズマテリアル色の設定
	void SetDiffuse(DirectX::XMFLOAT4 diffuse) {
		mDiffuse = diffuse;
	}

	// UVアニメーション用オフセット
	DirectX::XMFLOAT2 mUvOffset = DirectX::XMFLOAT2(0, 0);

protected:
	// 3Dデータを持つ変数
	ModelData mModelData;

	// ライトの切り替え
	bool mLightActive = true;

	// ディフューズマテリアル色
	DirectX::XMFLOAT4 mDiffuse = DirectX::XMFLOAT4(1, 1, 1, 1);

	// 定数バッファの転送用
	struct ConstBufferData{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX worldRotate;
		// UVアニメーション用オフセット (zwは使わない)
		DirectX::XMFLOAT4 uvOffset;
		// ディフューズ・マテリアル
		DirectX::XMFLOAT4 diffuse;
		// 平行光源
		DirectX::XMFLOAT3 lightVector;
		float lightPower;
		// アンビエント
		DirectX::XMFLOAT4 ambient;
	};

	// ワールド，ビュー，プロジェクション，回転行列を作成して
	// outにセットする
	virtual void GetWVPRMatrix(ConstBufferData& out);
	
// 追加機能（アニメーション）
public:
	// アニメーションの分割数情報をセット(Init時設定でお願いします)
	void SetUVSplit(DirectX::XMFLOAT4 mSetUV);

	// アニメーション処理込み
	void AnimationUpdate(int setState, int* setFlame); // 状態変数enum 、対応するフレーム

	// 描画のみ（固定座標で表示する際にも仕様）
	void SetUVAnimation(int mState, int idxNowFlame); // 変数:アニメーションUVの方向, 対応するフレームの添え字

	float mAnimSpeed = 0.004f; // アニメ再生速度(シーン.cppで変更可能)

private:
	// アニメーションの分割数データ
	DirectX::XMFLOAT4 mUVSplit = DirectX::XMFLOAT4(0.00f, 0.00f, 0.00f, 0.00f);
	float mAnimTime = 0.000f; // アニメーションの添え字用（時間をカウントする）
	float uvWidth = 0.0f;  // キャラクター１コマのUの幅
	float uvHeight = 0.0f; // キャラクター１コマのVの高さ
};

