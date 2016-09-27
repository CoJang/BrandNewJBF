// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

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

struct PS_INPUT{
    float2 uv : TEXCOORD0;
};
///////////////////////////////////////////

// Inner function(s) definition
///////////////////////////////////////////
float4 extractBright(float4 col){
    col = pow(max(col, 0), 2.2f);
    col = saturate((col - fLevel) / (1 - fLevel));

    return col;
}
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
    float4 col = tex2D(sampMain, uv);
    return extractBright(col);
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}