cbuffer CBUFFER {
	matrix m, v, p;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	pos = mul(m, pos);
	pos = mul(v, pos);
	pos = mul(p, pos);

	return pos;
}