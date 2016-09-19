// Predefinition(s)
///////////////////////////////////////////
#define BLUR_KERNAL_COUNT 9
///////////////////////////////////////////

// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

extern float fLevel;

sampler sampMain : register(s0){
    MipFilter = point;
    MinFilter = linear;
    MagFilter = linear;

    AddressU = clamp;
    AddressV = clamp;
};
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

// Inner variable(s) definition
///////////////////////////////////////////
static const float2 vPixelKernel[BLUR_KERNAL_COUNT] = {
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0,  0 },
    { 0,  1 },
    { 0,  2 },
    { 0,  3 },
    { 0,  4 },
};

static const float fBlurWeights[BLUR_KERNAL_COUNT] = {
    0.05,
    0.09,
    0.12,
    0.15,
    0.16,
    0.15,
    0.12,
    0.09,
    0.05,
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
    float4 col = 0;

    for (int i = 0; i < BLUR_KERNAL_COUNT; ++i)col += tex2D(sampMain, uv + vPixelKernel[i].xy * fLevel) * fBlurWeights[i];
    col *= 1.5f;

    return col;
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}