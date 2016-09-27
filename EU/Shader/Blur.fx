// Predefinition(s)
///////////////////////////////////////////
#define BLUR_KERNAL_COUNT 15
///////////////////////////////////////////

// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWVP : WVP;

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

// Inner variable(s) definition
///////////////////////////////////////////
static const float vPixelKernel[BLUR_KERNAL_COUNT] = {
    0,
    0.00375,
    -0.00375,
    0.00875,
    -0.00875,
    0.01375,
    -0.01375,
    0.01875,
    -0.01875,
    0.02375,
    -0.02375,
    0.02875,
    -0.02875,
    0.03375,
    -0.03375
};

static const float fBlurWeights[BLUR_KERNAL_COUNT] = {
    0.199471146  / 1.87975681,
    0.193334058  / 1.87975681,
    0.193334058  / 1.87975681,
    0.176032662  / 1.87975681,
    0.176032662  / 1.87975681,
    0.150568709  / 1.87975681,
    0.150568709  / 1.87975681,
    0.120985359  / 1.87975681,
    0.120985359  / 1.87975681,
    0.0913245454 / 1.87975681,
    0.0913245454 / 1.87975681,
    0.0647588    / 1.87975681,
    0.0647588    / 1.87975681,
    0.04313866   / 1.87975681,
    0.04313866   / 1.87975681,
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

float4 frag_v(float2 uv : TEXCOORD0) : COLOR0{
    float4 col = 0;

    for (int i = 0; i < BLUR_KERNAL_COUNT; ++i)col += tex2D(sampMain, uv + float2(0, vPixelKernel[i])) * fBlurWeights[i];

    return col;
}
float4 frag_h(float2 uv : TEXCOORD0) : COLOR0{
    float4 col = 0;

    for (int i = 0; i < BLUR_KERNAL_COUNT; ++i)col += tex2D(sampMain, uv + float2(vPixelKernel[i], 0)) * fBlurWeights[i];

    return col;
}
///////////////////////////////////////////

technique main_v{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag_v();
    }
}
technique main_h{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag_h();
    }
}