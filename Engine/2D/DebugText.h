#pragma once
#include "Sprite.h"

#include <windows.h>
#include <string>

//�f�o�b�O������N���X�̒�`
class DebugText
{
public:
	static DebugText* GetInstance();

	//�����ɒ萔�錾���L�q
	static const int maxCharCount = 256;	//1�t���[���ł̍ő�\��������
	static const int fontWidth = 9;		//�t�H���g�摜��1�������̉���
	static const int fontHeight = 18;		//�t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;	//�t�H���g�摜��1�����̕�����

//�����Ƀ����o�֐��̐錾���L�q

	DebugText();
	~DebugText();

	void Initialize(UINT texnumber);
	void Print(const std::string& text, const float& x, const float& y, const float& scale = 1.0f);
	void DrawAll();

	/*void Finalize();*/
private:
	//�����Ƀ����o�ϐ��̐錾���L�q

		//�X�v���C�g���ʕ���(�؂�Ă���)
	SpriteCommon* spriteCommon_ = nullptr;
	//�X�v���C�g�̔z��
	Sprite* sprites_[maxCharCount];
	//�X�v���C�g�z��̓Y�����ԍ�
	int spriteIndex_ = 0;
};