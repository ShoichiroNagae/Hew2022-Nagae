#include "GameScene.h"
#include "Direct3D.h"
#include <DirectXMath.h>
#include "ObjModelLoader.h"
#include "NormalObject.h"
#include "BillboardObject.h"
#include "Input.h"
#include "Camera.h"
#include "Model.h"
#include "SceneManager.h"

#include "AnimationData.h"
#include "CreateSquarePolygon.h"

RESULT_DATA BaseScene::mData;
extern ID3D11Buffer* gpConstBuffer; //定数バッファ

// コンストラクタ
GameScene::GameScene() {
	Init();
}

// 定数バッファ作成
void GameScene::CreateConstBuffer()
{
	// 定数バッファ作成
	// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC const_buffef_D{};
	const_buffef_D.ByteWidth = 4 * 4 * 4 * 4 * 2;				// バッファのサイズ（4x4行列x4個）
	const_buffef_D.Usage = D3D11_USAGE_DYNAMIC;					// 使用方法
	const_buffef_D.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// バッファの種類(コンスタントバッファ)
	const_buffef_D.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// CPUアクセス設定
	const_buffef_D.MiscFlags = 0;								// その他のオプション
	const_buffef_D.StructureByteStride = 0;						// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&const_buffef_D,
		nullptr, &gpConstBuffer);
}

// カメラ初期設定
void GameScene::CameraInit(Camera* cam)
{
	// カメラ初期値
	// eyeとfocusが同じ座標だとダメ
	cam->SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -2.0f));
	cam->SetFocus(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
	// upは(0.0f,0.0f,0.0f)だとダメ
	cam->SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

// モデル読み込み
void GameScene::ModelLoad(ObjModelLoader oml, std::string ModelName,
	const char* pObjFileName, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		pObjFileName, pTexFileName
	);
}

void GameScene::ModelLoad(ObjModelLoader oml,std::string ModelName,
	float width, float height, float uvWidth, float uvHeight, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		width, height, uvWidth, uvHeight, pTexFileName
	);
}

 // ビルボードアニメーション関係系用ローダー　　作成::井戸上
void GameScene::ModelLoad(ObjModelLoader oml, std::string ModelName,
	DirectX::XMFLOAT4 Set_whuv, const wchar_t* pTexFileName)
{
	oml = ObjModelLoader();
	gModelManager[ModelName] = oml.Load(
		Set_whuv.x, Set_whuv.y, Set_whuv.z, Set_whuv.w, pTexFileName
	);
}

void GameScene::ObjectCreate(std::string objName, float mScale, float mx, float my, float mz)
{
	Model* pModel = gObjManager[objName]->GetModel();
	pModel->SetModelData(gModelManager[objName]);
	pModel->SetScale(mScale);
	pModel->Setpos(mx, my, mz);
	pModel->mCamera = gpCamera;
}



// 敵の自動生成
void GameScene::CreateEnemy()
{
	if (playerLanded) return;

	int min = ENEMY_MIN_XPOS;
	int max = ENEMY_MAX_XPOS;
	std::random_device rnd;									// 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());									//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_int_distribution<> randDecimal(0, 99);		// [0, 99] 範囲の乱数			小数部
	std::uniform_int_distribution<> randInteger(min, max);	// プレイヤーに見える範囲の乱数	整数部
	std::uniform_int_distribution<> randY(1, 7);	// Y軸用乱数	整数部

	// 敵の位置をランダムで決定
	float EnemyPosX = randInteger(mt);
	EnemyPosX += (randDecimal(mt) * 0.01f);
	float EnemyPosY = randY(mt);

	Model* pPlayerModel = gObjManager["Player"]->GetModel();
	float EnemyPosZ = pPlayerModel->mPos.z + 50.0f;
	if (EnemyPosZ > GROUND_POS) EnemyPosZ = GROUND_POS - 50.0f;

	// 敵を生成
	GameObject* tmp = new NormalObject();
	Model* pEnemyModel = tmp->GetModel();
	HitSphere* pHit = tmp->GetHit();

	tmp->mSpeed = -ENEMY_SPEED_DEF;
	pEnemyModel->SetModelData(gModelManager["Enemy"]);
	pEnemyModel->SetScale(2.0f);
	pEnemyModel->mPos.z = EnemyPosZ;
	pEnemyModel->mPos.x = EnemyPosX;
	pEnemyModel->mPos.y = EnemyPosY;
	pEnemyModel->mRotate.y = 90.0f;
	pHit->SetHankei(1.0f);
	pEnemyModel->mCamera = gpCamera;
	gEnemyManager.emplace_back(tmp);
}

bool GameScene::CheckEnemy(GameObject* _enemy)
{
	Model* Enemy = _enemy->GetModel();
	bool isEnemyBack = false;

	// 敵がプレイヤーよりも後ろにいる
	if (Enemy->mPos.z < gpCamera->mEye.z) isEnemyBack = true;

	return isEnemyBack;
}

void GameScene::MoveLimit()
{
	// プレイヤーの座標を取得
	Model* pModel = gObjManager["Player"]->GetModel();
	float& x = pModel->mPos.x;
	float& y = pModel->mPos.y;
	float& z = pModel->mPos.z;

	// プレイヤー移動限界値を設定
	const float xMax = 5.5f;
	const float xMin = -5.5f;
	const float yMax = 7.5f;
	const float yMin = 0.5f;

	// 制限
	if (x >= xMax) x = xMax;
	if (x <= xMin) x = xMin;
	if (y >= yMax) y = yMax;
	if (y <= yMin) y = yMin;
}

// 初期化
void GameScene::Init()
{	
	// 定数バッファ作成
	this->CreateConstBuffer();

	// カメラ作成
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	CameraInit(gpCamera);


	// モデル読み込み
	ModelLoad(loader, "ground1", "assets/Game/ground1.obj", L"assets/Game/ground1.jpg");	// 地面
	ModelLoad(loader, "Player", 0.5f, 0.6f, 0.33f, 0.25f, L"assets/Game/char01.png");		// プレイヤー
	ModelLoad(loader, "Player", Player2DSize, L"assets/Game/MainPlayer.png");		// プレイヤー(変更要望)
	ModelLoad(loader, "Enemy", "assets/Game/billboard.obj", L"assets/Game/HewEnemy01.png");		// 敵
	ModelLoad(loader, "Enemy2", "assets/Game/billboard.obj", L"assets/Game/HewEnemy02.png");		// 敵
	ModelLoad(loader, "BackGround", "assets/Game/ground1.obj", L"assets/ground1.jpg");		// 背景

	ModelLoad(loader, "clearLogo", 1.0f, 1.0f, 1.0f, 1.0f, L"assets/Game/clearlogo.png");	// クリアロゴ

	// 2Dキャラオブジェクト生成
	gObjManager["Player"] = new BillboardObject();
	ObjectCreate("Player", 1.5f, 0.0f, 1.0f, 0.0f);
	Model* pModel = gObjManager["Player"]->GetModel();
	pModel->mRotate.y = 90.0f;	// プレイヤーをZ軸方向に向ける
	pModel->mCamera = gpCamera;
	pModel->mPos.y = 3.0f;
	HitSphere* pHit = gObjManager["Player"]->GetHit();                                                                                                            
	pHit->SetHankei(0.4f);
	gObjManager["Player"]->mSpeed = 0.005f;
	pModel->SetUVSplit(Player2DSize); //アニメーション用UVのセット(入れないと動かない。)

	// クリアロゴ生成
	gObjManager["clearLogo"] = new NormalObject();
	ObjectCreate("clearLogo", 1.0f, 0.0f, 0.0f, 0.0f);
	pModel = gObjManager["clearLogo"]->GetModel();
	pModel->mCamera = gpCamera;
	pModel->m2dRender = true;
	pModel->SetDiffuse(DirectX::XMFLOAT4(1, 1, 1, 0.5f));
	gObjManager["clearLogo"]->mActive = false;

	//gModelManager["title"] = CreateSquarePolygon(1.0f, 0.7f, 1.0f, 1.0f, L"assets/TitleLogo640.png");
	//tmp = new NormalObject();
	//pModel = tmp->GetModel();
	//pModel->SetModelData(gModelManager["title"]);
	//pModel->Set2dRender(true);
	//pModel->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	//gObjectList.emplace_back(tmp);

	// 地面を生成
	pModel = nullptr;
	// 下側
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 0.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = -2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// 上側
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 0.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 4.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 10.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// 右側
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = -8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 6.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
	// 左側
	for (int i = 0; i < MAX_GROUND; i++)
	{
		GameObject* tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 2.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);

		tmp = new NormalObject();
		pModel = tmp->GetModel();
		pModel->SetModelData(gModelManager["ground1"]);
		pModel->SetScale(2.0f);
		pModel->mPos.x = 8.0f;
		pModel->mPos.z = 4.0f * i;
		pModel->mPos.y = 6.0f;
		pModel->mCamera = gpCamera;
		gGround.emplace_back(tmp);
	}
		

	// メンバ変数初期化
	frameCount = 0;
	playerLanded = false;

	// 追従カメラが追従する対象を設定
	((BackCamera*)gpCamera)->SetTarget(gObjManager["Player"]);
}

void GameScene::Update()
{
	// デルタタイムが想定外の値になった場合の処理
	if (gDeltaTime >= 100)
	{
		gDeltaTime = 0;
	}
	
	if (gDeltaTime <= 0)
	{
		gDeltaTime = 1;
	}

	// 主人公の移動
	Model* pPlayerModel = gObjManager["Player"]->GetModel();
	if (Input_GetKeyDown('W')) pPlayerModel->mPos.y += 0.01f;
	if (Input_GetKeyDown('S')) pPlayerModel->mPos.y -= 0.01f;
	if (Input_GetKeyDown('A')) pPlayerModel->mPos.x -= 0.01f;
	if (Input_GetKeyDown('D')) pPlayerModel->mPos.x += 0.01f;
// ************************************************************* 
	// アニメーション切り替わりテスト
	// gObjManagerから別のobjectに切り替える
	// if(状態変数)を用意してアニメーションの管理をする
	// 事前にアニメーション用UVのセット（SetUVSplit）を入れないと動かない。Initで設定)
	// アニメーション処理
	// アニメーションの時間をカウント

	

	switch (SetAnimState)
	{
	case DEFAULT:
		pPlayerModel->AnimationUpdate(PLAYER2DSTATE::DEFAULT, Char2D_DefaultFlame);
		if (Input_GetKeyTrigger(VK_SPACE)) {// スぺースキーを推したらフラグON
			SetAnimState = PLAYER2DSTATE::ATTACK;
			pPlayerModel->mAnimTime = 0.0f;
			pPlayerModel->mAnimSpeed = 0.01f;
			pPlayerModel->Loop = false;
		}
		break;

	case ATTACK:
		pPlayerModel->AnimationUpdate(PLAYER2DSTATE::ATTACK, Char2D_AttackFlame);
		if (pPlayerModel->Loop == true)// 攻撃アニメーションが最後まで行くとフラグOFF
		{
			SetAnimState = PLAYER2DSTATE::DEFAULT;
			pPlayerModel->mAnimTime = 0.0f;
			pPlayerModel->mAnimSpeed = 0.004f;
		}
		break;
	}
	//// 固定表示
	//pPlayerModel->SetUVAnimation(PLAYER2DSTATE::FRONT, Char2D_kihonFlame[0]);

	//// テクスチャ変更
	/*if (Input_GetKeyDown(VK_SPACE))
		p2DcharModel->ChangeTexData(L"assets/ground1.jpg");*/
// **************************************************************	
	// プレイヤーの移動を制限
	MoveLimit();

	// 加速・減速
	if (Input_GetKeyDown('R')) {
		if (gObjManager["Player"]->mSpeed < 0.01f)
		{
			gObjManager["Player"]->mSpeed += 0.0001f;
		}
		else if (gObjManager["Player"]->mSpeed < 0.05f) 
		{
			gObjManager["Player"]->mSpeed += 0.00005f + (0.0000001f / gObjManager["Player"]->mSpeed);
		}
		else
		{
			gObjManager["Player"]->mSpeed += 0.0000001f / gObjManager["Player"]->mSpeed;
		}
	}

	if (Input_GetKeyDown('F')) {
		if (gObjManager["Player"]->mSpeed > 0.005f)
			gObjManager["Player"]->mSpeed -= 0.0001f;
	}

	// 敵の自動生成
	if (frameCount == 50) CreateEnemy();
	if (frameCount > 50) frameCount = 0;

	// 背景テスト
	if (Input_GetKeyTrigger('L')) {
		// オブジェクト生成
		gObjManager["BackGround"] = new NormalObject();
		Model* pModel = gObjManager["BackGround"]->GetModel();
		pModel->SetModelData(gModelManager["BackGround"]);
		pModel->SetScale(100.0f);
		pModel->mPos.x = -100.0f;
		pModel->mPos.y = 1.0f;
		pModel->mPos.z = 500.0f;
		pModel->mCamera = gpCamera;
	}

	// ゲームオブジェクトを更新
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Update();

	// 敵を全て更新
	for (int i = 0; i < gEnemyManager.size(); i++)
	{
		GameObject* tmp = gEnemyManager[i];
		HitSphere* tmpHit = tmp->GetHit();

		tmp->Update();

		// 当たり判定を実行
		if (tmpHit->IsHit(gObjManager["Player"]->GetHit()))
		{
			// 当たっているときにボタン入力があったら敵を消す
			if (SetAnimState == PLAYER2DSTATE::ATTACK)
			{
				// プレイヤーの切るアニメーションとか実行？

				// スコア増加？
				mData.KILL_ENEMY++;	// 倒した敵の数を増加
				this->nowCombo++;	// コンボを増加

				 delete gEnemyManager[i];
				 gEnemyManager.erase(gEnemyManager.begin() + i);
			}
			// 敵に当ってしまったとき
			if(SetAnimState == PLAYER2DSTATE::DEFAULT)
			{
				// 少し上に吹き飛ぶ
				Model* pEnemyModel = tmp->GetModel();
				if (pPlayerModel->mPos.z > pEnemyModel->mPos.z + 1.0f) {
					if (gObjManager["Player"]->mSpeed > 0.0f)
					{
						gObjManager["Player"]->mSpeed = 0.001f;
						delete gEnemyManager[i];
						gEnemyManager.erase(gEnemyManager.begin() + i);
					}
				}
				// プレイヤーのスコアとか減らす？

				// コンボを0に戻す
				if (nowCombo > maxCombo) maxCombo = nowCombo;
				this->nowCombo = 0;
			}
		}
		// プレイヤーと敵が接触しておらず
		// 敵がプレイヤーの後ろに行ったら消す
		else if (CheckEnemy(gEnemyManager[i]))
		{
			if (nowCombo > maxCombo) maxCombo = nowCombo;
			this->nowCombo = 0;

			delete gEnemyManager[i];
			gEnemyManager.erase(gEnemyManager.begin() + i);
		}
	}
	

	// 弾管理配列の中身をすべて更新する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Update();

	// 地面を全て更新
	for (int i = 0; i < gGround.size(); i++)
		gGround[i]->Update();

	// カメラの更新処理（ビュー変換行列計算）
	gpCamera->Update();

	// プレイヤーが地面についたときの処理
	if (gObjManager["Player"]->GetModel()->mPos.z > GROUND_POS)
	{
		// プレイヤーの移動を止める
		gObjManager["Player"]->mSpeed = 0.0f;
		playerLanded = true;

		// クリアロゴを表示
		gObjManager["clearLogo"]->mActive = true;

		// クリアロゴ表示から何秒後かにリザルトシーンへ遷移
	}

	// フレーム数加算
	frameCount++;

}

void GameScene::Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

	// ↓　自前の描画処理をここに書く *******

	// 地面を全て描画
	for (int i = 0; i < gGround.size(); i++)
		gGround[i]->Draw();
	
	// 敵を全て描画
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Draw();

	// ゲームオブジェクトを描画
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
		i->second->Draw();

	// 敵を全て描画
	for (int i = 0; i < gEnemyManager.size(); i++)
		gEnemyManager[i]->Draw();

	

	// 弾管理配列の中身をすべて描画する
	for (int i = 0; i < gShotManager.size(); i++)
		gShotManager[i]->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}

// 解放
void GameScene::Release()
{
	// 地面を全て描画
	for (int i = 0; i < gGround.size(); i++)
		delete gGround[i];

	COM_SAFE_RELEASE(gpConstBuffer);

	// モデルマネージャーが管理するモデルを全解放する
	for (auto i = gModelManager.begin();// 一番前の要素の管理情報を返す
		i != gModelManager.end();// 一番最後の管理情報を返す
		i++)
	{
		// first … 添え字
		// second … 格納されている要素そのもの
		COM_SAFE_RELEASE(i->second.mSRV);
		COM_SAFE_RELEASE(i->second.mVertexBuffer);
	}

	// 連想配列の要素を全削除
	gModelManager.clear();

	// オブジェクトマネージャーが管理するオブジェクトを全開放する
	for (auto i = gObjManager.begin();
		i != gObjManager.end();
		i++)
	{
		delete i->second;
	}
	// 連想配列の要素を全削除
	gObjManager.clear();

	// 敵マネージャーの要素を全て開放
	for (int i = 0; i < gEnemyManager.size(); i++)
		delete gEnemyManager[i];
}

// デストラクタ
GameScene::~GameScene() {
	Release();
}
