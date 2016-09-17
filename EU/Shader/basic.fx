// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

extern texture texMain;
sampler sampMain = sampler_state{
    Texture = (texMain);

    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = wrap;
    AddressV = wrap;
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

// Shader function(s) definition
///////////////////////////////////////////
VS_OUTPUT vert(VS_INPUT _in){
    VS_OUTPUT _out = (VS_OUTPUT)0;

    _out.pos = mul(float4(_in.pos, 1), matWVP);
    _out.uv = _in.uv;

    return _out;
}
float4 frag(PS_INPUT _in) : COLOR{
    return tex2D(sampMain, _in.uv);
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}