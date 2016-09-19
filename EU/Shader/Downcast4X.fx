// Predefinition(s)
///////////////////////////////////////////
#define DOWN_FILTER_COUNT 16
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
static const float2 vPixelDownFilter[DOWN_FILTER_COUNT] = {
    { 1.5,  -1.5 },
    { 1.5,  -0.5 },
    { 1.5,   0.5 },
    { 1.5,   1.5 },

    { 0.5,  -1.5 },
    { 0.5,  -0.5 },
    { 0.5,   0.5 },
    { 0.5,   1.5 },

    { -0.5,  -1.5 },
    { -0.5,  -0.5 },
    { -0.5,   0.5 },
    { -0.5,   1.5 },

    { -1.5,  -1.5 },
    { -1.5,  -0.5 },
    { -1.5,   0.5 },
    { -1.5,   1.5 },
};
static const float2 vTexelDownFilter[DOWN_FILTER_COUNT]< string ConvertPixelsToTexels = "vPixelDownFilter"; >;
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

    for (int i = 0; i < DOWN_FILTER_COUNT; ++i)col += tex2D(sampMain, uv + vTexelDownFilter[i].xy);

    return col / DOWN_FILTER_COUNT;
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}