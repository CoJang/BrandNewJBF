// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

extern float fLevel;

sampler sampFirst : register(s0){
    MipFilter = point;
    MinFilter = linear;
    MagFilter = linear;

    AddressU = clamp;
    AddressV = clamp;
};

sampler sampSecond : register(s1){
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

// Shader function(s) definition
///////////////////////////////////////////
VS_OUTPUT vert(VS_INPUT _in){
    VS_OUTPUT _out = (VS_OUTPUT)0;

    _out.pos = mul(float4(_in.pos, 1), matWVP);
    _out.uv = _in.uv;

    return _out;
}
float4 frag(float2 uv : TEXCOORD0) : COLOR0{
    float4 col = tex2D(sampFirst, uv);
    col += tex2D(sampSecond, uv) * fLevel;
    return col;
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}