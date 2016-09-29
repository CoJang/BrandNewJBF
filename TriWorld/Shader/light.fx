// Binded object(s) definition
///////////////////////////////////////////
extern float4x4 matWV : WV;
extern float4x4 matWVP : WVP;

extern float4 k_d : MATERIALDIFFUSE;
extern float4 k_a : MATERIALAMBIENT;
extern float4 k_s : MATERIALSPECULAR;
extern float k_p : MATERIALPOWER;

extern float4 l_d;
extern float4 l_a;
extern float4 l_s;
extern float3 l_r;

sampler sampBase : register(s0);
sampler sampSpec : register(s1);
///////////////////////////////////////////

// Inner structure(s) definition
///////////////////////////////////////////
struct VS_INPUT{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};
struct VS_OUTPUT{
    float4 pos : POSITION;
    float3 spec_pos : COLOR0;
    float3 normal : COLOR1;
    float2 uv : TEXCOORD0;
};

struct PS_INPUT{
    float3 spec_pos : COLOR0;
    float3 normal : COLOR1;
    float2 uv : TEXCOORD0;
};
///////////////////////////////////////////

VS_OUTPUT vert(VS_INPUT _in){
    VS_OUTPUT _out = (VS_OUTPUT)0;

    _out.pos = mul(float4(_in.pos, 1), matWVP);

    _out.spec_pos = mul(float4(_in.pos, 1), (float4x3)matWV);
    _out.spec_pos = normalize(_out.spec_pos);

    _out.normal = mul(_in.normal, (float3x3)matWV);
    _out.normal = normalize(_out.normal);

    _out.uv = _in.uv;

    return _out;
}
float4 frag(PS_INPUT _in) : COLOR{
    float4 col = tex2D(sampBase, _in.uv);
    float4 spec_map = tex2D(sampSpec, _in.uv);

    float3 L = -l_r;
    float3 N = _in.normal;
    float3 R = normalize(2 * dot(N, L) * N - L);
    float3 V = _in.spec_pos;

    float4 diff = (l_a * k_a) + (l_d * k_d) * max(0, dot(N, L));
    float4 spec = (l_s * k_s) * pow(max(0, dot(R, V)), k_p * 0.25f);

    spec *= spec_map;

    col = col * diff + spec;
    col = saturate(col);

    return col;
}

technique main{
    pass P0{
        VertexShader = compile vs_2_0 vert();
        PixelShader = compile ps_2_0 frag();
    }
}