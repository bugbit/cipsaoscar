float4x4 WorldITXf : WorldInverseTranspose;
float4x4 WvpXf : WorldViewProjection;
float4x4 WorldXf : World;
float4x4 ViewIXf : ViewInverse;

/// Point Lamp 0 ////////////
float3 Lamp0Pos : Position = {20.5f,2.0f,1.25f};
float3 Lamp0Color : Specular = {1.0f,1.0f,1.0f};

// Ambient Light
float3 AmbiColor : Ambient = {0.3f,0.3f,0.3f};

float Ks = 0.9;

float SpecExpon : SpecularPower = 10.0;

float Bump : BumpConstant = 3.0; 

float Kr : ReflectConstant = 0.7;

sampler DiffuseSampler : register( s0 ) = sampler_state
{
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Wrap;
    AddressV = Wrap;
};

sampler sNormalSampler : register( s1 ) = sampler_state
{
    MipFilter = LINEAR; 
    MinFilter = LINEAR;
    MagFilter = LINEAR;
	AddressU = Wrap;
    AddressV = Wrap;
};

sampler sEnvSampler : register( s2 ) = sampler_state
{
    magfilter = LINEAR;
	mipfilter = LINEAR;
	miNfilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
};

struct appdata {
    float3 Position	: POSITION;
    float4 UV		: TEXCOORD0;
    float4 Normal	: NORMAL;
    float4 Tangent	: TANGENT0;
    float4 Binormal	: BINORMAL0;
};

struct vertexOutput {
    float4 HPosition	: POSITION;
    float2 UV		: TEXCOORD0;
    // The following values are passed in "World" coordinates since
    //   it tends to be the most flexible and easy for handling
    //   reflections, sky lighting, and other "global" effects.
    float3 LightVec	: TEXCOORD1;
    float3 WorldNormal	: TEXCOORD2;
    float3 WorldTangent	: TEXCOORD3;
    float3 WorldBinormal : TEXCOORD4;
    float3 WorldView	: TEXCOORD5;
};

void phong_shading(vertexOutput IN,
		    float3 LightColor,
		    float3 Nn,
		    float3 Ln,
		    float3 Vn,
		    out float3 DiffuseContrib,
		    out float3 SpecularContrib)
{
    float3 Hn = normalize(Vn + Ln);
    float4 litV = lit(dot(Ln,Nn),dot(Hn,Nn),SpecExpon);
    DiffuseContrib = litV.y * LightColor;
    SpecularContrib = litV.y * litV.z * Ks * LightColor;
}

vertexOutput std_VS(appdata IN) {
	vertexOutput OUT = (vertexOutput)0;
    OUT.WorldNormal = mul(IN.Normal,WorldITXf).xyz;
    OUT.WorldTangent = mul(IN.Tangent,WorldITXf).xyz;
    OUT.WorldBinormal = mul(IN.Binormal,WorldITXf).xyz;
    float4 Po = float4(IN.Position.xyz,1);
    float3 Pw = mul(Po,WorldXf).xyz;
	Lamp0Pos=float3(-20.5,-2.0,1.25);
    OUT.LightVec = (Lamp0Pos - Pw);
    OUT.UV = IN.UV.xy;
    OUT.WorldView = normalize(ViewIXf[3].xyz - Pw);
    OUT.HPosition = mul(Po,WvpXf);
	return OUT;
}

float4 std_PS(vertexOutput IN) : COLOR {
    float3 diffContrib;
    float3 specContrib;
    float3 Ln = normalize(IN.LightVec);
    float3 Vn = normalize(IN.WorldView);
	float3 Nn = normalize(IN.WorldNormal);
    float3 Tn = normalize(IN.WorldTangent);
    float3 Bn = normalize(IN.WorldBinormal);
	
    float3 bump = Bump * (tex2D(sNormalSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
    Nn = Nn + bump.x*Tn + bump.y*Bn;
    Nn = normalize(Nn);
	phong_shading(IN,Lamp0Color,Nn,Ln,Vn,diffContrib,specContrib);
	float3 diffuseColor = tex2D(DiffuseSampler,IN.UV).rgb;
	float3 result = specContrib+(diffuseColor*(diffContrib+AmbiColor));
    float3 R = -reflect(Vn,Nn);
    float3 reflColor = Kr * texCUBE(sEnvSampler,R.xyz).rgb * (diffContrib+AmbiColor);
	result += diffuseColor*reflColor;
	
	return float4(result,1);
}

technique Main{
    pass p0{
        VertexShader = compile vs_2_0 std_VS();
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = None;
        PixelShader = compile ps_2_a std_PS();
    }
}
