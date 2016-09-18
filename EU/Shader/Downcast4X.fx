// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

extern float fLevel;

extern texture texMain;
sampler sampMain = sampler_state{
    Texture = (texMain);

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

struct PS_INPUT{
    float2 uv : TEXCOORD0;
};
///////////////////////////////////////////

// Inner variable(s) definition
///////////////////////////////////////////
float2 vPixelDownFilter[16] = {
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
float2 vTexelDownFilter[16]< string ConvertPixelsToTexels = "vPixelDownFilter"; >;
///////////////////////////////////////////

// Shader function(s) definition
///////////////////////////////////////////
VS_OUTPUT vert(VS_INPUT _in){
    VS_OUTPUT _out = (VS_OUTPUT)0;

    _out.pos = mul(float4(_in.pos, 1), matWVP);
    _out.uv = _in.uv;

    return _out;
}
float4 frag(PS_INPUT _in) : COLOR{
    float4 col = 0;

    for (int i = 0; i < 16; ++i)col += tex2D(sampMain, _in.uv + vTexelDownFilter[i].xy);

    return col / 16;
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}