#include "Model.h"

#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>

using namespace DirectX;
using namespace std;

//�ÓI�����o�ϐ��̎���
const std::string Model::baseDirectory = "Resources/";
ID3D12Device* Model::device = nullptr;
UINT Model::descriptorHandleIncrementSize_ = 0;

Model::~Model()
{
	for (auto m : meshes)
	{
		delete m;
	}
	meshes.clear();

	for (auto m : materials)
	{
		delete m.second;
	}
	materials.clear();
}

void Model::StaticInitialize(ID3D12Device* device)
{
	Model::device = device;

	//���b�V���̐ÓI������
	Mesh::StaticInitialize(device);
}

Model* Model::LoadFromObj(const std::string& modelname)
{
    Model* instance = new Model;

	////�f�X�N���v�^�q�[�v����
	//instance->InitializeDescriptorHeap();

	//OBJ�t�@�C������f�[�^��ǂݍ���
	instance->Initialize(modelname);

	////�o�b�t�@����
	//instance->CreateBuffers();

	return instance;
}

void Model::LoadTexture()
{
	int textureIndex = 0;
	string directoryPath = baseDirectory + name + "/";

	for (auto& m : materials)
	{
		Material* material = m.second;

		//�e�N�X�`������
		if (material->textureFileName.size() > 0)
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);

			//�}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
		//�e�N�X�`���Ȃ�
		else
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize_);

			//�}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);

			textureIndex++;
		}
	}

	//HRESULT result = S_FALSE;

	//// WIC�e�N�X�`���̃��[�h
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	////�t�@�C���p�X����
	//std::string filepath = directoyPath + filename;

	////���j�R�[�h������ɕϊ�����
	//wchar_t wfilepath[128];
	//int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//result = LoadFromWICFile(
	//	wfilepath, WIC_FLAGS_NONE,
	//	&metadata, scratchImg);
	//if (FAILED(result)) {
	//	return result;
	//}

	//const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	//// ���\�[�X�ݒ�
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	//// �e�N�X�`���p�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff_));
	//if (FAILED(result)) {
	//	return result;
	//}

	//// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//result = texbuff_->WriteToSubresource(
	//	0,
	//	nullptr, // �S�̈�փR�s�[
	//	img->pixels,    // ���f�[�^�A�h���X
	//	(UINT)img->rowPitch,  // 1���C���T�C�Y
	//	(UINT)img->slicePitch // 1���T�C�Y
	//);
	//if (FAILED(result)) {
	//	return result;
	//}

	//// �V�F�[�_���\�[�X�r���[�쐬
	//cpuDescHandleSRV_ = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap_->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);
	//gpuDescHandleSRV_ = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap_->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize_);

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	//D3D12_RESOURCE_DESC resDesc = texbuff_->GetDesc();

	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = 1;

	//device->CreateShaderResourceView(texbuff_.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
	//	&srvDesc, //�e�N�X�`���ݒ���
	//	cpuDescHandleSRV_
	//);

	//return true;
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	//// ���_�o�b�t�@�̐ݒ�
	//cmdList->IASetVertexBuffers(0, 1, &vbView_);
	//// �C���f�b�N�X�o�b�t�@�̐ݒ�
	//cmdList->IASetIndexBuffer(&ibView_);

	////�}�e���A���p�萔�o�b�t�@�r���[���Z�b�g
	//cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());

	//// �f�X�N���v�^�q�[�v�̔z��
	//ID3D12DescriptorHeap* ppHeaps[] = { descHeap_.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//// �V�F�[�_���\�[�X�r���[���Z�b�g
	//if (material.textureFilename.size() > 0)
	//{
	//	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV_);
	//}

	//// �`��R�}���h
	//cmdList->DrawIndexedInstanced(((UINT)indices_.size()), 1, 0, 0, 0);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�S���b�V����`��
	for (auto& mesh : meshes)
	{
		mesh->Draw(cmdList);
	}
}

void Model::Initialize(const std::string& modelname)
{
	//obj�t�@�C�����J��
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = "Resources/" + modelname + "/";

	//HRESULT result = S_FALSE;

	//�t�@�C���X�g���[��
	std::ifstream file;

	file.open(directoryPath + filename);

	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//���f�����𖼑O�ɃR�s�[
	name = modelname;

	//���b�V���𐶐�
	Mesh* mesh = new Mesh;
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	std::vector<XMFLOAT3> positions;	//���_���W
	std::vector<XMFLOAT3> normals;	//�@���x�N�g��
	std::vector<XMFLOAT2> texcoords;	//�e�N�X�`��UV

	//1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�u�X�y�[�X��؂�ōs�̐擪��������擾
		std::string Key;

		getline(line_stream, Key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (Key == "mtllib")
		{
			//�}�e���A���t�@�C���̊e�ǂݍ���
			std::string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		//�擪�̕�����g�Ȃ�O���[�v�̊J�n
		if (Key == "g")
		{
			//�J�����g�̃��b�V���̏�񂪑����Ă���Ȃ�
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0)
			{
				//�R���e�i�ɓo�^
				meshes.emplace_back(mesh);
				//���̃��b�V������
				mesh = new Mesh;
				indexCountTex = 0;
			}

			//�O���[�v���ǂݍ���
			string groupName;
			line_stream >> groupName;

			//���b�V���ɖ��O�Z�b�g
			mesh->SetName(groupName);
		}

		//�擪������v�Ȃ璸�_���W
		if (Key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			//���_���W�Ƀf�[�^��ǉ�
			positions.emplace_back(position);

		}

		//�擪������vt�Ȃ�e�N�X�`��
		if (Key == "vt")
		{
			//U, V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;

			//V�������]
			texcoord.y = 1.0f - texcoord.y;

			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		//�擪������vn�Ȃ�@���x�N�g��
		if (Key == "vn")
		{
			//X, Y, Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������usemtl�Ȃ�}�e���A�������蓖��
		if (Key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr)
			{
				//�}�e���A�����ǂݍ���
				string materialName;
				line_stream >> materialName;

				//�}�e���A�����Ō������A�}�e���A�������蓖�Ă�
				auto itr = materials.find(materialName);
				if (itr != materials.end())
				{
					mesh->SetMaterial(itr->second);
				}
			}
		}

		//�擪������f�Ȃ�|���S��(�O�p�`)
		if (Key == "f")
		{
			int faceIndexCount = 0;

			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				//���_�ԍ�
				index_stream >> indexPosition;
				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, std::ios_base::cur);

				//�}�e���A���A�e�N�X�`��������ꍇ
				if (material && material->textureFileName.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, std::ios_base::cur);	//�X���b�V�����΂�
					index_stream >> indexNormal;

					//���_�f�[�^�̒ǉ�
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);

					/*���_�C���f�b�N�X�ɒǉ�
					indices_.emplace_back((unsigned short)indices_.size());*/
				}

				else
				{
					char c;
					index_stream >> c;
					//�X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/')
					{
						//���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}

					else
					{
						index_stream.seekg(-1, ios_base::cur);	//1�����߂�
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
						index_stream >> indexNormal;
						//���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
				}
				//�C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3)
				{
					//�l�p�`�|���S��4�_�ڂȂ̂�
					//�l�p�`��0,1,2,3�̂�����2,3,0�ŎO�p�`���\�z
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				}

				else
				{
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
			}
		}
	}
		//�t�@�C�������
		file.close();

		//�R���e�i�ɓo�^
		meshes.emplace_back(mesh);

		//���b�V���̃}�e���A���`�F�b�N
		for (auto& m : meshes)
		{
			//�}�e���A���̊��蓖�Ă��Ȃ�
			if (m->GetMaterial() == nullptr)
			{
				if (defaultMaterial == nullptr)
				{
					//�f�t�H���g�}�e���A���𐶐�
					defaultMaterial = Material::Create();
					defaultMaterial->name = "no material";
					materials.emplace(defaultMaterial->name, defaultMaterial);
				}
				//�f�t�H���g�}�e���A�����Z�b�g
				m->SetMaterial(defaultMaterial);
			}
		}

		//���b�V���o�b�t�@�̐���
		for (auto& m : meshes)
		{
			m->CreateBuffers();
		}

		//�}�e���A���̐��l��萔�o�b�t�@�ɔ��f
		for (auto& m : materials)
		{
			m.second->Update();
		}

		//�f�X�N���v�^�q�[�v�̐���
		InitializeDescriptorHeap();

		//�e�N�X�`���̓ǂݍ���
		LoadTexture();
	}

void Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	//�}�e���A���̐�
	size_t count = materials.size();

	// �f�X�N���v�^�q�[�v�𐶐�	
	if (count > 0)
	{
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
		descHeapDesc.NumDescriptors = (UINT)count; // �V�F�[�_�[���\�[�X�r���[1��
		result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
		if (FAILED(result)) {
			assert(0);
		}
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize_ = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪�������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�擪�^�u�����̓X���[����
		if (key[0] == '\t')
		{
			key.erase(key.begin());		//�擪�̕�������폜
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			////�}�e���A�����ǂݍ���
			//line_stream >> material.name;
			//���Ƀ}�e���A���������
			if (material)
			{
				//�}�e���A���R���e�i�ɓo�^
				AddMaterial(material);
			}

			//�V�����}�e���A���𐶐�
			material = Material::Create();
			//�}�e���A�����ǂݍ���
			line_stream >> material->name;
		}

		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//�擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			//�e�N�X�`���t�@�C�����ǂݍ���
			line_stream >> material->textureFileName;

			//�e�N�X�`���ǂݍ���
			//LoadTexture(directoryPath, material.textureFilename);

			//�t���p�X����t�@�C���������o��
			size_t pos1;
			pos1 = material->textureFileName.rfind('\\');
			if (pos1 != string::npos)
			{
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}

			pos1 = material->textureFileName.rfind('/');
			if (pos1 != string::npos)
			{
				material->textureFileName = material->textureFileName.substr(pos1 + 1, material->textureFileName.size() - pos1 - 1);
			}

		}
	}

	//�t�@�C�������
	file.close();

	//�}�e���A����o�^
	if (material)
	{
		AddMaterial(material);
	}

}

void Model::AddMaterial(Material* material)
{
	//�R���e�i�ɓo�^
	materials.emplace(material->name, material);
}

void Model::CreateBuffers()
{
	//HRESULT result = S_FALSE;

	//UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices_.size());
	//UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices_.size());


	//// ���_�o�b�t�@����
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff_));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// �C���f�b�N�X�o�b�t�@����
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff__));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// ���_�o�b�t�@�ւ̃f�[�^�]��
	//VertexPosNormalUvSkin* vertMap = nullptr;
	//result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
	//	std::copy(vertices_.begin(), vertices_.end(), vertMap);
	//	vertBuff_->Unmap(0, nullptr);
	//}

	//// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//unsigned short* indexMap = nullptr;
	//result = indexBuff__->Map(0, nullptr, (void**)&indexMap);
	//if(SUCCEEDED(result))
	//{
	//	std::copy(indices_.begin(), indices_.end(), indexMap);
	//	indexBuff__->Unmap(0, nullptr);
	//}

	//// ���_�o�b�t�@�r���[�̍쐬
	//vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//vbView_.SizeInBytes = sizeVB;
	//vbView_.StrideInBytes = sizeof(vertices_[0]);

	//// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	//ibView_.BufferLocation = indexBuff__->GetGPUVirtualAddress();
	//ibView_.Format = DXGI_FORMAT_R16_UINT;
	//ibView_.SizeInBytes = sizeIB;


	////�}�e���A���p�萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB1_));
	//assert(SUCCEEDED(result));

	////�}�e���A���p�萔�o�b�t�@�փf�[�^�]��B1
	//ConstBufferDataB1* constMap1 = nullptr;
	//result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
	//assert(SUCCEEDED(result));
	//constMap1->ambient = material.ambient;
	//constMap1->diffuse = material.diffuse;
	//constMap1->specular = material.diffuse;
	//constMap1->alpha = material.alpha;
	//constBuffB1_->Unmap(0, nullptr);
}
