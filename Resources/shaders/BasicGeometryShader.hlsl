#include "Basic.hlsli"

[maxvertexcount(1)]
void main(
	triangle VSOutput input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
	//�_�X�g���[��
	//inout LineStream<GSOutput> output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[0].svpos;
		element.normal = input[0].normal;
		element.uv = input[0].uv;
		output.Append(element);
	}

	//GSOutput element;

	////�����̎n�_
	//element.svpos = input[0].svpos;
	//element.normal = input[0].normal;
	//element.uv = input[0].uv;
	//output.Append(element);

	////�����̏I�_
	//element.svpos = input[1].svpos;
	//element.normal = input[1].normal;
	//element.uv = input[1].uv;
	//output.Append(element);

}