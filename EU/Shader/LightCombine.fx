// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

sampler sampBase : register(s0);
sampler sampBackgroundLight : register(s1);
sampler sampForegroundLight : register(s2);
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
    float3 col = tex2D(sampBase, uv);
    float3 light = max(tex2D(sampBackgroundLight, uv), tex2D(sampForegroundLight, uv));

    col += light;
    col = saturate(col);

    return float4(col, 1);
}
///////////////////////////////////////////

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}