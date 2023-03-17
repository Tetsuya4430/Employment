#include "Player.h"

Player* Player::Create(Model* model, Camera* camera)
{
    //3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
    Player* instance = new Player();
    if (instance == nullptr)
    {
        return nullptr;
    }

    //������
    if (!instance->Initialize())
    {
        delete instance;
        assert(0);
    }

    //���f���̃Z�b�g
    if (model)
    {
        instance->SetModel(model);
    }

    //�J�����̃Z�b�g
    if (camera)
    {
        instance->SetCamera(camera);
    }

    return instance;
}


bool Player::Initialize()
{
    // nullptr�`�F�b�N
    assert(Object3d::GetDevice());

    //�R���g���[���[������
    InitInput();

    //�v���C���[�e��X�e�[�^�X������
	Object3d::SetPosition({ 0, -10, -50 });
    Level = 1;
    EXP = 0;

    audio->Initialize();


    Audio::GetInstance()->LoadWave("Avoid.wav");
    Audio::GetInstance()->LoadWave("LevelUp.wav");

    Object3d::Initialize();

    return true;
}

void Player::Update()
{
    Object3d::Update();

	Mag = 0.0f;

	if (Level == 1)
	{
		Speed = 0.5f;
	}

	else if (Level == 2)
	{
		Speed = 0.75f;
	}

	else if (Level == 3)
	{
		Speed = 1.0f;
	}



	//�R���g���[���[�̉������X�V
	UpdateInput();

	if (HP > 0)
	{
		//�v���C���[�̈ړ�
		//�L�[�{�[�h
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
		{
			MoveFlag = 1;
			K = 0.5f;

			if (MoveCanFlag == true)
			{


				if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_A) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_W))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (Input::GetInstance()->PushKey(DIK_D) && Input::GetInstance()->PushKey(DIK_S))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else
				{
					Mag = 1.0f;

					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				if (Input::GetInstance()->PushKey(DIK_D))
				{
					moveR = true;
					if (Object3d::position_.x <= 45)
					{
						Object3d::position_.x += Speed * Mag;
						Old.x = Speed * Mag;
					}

					if (Object3d::rotation_.z > -RotLimit)
					{
						//Object3d::rotation_.z -= RotValue;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_A))
				{
					moveL = true;
					if (Object3d::position_.x >= -45)
					{
						Object3d::position_.x -= Speed * Mag;
						Old.x = -Speed * Mag;
					}

					if (Object3d::rotation_.z < RotLimit)
					{
						//Object3d::rotation_.z += RotValue;
					}
				}


				if (Input::GetInstance()->PushKey(DIK_W))
				{
					moveU = true;
					if (Object3d::position_.y <= 20)
					{
						Object3d::position_.y += Speed * Mag;
						Old.y = Speed * Mag;
					}
				}

				if (Input::GetInstance()->PushKey(DIK_S))
				{
					moveD = true;
					if (Object3d::position_.y >= -25)
					{
						Object3d::position_.y -= Speed * Mag;
						Old.y = -Speed * Mag;
					}
				}

			}


			//�v���C���[���ړ����Ă��Ȃ��Ƃ��̓v���C���[�̉�]�p�x�����X�ɖ߂�
			if (MoveFlag == 0)
			{
				if (Object3d::rotation_.z > 0)
				{
					Object3d::rotation_.z -= 1.0f;
				}

				if (Object3d::rotation_.z < 0)
				{
					Object3d::rotation_.z += 1.0f;
				}
			}


			if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				MoveFlag = 1;
				if (IsButtonPush(ButtonKind::RightButton))
				{
					Object3d::position_.x += Speed * Mag;
					if (RotlimR < Object3d::rotation_.z)
					{
						Object3d::rotation_.z -= 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::LeftButton))
				{
					MoveFlag = 1;
					Object3d::position_.x -= Speed * Mag;
					if (RotlimL > Object3d::rotation_.z)
					{
						Object3d::rotation_.z += 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::UpButton))
				{
					Object3d::position_.y += Speed * Mag;
				}

				if (IsButtonPush(ButtonKind::DownButton))
				{
					Object3d::position_.y -= Speed * Mag;
				}
			}


			//}
		}

		else
		{
			MoveFlag = 0;
		}


		//�L�[�{�[�h
		if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{

			K = 0.5f;

			if (MoveCanFlag == true)
			{


				if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::UpButton))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (IsButtonPush(ButtonKind::LeftButton) && IsButtonPush(ButtonKind::DownButton))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::UpButton))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else if (IsButtonPush(ButtonKind::RightButton) && IsButtonPush(ButtonKind::DownButton))
				{
					Mag = 0.71f;
					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}

				else
				{
					Mag = 1.0f;

					moveR = false;
					moveL = false;
					moveU = false;
					moveD = false;
				}


				MoveFlag = 1;
				if (IsButtonPush(ButtonKind::RightButton))
				{
					moveR = true;
					if (Object3d::position_.x <= 45)
					{
						Object3d::position_.x += Speed * Mag;
						Old.x = Speed * Mag;
					}
				}


				if (IsButtonPush(ButtonKind::LeftButton))
				{
					moveL = true;
					if (Object3d::position_.x >= -45)
					{
						Object3d::position_.x -= Speed * Mag;
						Old.x = -Speed * Mag;
					}
				}


				if (IsButtonPush(ButtonKind::UpButton))
				{
					moveU = true;
					if (Object3d::position_.y <= 20)
					{
						Object3d::position_.y += Speed * Mag;
						Old.y = Speed * Mag;
					}
				}

				if (IsButtonPush(ButtonKind::DownButton))
				{
					moveD = true;
					if (Object3d::position_.y >= -25)
					{
						Object3d::position_.y -= Speed * Mag;
						Old.y = -Speed * Mag;
					}
				}

				else
				{
					MoveFlag = 0;
				}
			}


			//�v���C���[���ړ����Ă��Ȃ��Ƃ��̓v���C���[�̉�]�p�x�����X�ɖ߂�
			if (MoveFlag == 0)
			{
				if (Avoidanceflag_X == false && Avoidanceflag_Y == false)
				{
					/*if (rotation_.z > 0)
					{
						rotation_.z -= 0.5f;
					}

					if (rotation_.z < 0)
					{
						rotation_.z += 0.5f;
					}*/
				}
			}


			if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
			{
				MoveFlag = 1;
				if (IsButtonPush(ButtonKind::RightButton))
				{
					Object3d::position_.x += 0.5f;
					if (RotlimR < Object3d::rotation_.z)
					{
						Object3d::rotation_.z -= 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::LeftButton))
				{
					MoveFlag = 1;
					Object3d::position_.x -= 0.5f;
					if (RotlimL > Object3d::rotation_.z)
					{
						Object3d::rotation_.z += 0.3f;
					}
				}

				if (IsButtonPush(ButtonKind::UpButton))
				{
					Object3d::position_.y += 0.5f;
				}

				if (IsButtonPush(ButtonKind::DownButton))
				{
					Object3d::position_.y -= 0.5f;
				}
			}

			else
			{
				MoveFlag = 0;
			}


			//�R���g���[���[�̉������X�V
			UpdateInput();
			//}
		}

		//�����ړ����Ă��Ȃ��̂ł����
		if (Mag == 0.0f)
		{
			//�ړ��ʕ�������
			if (K > 0)
			{
				if (moveR == true)
				{
					K -= 0.01f;
					Object3d::position_.x += K;
				}

				if (moveL == true)
				{
					K -= 0.01f;
					Object3d::position_.x -= K;
				}

				if (moveU == true)
				{
					K -= 0.01f;
					Object3d::position_.y += K;
				}

				if (moveD == true)
				{
					K -= 0.01f;
					Object3d::position_.y -= K;
				}

				//�΂߈ړ�
				else if (moveR == true && moveU == true)
				{
					K -= 0.01f;
					Object3d::position_.x += K;
					Object3d::position_.y += K;
				}

				else if (moveR == true && moveD == true)
				{
					K -= 0.01f;
					Object3d::position_.x += K;
					Object3d::position_.y -= K;
				}

				else if (moveL == true && moveD == true)
				{
					K -= 0.01f;
					Object3d::position_.x -= K;
					Object3d::position_.y -= K;
				}

				else if (moveL == true && moveU == true)
				{
					K -= 0.01f;
					Object3d::position_.x -= K;
					Object3d::position_.y += K;
				}
			}

		}

		//�f�o�b�O�p
			//�I�u�W�F�N�g�̍��W
			//std::ostringstream Objstr;
			//Objstr << "ObjPos("
			//	<< std::fixed << std::setprecision(5)
			//	<< position_.x << ","
			//	<< position_.y << ","
			//	<< position_.z << ")";

			//DebugText::GetInstance()->Print(Objstr.str(), 0, 300, 2.0f);

			////mag
			//std::ostringstream Magstr;
			//Magstr << "Mag("
			//	<< std::fixed << std::setprecision(5)
			//	<< Mag << ")";

			//DebugText::GetInstance()->Print(Magstr.str(), 0, 0, 2.0f);

			////Old
			//std::ostringstream Oldstr;
			//Oldstr << "OldPos("
			//	<< std::fixed << std::setprecision(5)
			//	<< Old.x << ","
			//	<< Old.y << ")";

			//DebugText::GetInstance()->Print(Oldstr.str(), 0, 50, 2.0f);


			////�����ړ����I�����Ă���Ȃ�
			//if (Mag == 0.0)
			//{
			//	//�܂��ړ��ʂ�0�łȂ��̂Ȃ��
			//	if (Old.x != 0.0f)
			//	{
			//		if (Old.x < 0.0f)
			//		{
			//			Old.x += 0.1f;
			//		}

			//		if (Old.x > 0.0f)
			//		{
			//			Old.x -= 0.1f;
			//		}
			//	}

			//	//�܂��ړ��ʂ�0�łȂ��̂Ȃ��
			//	if (Old.y != 0.0f)
			//	{
			//		if (Old.y < 0.0f)
			//		{
			//			Old.y += 0.1f;
			//		}

			//		if (Old.y > 0.0f)
			//		{
			//			Old.y -= 0.1f;
			//		}
			//	}

			//	//�ړ��̕�������
			//	position_.x += Old.x;
			//	position_.y += Old.y;
			//}

			//������[�V����
			//�L�[�{�[�h
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S))
		{

			if (AvoidanceTimer_X == 0)
			{
				//�E���
				if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_D) && Avoidanceflag_X == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.x + AvoidDistance_X;
					RolePos = Object3d::rotation_.z - RoleDistance;
					RotFlag_R = true;

					Avoidanceflag_X = true;
				}

				//�����
				if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_A) && Avoidanceflag_X == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.x - AvoidDistance_X;
					RolePos = Object3d::rotation_.z + RoleDistance;
					RotFlag_L = true;

					Avoidanceflag_X = true;
				}
			}

			if (AvoidanceTimer_Y == 0)
			{
				//����
				if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_W) && Avoidanceflag_Y == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.y + AvoidDistance_Y;

					Avoidanceflag_Y = true;
				}

				//�����
				if (Input::GetInstance()->TriggerKey(DIK_E) && Input::GetInstance()->PushKey(DIK_S) && Avoidanceflag_Y == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.y - AvoidDistance_Y;

					Avoidanceflag_Y = true;
				}
			}
		}

		//�΂߉���͎󂯕t���Ȃ�
		if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_D))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (Input::GetInstance()->PushKey(DIK_W) && Input::GetInstance()->PushKey(DIK_A))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_D))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (Input::GetInstance()->PushKey(DIK_S) && Input::GetInstance()->PushKey(DIK_A))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		//�R���g���[���[
		if (IsButtonPush(ButtonKind::RightButton) || IsButtonPush(ButtonKind::LeftButton) || IsButtonPush(ButtonKind::UpButton) || IsButtonPush(ButtonKind::DownButton))
		{

			if (AvoidanceTimer_X == 0)
			{
				//�E���
				if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::RightButton) && Avoidanceflag_X == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.x + AvoidDistance_X;
					RolePos = Object3d::rotation_.z - RoleDistance;
					RotFlag_R = true;

					Avoidanceflag_X = true;
				}

				//�����
				if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::LeftButton) && Avoidanceflag_X == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.x - AvoidDistance_X;
					RolePos = Object3d::rotation_.z + RoleDistance;
					RotFlag_L = true;

					Avoidanceflag_X = true;
				}
			}

			if (AvoidanceTimer_Y == 0)
			{
				//����
				if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::UpButton) && Avoidanceflag_Y == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.y + AvoidDistance_Y;

					Avoidanceflag_Y = true;
				}

				//�����
				if (IsButtonPush(ButtonKind::Button_A) && IsButtonPush(ButtonKind::DownButton) && Avoidanceflag_Y == false)
				{
					Audio::GetInstance()->PlayWave("Avoid.wav", 0.3f, false);
					PointPos = Object3d::position_.y - AvoidDistance_Y;

					Avoidanceflag_Y = true;
				}
			}
		}

		//�΂߉���͎󂯕t���Ȃ�
		if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::RightButton))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (IsButtonPush(ButtonKind::UpButton) && IsButtonPush(ButtonKind::LeftButton))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::RightButton))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		if (IsButtonPush(ButtonKind::DownButton) && IsButtonPush(ButtonKind::LeftButton))
		{
			Avoidanceflag_X = false;
			Avoidanceflag_Y = false;
		}

		//���ۂ̉������
		if (Avoidanceflag_X == true)
		{
			AvoidanceTimer_X += 1;


			dx = (PointPos - Object3d::position_.x) / AvoidCount;
			Object3d::position_.x += dx;

			if (RotFlag_R == true)
			{
				rx = (RolePos - Object3d::rotation_.z) / AvoidCount;
				Object3d::rotation_.z += rx;
			}

			else if (RotFlag_L == true)
			{
				rx = (RolePos - Object3d::rotation_.z) / AvoidCount;
				Object3d::rotation_.z += rx;
			}

			if (AvoidanceTimer_X >= 20)
			{
				AvoidanceTimer_X = 0;
				Object3d::rotation_.z = 0;
				RotFlag_R = false;
				RotFlag_L = false;
				Avoidanceflag_X = false;
			}

		}

		if (Avoidanceflag_Y == true)
		{
			AvoidanceTimer_Y += 1;


			dx = (PointPos - Object3d::position_.y) / AvoidCount;
			Object3d::position_.y += dx;

			if (AvoidanceTimer_Y >= 20)
			{
				AvoidanceTimer_Y = 0;
				Avoidanceflag_Y = false;
			}
		}


		//�v���C���[����ʔ��O�ɐi�o���悤�Ƃ���Ƃ��͉����߂�


			//�E��
			if (Object3d::position_.x >= 45)
			{
				Object3d::position_.x = 45;
			}

			//����
			if (Object3d::position_.x <= -45)
			{
				Object3d::position_.x = -45;
			}

			//�㑤
			if (Object3d::position_.y >= 20)
			{
				Object3d::position_.y = 20;
			}

			//����
			if (Object3d::position_.y <= -20)
			{
				Object3d::position_.y = -20;
			}

		//���x���̊Ǘ�
		//�o���l��5���܂�����
		if (EXP == 5 && Level < 3)
		{
			Audio::GetInstance()->PlayWave("LevelUp.wav", 0.3f, false);
			//�o���l�����Z�b�g���āA���x����1�㏸
			EXP = 0;
 			LevelFlag = true;
			Level += 1;
		}
	}
}
