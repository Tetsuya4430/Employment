#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

Audio* Audio::GetInstance()
{
	static Audio instance;

	return &instance;
}

void Audio::Initialize(const std::string& directoryPath)
{

	directoryPath_ = directoryPath;

	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;

	// XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
	}

	// �}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
	}

}

void Audio::Finalize()
{
	//XAudio2���
	xAudio2.Reset();

	//�����f�[�^���
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();

	for (; it != soundDatas.end(); ++it)
	{
		UnLoad(&it->second);
	}
}

void Audio::LoadWave(const std::string& filename)
{
	if (soundDatas.find(filename) != soundDatas.end())
	{
		//�����̏d���ǂݍ��݂𖳎�
		return;
	}

	//�f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	std::string fullpath = directoryPath_ + filename;

	// �t�@�C���X�g���[��
	std::ifstream file;
	// Wave�t�@�C�����J��
	file.open(fullpath, std::ios_base::binary);
	// �t�@�C���I�[�v�����s���`�F�b�N
	//assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	// Format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}


	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	// LIST�`�����N�����o�����ꍇ
	if (strncmp(data.id, "LIST", 4) == 0)
	{
		//�ǂݎ��ʒu��LIST�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0)
	{
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Wave�t�@�C������� Close Wave File
	file.close();

	//return����ׂ̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^��A�z�z��Ɋi�[
	soundDatas.insert(std::make_pair(filename, soundData));
}

void Audio::UnLoad(SoundData* soundData)
{
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

}

void Audio::PlayWave(const std::string& filename, const float Volume, bool Loop)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);

	//���ǂݍ��݂̌��o
	assert(it != soundDatas.end());

	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// �Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	//buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (Loop)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	soundData.pSourceVoice->SetVolume(Volume);

	// �g�`�f�[�^�̍Đ�
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();
}

void Audio::StopWave(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);

	//���ǂݍ��݂̌��o
	assert(it != soundDatas.end());

	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;


	XAUDIO2_VOICE_STATE State;
	soundData.pSourceVoice->GetState(&State);
	if (State.BuffersQueued == 0)
	{
		return;
	}

	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&buf);

}