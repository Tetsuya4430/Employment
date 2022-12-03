#include "Particle.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//ê√ìIÉÅÉìÉoïœêîÇÃé¿ëÃ
const float Particle::radius = 5.0f;
const float Particle::prizmHeight = 8.0f;
ID3D12Device* Particle::device = nullptr;
UINT Particle::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Particle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Particle::rootsignature;
ComPtr<ID3D12PipelineState> Particle::pipelinestate;
ComPtr<ID3D12DescriptorHeap> Particle::descHeap;
ComPtr<ID3D12Resource> Particle::vertBuff;
ComPtr<ID3D12Resource> Particle::indexBuff;
ComPtr<ID3D12Resource> Particle::texBuff;
CD3DX12_CPU_DESCRIPTOR_HANDLE Particle::cpuDescHandleSRV;
CD3DX12_GPU_DESCRIPTOR_HANDLE Particle::gpuDescHandleSRV;
XMMATRIX Particle::matView{};
XMMATRIX Particle::matProjection{};
//XMFLOAT3 Particle::eye = { 0, 0, -50.0f };
//XMFLOAT3 Particle::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW Particle::vbView{};
D3D12_INDEX_BUFFER_VIEW Particle::ibView{};
Particle::VertexPosNormalUv Particle::vertices[vertexCount];
unsigned short Particle::indices[planeCount * 3];