// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

extern float fCoefficient;
extern float fLevel;

sampler sampMain : register(s0);
///////////////////////////////////////////

// Inner structure(s) definition
///////////////////////////////////////////
struct VS_INPUT{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};
struct VS_OUTPUT{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};
///////////////////////////////////////////

// Shader function(s) definition
///////////////////////////////////////////
VS_OUTPUT vert(VS_INPUT _in){
    VS_OUTPUT _out = (VS_OUTPUT)0;

    _out.pos = mul(float4(_in.pos, 1), matWVP);
    _out.uv = _in.uv;

    return _out;
}
float4 frag(float2 uv : TEXCOORD0) : COLOR0{
    float r2 = ((uv.x - 0.5f) * (uv.x - 0.5f)) + ((uv.y - 0.5f) * (uv.y - 0.5f));
    float f = (fLevel != 0) ? (1.f + r2 * (fCoefficient + fLevel * sqrt(r2))) : (1.f + r2 * fCoefficient);

    float2 uv2 = f * (uv - 0.5f) + 0.5f;

    float3 col_r = tex2D(sampMain, uv2) * float3(1, 0, 0);
    float3 col_b = tex2D(sampMain, uv) * float3(0, 1, 1);

    return float4(max(col_r, col_b), 1);
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}