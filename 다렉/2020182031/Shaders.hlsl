#define PARTICLE_TYPE_EMITTER		0
#define PARTICLE_TYPE_SHELL			1
#define PARTICLE_TYPE_FLARE01		2
#define PARTICLE_TYPE_FLARE02		3
#define PARTICLE_TYPE_FLARE03		4

#define SHELL_PARTICLE_LIFETIME		3.0f
#define FLARE01_PARTICLE_LIFETIME	2.5f
#define FLARE02_PARTICLE_LIFETIME	1.5f
#define FLARE03_PARTICLE_LIFETIME	2.0f
struct MATERIAL
{
	float4					m_cAmbient;
	float4					m_cDiffuse;
	float4					m_cSpecular; //a = power
	float4					m_cEmissive;
};

cbuffer cbCubeMappingRender : register(b0)
{
	matrix gmtxCubeMappingViews[6];
	matrix gmtxCubeMappingProjection;
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	matrix		gmtxInverseView : packoffset(c8);
	float3		gvCameraPosition : packoffset(c12);

};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	MATERIAL	gMaterial : packoffset(c4);
	uint		gnTexturesMask : packoffset(c8);
};

cbuffer cbFrameworkInfo : register(b3)
{
	float		gfCurrentTime : packoffset(c0.x);
	float		gfElapsedTime : packoffset(c0.y);
	float		gfSecondsPerFirework : packoffset(c0.z);
	int			gnFlareParticlesToEmit : packoffset(c0.w);;
	float3		gf3Gravity : packoffset(c1.x);
	int			gnMaxFlareType2Particles : packoffset(c1.w);;
};

#include "Light.hlsl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#define _WITH_VERTEX_LIGHTING

#define MATERIAL_ALBEDO_MAP			0x01
#define MATERIAL_SPECULAR_MAP		0x02
#define MATERIAL_NORMAL_MAP			0x04
#define MATERIAL_METALLIC_MAP		0x08
#define MATERIAL_EMISSION_MAP		0x10
#define MATERIAL_DETAIL_ALBEDO_MAP	0x20
#define MATERIAL_DETAIL_NORMAL_MAP	0x40


#define _WITH_STANDARD_TEXTURE_MULTIPLE_DESCRIPTORS

#ifdef _WITH_STANDARD_TEXTURE_MULTIPLE_DESCRIPTORS
Texture2D gtxtTexture : register(t0);
Texture2D gtxtAlbedoTexture : register(t6);
Texture2D gtxtSpecularTexture : register(t7);
Texture2D gtxtNormalTexture : register(t8);
Texture2D gtxtMetallicTexture : register(t9);
Texture2D gtxtEmissionTexture : register(t10);
Texture2D gtxtDetailAlbedoTexture : register(t11);
Texture2D gtxtDetailNormalTexture : register(t12);
TextureCube gtxtSkyCubeTexture : register(t13);
Texture2D gtxtTerrainBaseTexture : register(t14);
Texture2D gtxtTerrainDetailTexture : register(t15);
Texture2D gtxtWaterTexture : register(t16);

Texture2D<float4> gtxtParticleTexture : register(t17);
Buffer<float4> gRandomBuffer : register(t18);
Buffer<float4> gRandomSphereBuffer : register(t19);
TextureCube gtxtCubeMap : register(t20);
#else
Texture2D gtxtStandardTextures[7] : register(t6);
#endif

SamplerState gssWrap : register(s0);
SamplerState gssClamp : register(s1);
SamplerState gMirrorSamplerState : register(s2);
SamplerState gPointSamplerState : register(s3);
//SamplerState gssCubeMap : register(s4);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_STANDARD_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_STANDARD_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
	float3 bitangentW : BITANGENT;
	float2 uv : TEXCOORD;
};

struct VS_TERRAIN_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TERRAIN_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
};

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

struct VS_Water_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_Water_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

struct VS_SKYBOX_CUBEMAP_INPUT
{
	float3 position : POSITION;
};

struct VS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
};

struct GS_SKYBOX_CUBEMAP_OUTPUT
{
	float3	positionL : POSITION;
	float4	position : SV_POSITION;
	uint renderTarget : SV_RenderTargetArrayIndex;
};
struct VS_PARTICLE_INPUT
{
	float3 position : POSITION;
	float3 velocity : VELOCITY;
	float lifetime : LIFETIME;
	uint type : PARTICLETYPE;
};
struct VS_PARTICLE_DRAW_OUTPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float size : SCALE;
	uint type : PARTICLETYPE;
};

struct GS_PARTICLE_DRAW_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXTURE;
	uint type : PARTICLETYPE;
};
struct VS_LIGHTING_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_LIGHTING_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


VS_SKYBOX_CUBEMAP_OUTPUT VSSkyBox(VS_SKYBOX_CUBEMAP_INPUT input)
{
	VS_SKYBOX_CUBEMAP_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.positionL = input.position;

	return(output);
}

float4 PSSkyBox(VS_SKYBOX_CUBEMAP_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtSkyCubeTexture.Sample(gssClamp, input.positionL);

	return(cColor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_LIGHTING_OUTPUT VSLightingColor(VS_LIGHTING_INPUT input)
{
	VS_LIGHTING_OUTPUT output = (VS_LIGHTING_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.positionW = mul(input.position, (float3x3)gmtxGameObject);
	output.position = mul(float4(input.position,1.0f), mul(gmtxView, gmtxProjection));	
	return(output);
}

float4 PSCubeMapping(VS_LIGHTING_OUTPUT input) : SV_Target
{
	float4 clIlumination = Lighting(input.positionW, input.normalW);
	float3 vFromCamera = normalize(input.positionW - gvCameraPosition.xyz);
	float3 vReflected = normalize(reflect(vFromCamera, input.normalW));
	float4 cCubeTextureColor = gtxtCubeMap.Sample(gssClamp, vReflected);
	return(clIlumination * cCubeTextureColor);
}


VS_SKYBOX_CUBEMAP_OUTPUT VSToCubeMap(VS_SKYBOX_CUBEMAP_INPUT input)
{
	VS_SKYBOX_CUBEMAP_OUTPUT output = (VS_SKYBOX_CUBEMAP_OUTPUT)0;
	output.position = mul(float4(input.position, 1.0f), gmtxGameObject);
	output.positionL = input.position;
	return(output);
}

[maxvertexcount(3)]
[Instance(6)]
void GSSkyBox(triangle VS_SKYBOX_CUBEMAP_OUTPUT input[3], uint nInstance: SV_GSInstanceID, inout TriangleStream<GS_SKYBOX_CUBEMAP_OUTPUT> stream)
{
	GS_SKYBOX_CUBEMAP_OUTPUT output;
	output.renderTarget = nInstance;
	matrix mtxView = mul(gmtxCubeMappingViews[nInstance], gmtxCubeMappingProjection); 
	for (int j = 0; j < 3; j++)
	{
		output.position = mul(input[j].position, mtxView);
		output.positionL = input[j].positionL;
		stream.Append(output);
	}
	stream.RestartStrip();
}

float4 PSToCubeMap(GS_SKYBOX_CUBEMAP_OUTPUT input) : SV_Target
{
	float4 cColor = gtxtSkyCubeTexture.Sample(gssClamp, input.positionL);
	return(cColor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_STANDARD_OUTPUT VSStandard(VS_STANDARD_INPUT input)
{
	VS_STANDARD_OUTPUT output;

	output.positionW = (float3)mul(float4(input.position, 1.0f), gmtxGameObject);
	output.normalW = mul(input.normal, (float3x3)gmtxGameObject);
	output.tangentW = (float3)mul(float4(input.tangent, 1.0f), gmtxGameObject);
	output.bitangentW = (float3)mul(float4(input.bitangent, 1.0f), gmtxGameObject);
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection); 
	output.uv = input.uv;

	return(output);
}

float4 PSStandard(VS_STANDARD_OUTPUT input) : SV_TARGET
{
	float4 cAlbedoColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 cSpecularColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 cNormalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 cMetallicColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 cEmissionColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef _WITH_STANDARD_TEXTURE_MULTIPLE_DESCRIPTORS
	if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtAlbedoTexture.Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtSpecularTexture.Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtNormalTexture.Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtMetallicTexture.Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtEmissionTexture.Sample(gssWrap, input.uv);
#else
	if (gnTexturesMask & MATERIAL_ALBEDO_MAP) cAlbedoColor = gtxtStandardTextures[0].Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_SPECULAR_MAP) cSpecularColor = gtxtStandardTextures[1].Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_NORMAL_MAP) cNormalColor = gtxtStandardTextures[2].Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_METALLIC_MAP) cMetallicColor = gtxtStandardTextures[3].Sample(gssWrap, input.uv);
	if (gnTexturesMask & MATERIAL_EMISSION_MAP) cEmissionColor = gtxtStandardTextures[4].Sample(gssWrap, input.uv);
#endif

	float4 cIllumination = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 cColor = cAlbedoColor + cSpecularColor + cEmissionColor;
	if (gnTexturesMask & MATERIAL_NORMAL_MAP)
	{
		float3 normalW = input.normalW;
		float3x3 TBN = float3x3(normalize(input.tangentW), normalize(input.bitangentW), normalize(input.normalW));
		float3 vNormal = normalize(cNormalColor.rgb * 2.0f - 1.0f); //[0, 1] ¡æ [-1, 1]
		normalW = normalize(mul(vNormal, TBN));
		cIllumination = Lighting(input.positionW, normalW);
		cColor = lerp(cColor, cIllumination, 0.5f);
	}

	return(cColor);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


VS_TEXTURED_OUTPUT VSTextured(VS_TEXTURED_INPUT input)
{
	VS_TEXTURED_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}


float4 PSTextured(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gssClamp, input.uv);
	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//


VS_Water_OUTPUT VSWater(VS_Water_INPUT input)
{
	VS_Water_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.uv = input.uv;

	return(output);
}


float4 PSWater(VS_Water_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtWaterTexture.Sample(gssWrap, input.uv*5);
	return(cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_TERRAIN_OUTPUT VSTerrain(VS_TERRAIN_INPUT input)
{
	VS_TERRAIN_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);
	output.color = input.color;
	output.uv0 = input.uv0;
	output.uv1 = input.uv1;

	return(output);
}

float4 PSTerrain(VS_TERRAIN_OUTPUT input) : SV_TARGET
{
	float4 cBaseTexColor = gtxtTerrainBaseTexture.Sample(gssWrap, input.uv0);
	float4 cDetailTexColor = gtxtTerrainDetailTexture.Sample(gssWrap, float2(input.uv1.x/6, input.uv1.y/3));
	float4 cColor = (cBaseTexColor * 1.0f) + (float4(cDetailTexColor.rrr,1.f) * 0.2f);

	return(cColor);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_PARTICLE_INPUT VSParticleStreamOutput(VS_PARTICLE_INPUT input)
{
	return(input);
}

float3 GetParticleColor(float fAge, float fLifetime)
{
	float3 cColor = float3(1.0f, 1.0f, 1.0f);

	if (fAge == 0.0f) cColor = float3(0.0f, 1.0f, 0.0f);
	else if (fLifetime == 0.0f)
		cColor = float3(1.0f, 1.0f, 0.0f);
	else
	{
		float t = fAge / fLifetime;
		cColor = lerp(float3(1.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), t * 1.0f);
	}

	return(cColor);
}

void GetBillboardCorners(float3 position, float2 size, out float4 pf4Positions[4])
{
	float3 f3Up = float3(0.0f, 1.0f, 0.0f);
	float3 f3Look = normalize(gvCameraPosition - position);
	float3 f3Right = normalize(cross(f3Up, f3Look));

	pf4Positions[0] = float4(position + size.x * f3Right - size.y * f3Up, 1.0f);
	pf4Positions[1] = float4(position + size.x * f3Right + size.y * f3Up, 1.0f);
	pf4Positions[2] = float4(position - size.x * f3Right - size.y * f3Up, 1.0f);
	pf4Positions[3] = float4(position - size.x * f3Right + size.y * f3Up, 1.0f);
}

void GetPositions(float3 position, float2 f2Size, out float3 pf3Positions[8])
{
	float3 f3Right = float3(1.0f, 0.0f, 0.0f);
	float3 f3Up = float3(0.0f, 1.0f, 0.0f);
	float3 f3Look = float3(0.0f, 0.0f, 1.0f);

	float3 f3Extent = normalize(float3(1.0f, 1.0f, 1.0f));

	pf3Positions[0] = position + float3(-f2Size.x, 0.0f, -f2Size.y);
	pf3Positions[1] = position + float3(-f2Size.x, 0.0f, +f2Size.y);
	pf3Positions[2] = position + float3(+f2Size.x, 0.0f, -f2Size.y);
	pf3Positions[3] = position + float3(+f2Size.x, 0.0f, +f2Size.y);
	pf3Positions[4] = position + float3(-f2Size.x, 0.0f, 0.0f);
	pf3Positions[5] = position + float3(+f2Size.x, 0.0f, 0.0f);
	pf3Positions[6] = position + float3(0.0f, 0.0f, +f2Size.y);
	pf3Positions[7] = position + float3(0.0f, 0.0f, -f2Size.y);
}

float4 RandomDirection(float fOffset)
{
	int u = uint(gfCurrentTime + fOffset + frac(gfCurrentTime) * 1000.0f) % 1024;
	return(normalize(gRandomBuffer.Load(u)));
}

float4 RandomDirectionOnSphere(float fOffset)
{
	int u = uint(gfCurrentTime + fOffset + frac(gfCurrentTime) * 1000.0f) % 256;
	return(normalize(gRandomSphereBuffer.Load(u)));
}

void OutputParticleToStream(VS_PARTICLE_INPUT input, inout PointStream<VS_PARTICLE_INPUT> output)
{
	input.position += input.velocity * gfElapsedTime;
	input.velocity += gf3Gravity * gfElapsedTime;
	input.lifetime -= gfElapsedTime;

	output.Append(input);
}

void EmmitParticles(VS_PARTICLE_INPUT input, inout PointStream<VS_PARTICLE_INPUT> output)
{
	float4 f4Random = RandomDirection(input.type);
	if (input.lifetime <= 0.0f)
	{
		VS_PARTICLE_INPUT particle = input;

		particle.type = PARTICLE_TYPE_SHELL;
		particle.position = input.position + (input.velocity * gfElapsedTime * f4Random.xyz);
		particle.velocity = input.velocity + (f4Random.xyz * 16.0f);
		particle.lifetime = SHELL_PARTICLE_LIFETIME + (f4Random.y * 0.5f);

		output.Append(particle);

		input.lifetime = gfSecondsPerFirework * 0.2f + (f4Random.x * 0.4f);
	}
	else
	{
		input.lifetime -= gfElapsedTime;
	}

	output.Append(input);
}

void ShellParticles(VS_PARTICLE_INPUT input, inout PointStream<VS_PARTICLE_INPUT> output)
{
	if (input.lifetime <= 0.0f)
	{
		VS_PARTICLE_INPUT particle = input;
		float4 f4Random = float4(0.0f, 0.0f, 0.0f, 0.0f);

		particle.type = PARTICLE_TYPE_FLARE01;
		particle.position = input.position + (input.velocity * gfElapsedTime * 2.0f);
		particle.lifetime = FLARE01_PARTICLE_LIFETIME;

		for (int i = 0; i < gnFlareParticlesToEmit; i++)
		{
			f4Random = RandomDirection(input.type + i);
			particle.velocity = input.velocity + (f4Random.xyz * 18.0f);

			output.Append(particle);
		}

		particle.type = PARTICLE_TYPE_FLARE02;
		particle.position = input.position + (input.velocity * gfElapsedTime);
		for (int j = 0; j < abs(f4Random.x) * gnMaxFlareType2Particles; j++)
		{
			f4Random = RandomDirection(input.type + j);
			particle.velocity = input.velocity + (f4Random.xyz * 10.0f);
			particle.lifetime = FLARE02_PARTICLE_LIFETIME + (f4Random.x * 0.4f);

			output.Append(particle);
		}
	}
	else
	{
		OutputParticleToStream(input, output);
	}
}

void OutputEmberParticles(VS_PARTICLE_INPUT input, inout PointStream<VS_PARTICLE_INPUT> output)
{
	if (input.lifetime > 0.0f)
	{
		OutputParticleToStream(input, output);
	}
}

void GenerateEmberParticles(VS_PARTICLE_INPUT input, inout PointStream<VS_PARTICLE_INPUT> output)
{
	if (input.lifetime <= 0.0f)
	{
		VS_PARTICLE_INPUT particle = input;

		particle.type = PARTICLE_TYPE_FLARE03;
		particle.position = input.position + (input.velocity * gfElapsedTime);
		particle.lifetime = FLARE03_PARTICLE_LIFETIME;
		for (int i = 0; i < 64; i++)
		{
			float4 f4Random = RandomDirectionOnSphere(input.type + i);
			particle.velocity = input.velocity + (f4Random.xyz * 25.0f);

			output.Append(particle);
		}
	}
	else
	{
		OutputParticleToStream(input, output);
	}
}

[maxvertexcount(128)]
void GSParticleStreamOutput(point VS_PARTICLE_INPUT input[1], inout PointStream<VS_PARTICLE_INPUT> output)
{
	VS_PARTICLE_INPUT particle = input[0];

	if (particle.type == PARTICLE_TYPE_EMITTER) EmmitParticles(particle, output);
	else if (particle.type == PARTICLE_TYPE_SHELL) ShellParticles(particle, output);
	else if ((particle.type == PARTICLE_TYPE_FLARE01) || (particle.type == PARTICLE_TYPE_FLARE03)) OutputEmberParticles(particle, output);
	else if (particle.type == PARTICLE_TYPE_FLARE02) GenerateEmberParticles(particle, output);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VS_PARTICLE_DRAW_OUTPUT VSParticleDraw(VS_PARTICLE_INPUT input)
{
	VS_PARTICLE_DRAW_OUTPUT output = (VS_PARTICLE_DRAW_OUTPUT)0;

	output.position = input.position;
	output.size = 2.5f;
	output.type = input.type;

	if (input.type == PARTICLE_TYPE_EMITTER) { output.color = float4(1.0f, 0.1f, 0.1f, 1.0f); output.size = 3.0f; }
	else if (input.type == PARTICLE_TYPE_SHELL) { output.color = float4(0.1f, 0.0f, 1.0f, 1.0f); output.size = 3.0f; }
	else if (input.type == PARTICLE_TYPE_FLARE01) { output.color = float4(1.0f, 1.0f, 0.1f, 1.0f); output.color *= (input.lifetime / FLARE01_PARTICLE_LIFETIME); }
	else if (input.type == PARTICLE_TYPE_FLARE02) output.color = float4(1.0f, 0.1f, 1.0f, 1.0f);
	else if (input.type == PARTICLE_TYPE_FLARE03) { output.color = float4(1.0f, 0.1f, 1.0f, 1.0f); output.color *= (input.lifetime / FLARE03_PARTICLE_LIFETIME); }

	return(output);
}

static float3 gf3Positions[4] = { float3(-1.0f, +1.0f, 0.5f), float3(+1.0f, +1.0f, 0.5f), float3(-1.0f, -1.0f, 0.5f), float3(+1.0f, -1.0f, 0.5f) };
static float2 gf2QuadUVs[4] = { float2(0.0f, 0.0f), float2(1.0f, 0.0f), float2(0.0f, 1.0f), float2(1.0f, 1.0f) };

[maxvertexcount(4)]
void GSParticleDraw(point VS_PARTICLE_DRAW_OUTPUT input[1], inout TriangleStream<GS_PARTICLE_DRAW_OUTPUT> outputStream)
{
	GS_PARTICLE_DRAW_OUTPUT output = (GS_PARTICLE_DRAW_OUTPUT)0;

	output.type = input[0].type;
	output.color = input[0].color;
	for (int i = 0; i < 4; i++)
	{
		float3 positionW = mul(gf3Positions[i] * input[0].size, (float3x3)gmtxInverseView) + input[0].position;
		output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
		output.uv = gf2QuadUVs[i];

		outputStream.Append(output);
	}
	outputStream.RestartStrip();
}

float4 PSParticleDraw(GS_PARTICLE_DRAW_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtParticleTexture.Sample(gssWrap, input.uv);
	cColor *= input.color;

	return(cColor);
}
