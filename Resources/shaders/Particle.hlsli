//定数バッファ
cbuffer cbuff0 : register(b0)
{
	matrix mat;		//3D変換行列
	matrix matBillboard;	//ビルボード行列
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
	float scale : TEXCOORD;	//スケール
	float4 color : COLOR;	//色
	float3 rotation : ROTATION;	//回転
};

//ジオメトリシェーダーからピクセルシェーダーへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float2 uv : TEXCOORD;		//uv値
	float4 color : COLOR;	//色
	float3 rotation : ROTATION;	//回転
};