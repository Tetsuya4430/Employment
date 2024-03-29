#include "DebugText.h"

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

void DebugText::Initialize(UINT texnumber)
{
	//全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites_); i++)
	{
		//スプライトを生成
		sprites_[i] = Sprite::Create(texnumber, { 1, 1, 1, 1 }, { 0, 0 });
	}
}

void DebugText::Finalize()
{
}

void DebugText::Print(const std::string& text, const float& x, const float& y, const float& scale)
{
	//全ての文字について
	for (int i = 0; i < text.size(); i++)
	{
		//最大文字数超過
		if (spriteIndex_ >= maxCharCount)
		{
			break;
		}
		//1文字取り出す(※ASCIIコードでしか成り立たない
		const unsigned char& character = text[i];

		//ASCIIコードの2段分飛ばした番号を計算
		int fontIndex = character - 32;

		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//座標計算
		sprites_[spriteIndex_]->SetPosition({ x + fontWidth * scale * i, y, 0 });
		sprites_[spriteIndex_]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites_[spriteIndex_]->SetTexSize({ fontWidth,  fontHeight });
		sprites_[spriteIndex_]->SetSize({ fontWidth * scale, fontHeight * scale });
		//頂点バッファ転送
		sprites_[spriteIndex_]->TransferVertexBuffer();

		//更新
		sprites_[spriteIndex_]->Update();
		//文字を1つ進める
		spriteIndex_++;
	}
}

//まとめて描画
void DebugText::DrawAll()
{
	//全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex_; i++)
	{
		//スプライト描画
		sprites_[i]->Draw();
	}
	spriteIndex_ = 0;
}

//void DebugText::Finalize()
//{
//	delete sprites_;
//}
