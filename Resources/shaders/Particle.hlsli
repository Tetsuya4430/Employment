cbuffer cbuff0 : register(b0)
{
	//float4 color;	//色(RGBA)
	matrix mat;		//3D変換行列
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
	//float3 normal: NOMAL;		//法線ベクトル
	//float2 uv : TEXCOORD;		//uv座標
};

//ジオメトリシェーダーからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	//float3 normal : NORMAL;		//法線ベクトル
	float2 uv : TEXCOORD;		//uv値
};