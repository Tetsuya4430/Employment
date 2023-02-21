#include "BaseScene.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "DebugText.h"
#include "Sprite.h"
#include "Object3d.h"
#include "Player.h"
#include "Fbx3d.h"
#include "Audio.h"
#include "Input.h"
#include "Bullet.h"
//#include "Reticle.h"
#include "EnemyBullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossBullet.h"
#include "ObjectManager.h"
#include "PostEffect.h"
#include "Controller.h"
#include "Particle.h"
//#include "Test.h"

#include <list>
#include <sstream>
#include <fstream>

//class Player;
//class Bullet;
//class EnemyBullet;

/// <summary>
/// �Q�[���v���C�V�[��
/// </summary>
class GameScene : public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static const int debugTextTexNumber = 0;

	// �R���X�g�N���^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	//������
	void Initialize(/*DirectXCommon* dxCommon*/) ;

	//�I������
	void Finalize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;


	//�G�̏���������
	void EnemyInit();

	//�{�X�̏���������
	void BossInit();

	//�G�̍X�V�֐�
	void EnemyUpdate(XMFLOAT3 enemyPos);

	//�v���C���[�̍U��
	void Attack(XMFLOAT3 StartPos);

	//�G�̍U��
	void EnemyAttack(XMFLOAT3 EnemyPos);

	//�{�X�̍U��
	void BossAttack(XMFLOAT3 BossPos);

	//�G�̔����f�[�^�ǂݍ���
	void LoadEnemyPopData();

	//�G�����R�}���h�̍X�V
	void UpdateEnemyPopCommands();

	//�p�[�e�B�N�������֐�
	//void CreateParticleInfo(int PartNum, XMFLOAT3 Position, float Vel, int ParticleLife, float StartScale, float EndScale);

	//�~�Ɖ~�̓����蔻��
	bool CheckCollision(XMFLOAT3 Object1, XMFLOAT3 Object2, float R1, float R2);

	//�X�v���C�g�ǂݍ���
	void LoadSprite();

	//�X�v���C�g�X�V
	void UpdateSprite();

	//�X�v���C�g�`��
	void DrawSprite();

	//�X�^�[�g���o
	void Start();

private:
	//�萔
	const int MAX_Object = 10;

private:
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	//WindowsAPI
	//WinApp* winApp =  nullptr;

	//�f�o�b�O�e�L�X�g
	DebugText debugText;

	Model* model_1 = nullptr;
	Model* model_2 = nullptr;
	Model* model_Bullet = nullptr;
	Model* model_EnemyBullet = nullptr;
	Model* model_Enemy = nullptr;
	Model* model_Boss = nullptr;
	Model* model_sphere = nullptr;
	Model* model_reticle = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	Camera* camera = nullptr;
	PostEffect* postEffect = nullptr;

	Audio* audio = Audio::GetInstance();

	//�e
	std::list<std::unique_ptr<Bullet>> bullets;

	//�R�A�e
	std::list<std::unique_ptr<Bullet>> CoreR_bullets;
	std::list<std::unique_ptr<Bullet>> CoreL_bullets;


	//�G�̒e
	std::list<std::unique_ptr<EnemyBullet>> enemybullets;
	
	//�G
	std::list<std::unique_ptr<Enemy>> enemys;

	int EnemyFire = 0;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	//�{�X�̒e
	//std::list<std::unique_ptr<BossBullet>> bossbullets;
	std::list<std::unique_ptr<BossBullet>> bossbullets;

	Object3d* CelestialSphere = nullptr;

	//�v���C���[
	Player* P = nullptr;
	Player* CoreR = nullptr;
	Player* CoreL = nullptr;
	//Reticle* Reticle = nullptr;
	//Bullet* B = nullptr;

	//�{�X
	//St1_Boss* Boss = nullptr;
	Boss* Boss = nullptr;
	int BossFire = 0;
	
	//�p�[�e�B�N��
	Particle* particle = nullptr;
	Particle* EnemyPart = nullptr;


	//Player
	Player* player = nullptr;
	Player* Satellite_R = nullptr;
	Player* Satellite_L = nullptr;

	//UI
	Sprite* sprite = nullptr;
	Sprite* UI = nullptr;
	Sprite* HP_0 = nullptr;
	Sprite* HP_1 = nullptr;
	Sprite* HP_2 = nullptr;
	Sprite* HP_3 = nullptr;
	Sprite* HP_4 = nullptr;
	Sprite* HP_5 = nullptr;
	Sprite* Stage_1 = nullptr;
	Sprite* Go = nullptr;
	Sprite* Rule = nullptr;
	Sprite* LoadBG = nullptr;

	//�{�XHP�X�v���C�g
	Sprite* BossHP_0 = nullptr;
	Sprite* BossHP_1 = nullptr;
	Sprite* BossHP_2 = nullptr;
	Sprite* BossHP_3 = nullptr;
	Sprite* BossHP_4 = nullptr;
	Sprite* BossHP_5 = nullptr;
	Sprite* BossHP_6 = nullptr;
	Sprite* BossHP_7 = nullptr;
	Sprite* BossHP_8 = nullptr;
	Sprite* BossHP_9 = nullptr;
	Sprite* BossHP_10 = nullptr;
	Sprite* BossHP_11 = nullptr;
	Sprite* BossHP_12 = nullptr;
	Sprite* BossHP_13 = nullptr;
	Sprite* BossHP_14 = nullptr;
	Sprite* BossHP_15 = nullptr;

	//�v���C���[�o���l�X�v���C�g
	Sprite* ExpBar_0 = nullptr;
	Sprite* ExpBar_1 = nullptr;
	Sprite* ExpBar_2 = nullptr;
	Sprite* ExpBar_3 = nullptr;
	Sprite* ExpBar_4 = nullptr;
	Sprite* ExpBar_5 = nullptr;
	Sprite* Level_1 = nullptr;
	Sprite* Level_2 = nullptr;
	Sprite* Level_3 = nullptr;

	//�{�X�o�ꉉ�oUI
	Sprite* BossUI_U = nullptr;
	Sprite* BossUI_U_2 = nullptr;
	Sprite* BossUI_D = nullptr;
	Sprite* BossUI_D_2 = nullptr;
	Sprite* Warning = nullptr;


	//�G�̍��W

	float MoveVec = 1.0f;

	int BulletFlag = 0;

	int MoveFlag = 0;

	//�G�̒e���˃^�C�}�[
	int EnemyBulletTimer = 0;

	//�G�̑ҋ@�t���O
	bool EnemyFlag = false;

	//�G�̑ҋ@�^�C�}�[
	int EnemyTimer = 0;

	//�{�X�̒e���˃^�C�}�[
	int BossBulletTimer = 0;

	//�{�X�̑ҋ@�t���O
	bool BossFlag = false;

	//�{�X�o�ꉉ�o�t���O
	bool BossFlag_S = false;
	bool BossFlag_E = false;

	//�{�X���oUI��A�p�t���O
	bool EndUIFlag = false;
	
	//�{�XUI�`��t���O
	bool BossUIDrawFlag = false;

	//Warning�t���O
	bool WarningFlag = false;

	//���[�h�t���O
	bool LoadFlag = false;

	//�{�X�̑ҋ@�^�C�}�[
	int BossTimer = 128;

	//csv�t�@�C������G�̍��W�i�[�p�ϐ�
	XMFLOAT3 Num = {0, 0, 0};

	//�{�X���j����̉��o�^�C�}�[
	int WaitTimer = 0;

	//�Q�[���X�^�[�g���o�p
	int StartTimer = 0;

	public:
	//�e�̔��ˊԊu
	static const int BulletInterval = 60;

	//�{�X�̒e���ˊԊu
	static const int BossBulletInterval = 5;

	//���e�B�N�����W
	XMFLOAT3 ReticlePos = { 0, 0, 0 };

	//�e�X�g
	int Timer = 60;

	//�{�X�o��t���O
	bool BossHPFlag = false;

	//�{�X�̗̓^�C�}�[
	int BossHPTimer = 120;

	XMFLOAT2 BarSize = { 100, 200 };


	//�v���C���[�֌W
	XMFLOAT3 PlayerPos = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_R = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_L = { 0, 0, 0 };

	//�G
	XMFLOAT3 EnemyPos = { 0, 0, 0 };

	//�I�[�f�B�I����
	float Attack_Volume = 0.5f;
};

