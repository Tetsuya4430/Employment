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
#include "EnemyBullet.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossBullet.h"
#include "ObjectManager.h"
#include "PostEffect.h"
#include "Controller.h"
#include "Particle.h"
#include "DefenceObject.h"
#include "ImGuiManager.h"

#include <list>
#include <sstream>
#include <fstream>


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

	//�_���[�W�G�t�F�N�g
	void Damage();

	//�Q�[���I�[�o�[���o
	void GameOver();

	//覐΂̋���
	void MeteorMove();

	//�{�X�̎��S�G�t�F�N�g
	void BossDeath();

	//�^�C�g�����o
	void MoveTitle();

	//�f�o�b�O�e�L�X�g�`��p�֐�
	void DebagText();

	void HpBarMove();

	void EnemyDown(XMFLOAT3 EnemyPos);

	void CameraMove();

	//UI�A���t�@�l����֐�
	void UI_AlphaIncriment(float color_w);
	void UI_AlphaDecriment(float color_w);

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
	Model* model_Moon = nullptr;
	Model* model_Mars = nullptr;
	Model* model_Neptune = nullptr;
	Model* model_Wall = nullptr;
	Model* model_Meteor = nullptr;
	Model* model_Station = nullptr;
	Model* model_SpaceStation = nullptr;
	Model* model_reticle = nullptr;
	Model* model_Shooting = nullptr;
	Model* model_Force = nullptr;

	FbxModel* model1 = nullptr;
	Fbx3d* object1 = nullptr;

	Input* input = nullptr;

	//�J����
	Camera* camera = nullptr;

	XMFLOAT3 CameraDefault = { 0, 0, -50.0f };

	//�h�ꕝ
	XMFLOAT3 Shake = { 2.0f, 0.0f, 0.0f };

	int ShakeTimer = 0;


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

	std::list<std::unique_ptr<DefenceObject>> objects;

	//�G�����R�}���h
	std::stringstream enemyPopCommands;

	//�{�X�̒e
	//std::list<std::unique_ptr<BossBullet>> bossbullets;
	std::list<std::unique_ptr<BossBullet>> bossbullets;

	//3D�I�u�W�F�N�g
	Object3d* CelestialSphere = nullptr;
	Object3d* Moon = nullptr;
	Object3d* Mars = nullptr;
	Object3d* Neptune = nullptr;
	Object3d* Wall = nullptr;
	Object3d* Station = nullptr;
	Object3d* SpaceStation = nullptr;
	Object3d* Shooting = nullptr;
	Object3d* Force = nullptr;
	Object3d* CameraObject = nullptr;

	Object3d* test = nullptr;

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
	//Particle* EnemyPart = nullptr;
	//Particle* ShotPart = nullptr;
	Particle* particle_Red = nullptr;

	Particle* Testpart = nullptr;


	//Player
	Player* player = nullptr;
	Player* Satellite_R = nullptr;
	Player* Satellite_L = nullptr;

	XMFLOAT3 PlayerScale = { 0.5f, 0.5f, 0.5f };
	XMFLOAT3 SatelliteRange = { 6.0, 1.0, 0.0 };

	//UI
	Sprite* sprite = nullptr;
	Sprite* UI = nullptr;
	Sprite* Stage_1 = nullptr;
	Sprite* Go = nullptr;
	Sprite* Rule = nullptr;
	Sprite* LoadBG = nullptr;
	Sprite* DamageEffect = nullptr;
	Sprite* StartUI = nullptr;
	Sprite* HPBar = nullptr;
	Sprite* EmpBar = nullptr;
	Sprite* BossHPBar = nullptr;
	Sprite* PlayerFrame = nullptr;
	Sprite* BossFrame = nullptr;
	Sprite* EmpBossBar = nullptr;

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

	//ImGui
	ImGuiManager* Imgui = nullptr;

	//�v���C���[�̃q�b�g�t���O
	bool HitFlag = false;

	//�A�˃^�C�}�[
	int ShotTimer = 0;


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

	//�{�X���S���o�̃p�[�e�B�N���̔����J�E���g
	int BossPartTimer = 0;
	int PartCount = 0;

	//csv�t�@�C������G�̍��W�i�[�p�ϐ�
	XMFLOAT3 Num = {0, 0, 0};

	//�{�X���j����̉��o�^�C�}�[
	int WaitTimer = 0;

	//�Q�[���X�^�[�g���o�p
	int StartTimer = 0;

	//3d�I�u�W�F�N�g�p�ϐ�
	XMFLOAT3 MoonPos = { 0, 0, 0 };
	XMFLOAT3 MoonRot = { 0, 0, 0 };

	XMFLOAT3 MarsPos = { 0, 0, 0 };
	XMFLOAT3 MarsRot = { 0, 0, 0 };

	XMFLOAT3 NeptunePos = { 0, 0, 0 };
	XMFLOAT3 NeptuneRot = { 0, 0, 0 };

	XMFLOAT3 StationPos = { 0, 0, 0 };
	XMFLOAT3 StationRot = { 0, 0, 0 };

	//�V���X�P�[��
	XMFLOAT3 ShereScale = { 900, 900, 900 };

	//���X�P�[��
	XMFLOAT3 MoonScale = { 60, 60, 60 };
	//�����W
	XMFLOAT3 MoonPosition = { 350, 150, 400 };

	//�ΐ��X�P�[��
	XMFLOAT3 MarsScale = { 20, 20, 20 };
	//�ΐ����W
	XMFLOAT3 MarsPosition = { 100, -100, 500 };

	//�C�����X�P�[��
	XMFLOAT3 NeptuneScale = { 30, 30, 30 };
	//�C�������W
	XMFLOAT3 NeptunePosition = { -200, 150, 600 };

	//�F���X�e�[�V�����X�P�[��
	XMFLOAT3 StationScale = { 50, 50, 50 };
	//�F���X�e�[�V�������W
	XMFLOAT3 StationPosition = { -300, -50, 500 };
	//�F���X�e�[�V������]
	XMFLOAT3 StationRotation = { -60, 0, 0 };

	//�e�L�X�g���W(Shooting)
	XMFLOAT3 DefaultShootingScale = { 8, 8, 8 };
	XMFLOAT3 DefaultShootingPos = { -20, 10, 0 };
	//�e�L�X�g���W(Force)
	XMFLOAT3 DefaultForceScale = { 8, 8, 8 };
	XMFLOAT3 DefaultForcePos = { 0, 0, 0 };

	//�X�^�[�g���oUI�p�ϐ�(Stage1)
	XMFLOAT2 DefaultStage1Pos = { 1280, 285 };
	//�ړI���W�ϐ�
	XMFLOAT3 Stage1PointPos = { 460, 285 , 0};

	//�X�^�[�g���oUI�p�ϐ�(Go)
	XMFLOAT2 DefaultGoPos = { 0, -720 };
	//�ړI���W�ϐ�
	XMFLOAT3 GOPointPos = { 0, 0, 0};

	//�{�X�o�ꎞUI�̃J���[�ϐ�
	XMFLOAT4 BossUIColor = { 1, 1, 1, 0.7f };
	//�{�X�o�ꎞUI�p���W
	XMFLOAT2 BossUIPos_1 = { 0, 100 };
	XMFLOAT2 BossUIPos_2 = { 1280, 100 };
	//�{�X�o�ꎞUI�p�ړI���W
	XMFLOAT3 BossUIPointPos_1 = { 1280, 0, 0 };
	XMFLOAT3 BossUIPointPos_2 = { 0, 0, 0 };

	//�J�����I�u�W�F�N�g�p�ϐ�
	XMFLOAT3 DefaultCameraObjPos = { 0, 0, -50.0f };
	XMFLOAT3 DefaultCameraObjScale = { 1, 1, 1 };

	//�^�C�}�[���Z�b�g�p�ϐ�
	int TimerReset = 0;
	//�{�X�^�C�}�[���Z�b�g
	int DefaultBossTimer = 128;

	//�X�e�[�WUI�ړ�����Q�[���X�^�[�gUI�ړ��܂ł̃����W�^�C��
	int RangeTime = 0;

	//�����A���t�@�l
	//MAX
	float DefaultAlpha_Max = 1.0f;

	float VY = 0.0f;	//Y�����̑��x
	const float gravity = -9.8f / 60.0f;	//�d��

	bool DamageEffectflag = false;

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

	XMFLOAT2 PlayerHPSize = {800, 30};
	float HPdiv = 0.0f;
	XMFLOAT2 PlayerNowHP = {1, PlayerHPSize.y};

	XMFLOAT2 BossHPSize = { 800, 30 };
	float BossHPdiv = 0.0f;
	XMFLOAT2 BossNowHP = { 1, BossHPSize.y };

	//�t���[��
	XMFLOAT2 FrameSize = { 820, 50 };

	//�v���C���[�֌W
	XMFLOAT3 PlayerPos = { 0, 0, 0 };
	XMFLOAT3 PlayerRot = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_R = { 0, 0, 0 };
	XMFLOAT3 SatellitePos_L = { 0, 0, 0 };

	XMFLOAT3 TestPos = { 0, 0, 0 };

	XMFLOAT3 testStart = { 0, 0, 0 };
	XMFLOAT3 testEnd = { 20, 20, 40 };

	XMFLOAT3 PartSpeed = { 0, 0, 10 };
	XMFLOAT3 WarningPartSpeed = { 5, 5, 3 };
	float PartVel = 10.0f;

	//�G
	XMFLOAT3 EnemyPos = { 0, 0, 0 };

	//覐Δ����^�C�}�[
	int MeteorTimer = 0;

	XMFLOAT3 MeteorPos = { 0, 0, 0 };

	//�I�[�f�B�I����
	float Attack_Volume = 0.5f;
	float GameBGM_Volume = 0.1f;

	//�Q�[���X�^�[�g�t���O
	bool GameStart = false;

	//�^�C�g���e�L�X�g�p���W
	XMFLOAT3 ShootingPos = { 0, 0, 0 };

	XMFLOAT3 ForcePos = { 0, 0, 0 };

	int Count = 0;
	float Move_F = 0.0f;

	XMFLOAT3 CameraPos = { 0, 0, -50.0f };

	//�J�����̎��_�ړ�����
	float CameraLimit = 1.0f;

	float CameraMoveValue = 0.1f;

	//���x���^�C�}�[
	int LevelCount = 0;

	//UI���o�ҋ@�J�E���g
	int RangeCount = 100;

	//�{�X���j���̑ҋ@�J�E���g
	int WaitCount = 120;

	//�Q�[���I�[�o�[���̑ҋ@�J�E���g
	int WaitCount_GameOver = 100;

	//�^�C�}�[�C���N�������g�ϐ�
	int TimerIncrimentValue = 1;

	//�[���ϐ�
	int Value_Zero = 0;

	//---UI�̃A���t�@�l�ݒ�p---//
	//�A���t�@�l�ő�l
	const float MAX_ALPHA = 1.0f;
	//�A���t�@�l�̍ŏ��l
	const float MIN_ALPHA = 0.5f;
};

