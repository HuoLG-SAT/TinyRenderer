#version 330 core
#define MAX_POINT_NUM 5
#define POINT_LIGHT_FAR 25.0f

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 diffuse;
	vec3 specular;

	//��Ӱӳ��
	sampler2D shadowMap;
};

struct PointLight
{
	vec3 position;

	vec3 diffuse;
	vec3 specular;

	float constant;  
	float linear;	 
	float quadratic;

	//��Ӱӳ��
	samplerCube shadowMap;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 diffuse;
	vec3 specular;

	float inCutOff;	
	float outCutOff;

	//��Ӱӳ��
	sampler2D shadowMap;
};

in V2F
{
	vec3 worldNormal;
	vec3 worldPos;
	vec2 uv;
	vec3 cameraPos;
	vec4 directionalLightSpacePos;
	vec4 spotLightSpacePos;
} v2f;

out vec4 FragColor;

uniform bool isEnableDirectionalLightShadowCaster;
uniform bool isEnablePointLightShadowCaster;
uniform bool isEnableSpotLightShadowCaster;
uniform bool isOpenDirectionalLight;
uniform bool isOpenPointLight;
uniform bool isOpenSpotLight;
uniform bool isOpenAmbient;
uniform int pointLightNum;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_NUM];
uniform SpotLight spotLight;
uniform vec3 ambient;

vec3 CalculateDirectionalLight(DirectionalLight directionalLight,vec3 normal,vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor);
vec3 CalculatePointLight(PointLight pointLight,vec3 normal, vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor);
vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal,vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor);

float CalculateShadowMapping(vec4 lightSpacePos, vec3 lightDir, sampler2D shadowMap);
float CalculateOmniShadowMapping(vec3 lightPos, vec3 lightDir, samplerCube shadowMap);

void main()
{
	vec3 viewDir = normalize(v2f.cameraPos - v2f.worldPos);
	vec3 materialDiffuseColor = material.diffuseColor * texture(material.diffuseMap,v2f.uv).rgb;
	vec3 materialSpecularColor = material.specularColor * texture(material.specularMap,v2f.uv).rgb;
	vec3 ambientColor = material.ambientColor * texture(material.diffuseMap,v2f.uv).rgb * ambient;
	vec3 normal = normalize(v2f.worldNormal);

	vec3 color = vec3(0.0f,0.0f,0.0f);
	if(isOpenDirectionalLight)
	{
		color += CalculateDirectionalLight(directionalLight,normal, viewDir, materialDiffuseColor, materialSpecularColor);
	}
	if(isOpenPointLight)
	{
		for(int i = 0;i < pointLightNum && i < MAX_POINT_NUM;i++)
		{
			color += CalculatePointLight(pointLights[i], normal,viewDir, materialDiffuseColor, materialSpecularColor);
		}
	}
	if(isOpenSpotLight)
	{
		color += CalculateSpotLight(spotLight,normal, viewDir, materialDiffuseColor, materialSpecularColor);
	}
	if(isOpenAmbient)
	{
		color += ambientColor;
	}

	FragColor = vec4(color ,1);
}

vec3 CalculateDirectionalLight(DirectionalLight directionalLight,vec3 normal,vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor)
{
	//���շ���
	vec3 lightDir = normalize(-directionalLight.direction);

	//��������ɫ = ��Դ��������ɫ * ������������ɫ * max(0,��׼�����������淨����������׼����Ĺ�Դ��������)
	vec3 lambert = directionalLight.diffuse * materialDiffuseColor * max(0, dot(normal,lightDir));

	//�߹ⷴ����ɫ = ��Դ�߹ⷴ����ɫ * ���ʸ߹ⷴ����ɫ * (max(0, ��׼����Ķ��㷨����������׼����İ������)��N����)
	//������� = �۲췽������ + ����ⷽ������
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 blinnPhong = directionalLight.specular * materialSpecularColor * pow(max(0,dot(halfDir,normal)),material.shininess);

	//ƽ�й���˥��
	float atten = 1.0f;

	//��Ӱ
	float shadow = isEnableDirectionalLightShadowCaster ? CalculateShadowMapping(v2f.directionalLightSpacePos,lightDir,directionalLight.shadowMap) : 0.0f;

	//���չ�����ɫ = ��������ɫ + �߹ⷴ����ɫ + ��������ɫ
	return (lambert + blinnPhong) * atten * (1 - shadow);
}
vec3 CalculatePointLight(PointLight pointLight,vec3 normal, vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor)
{
	//���շ���
	vec3 lightDir = normalize(pointLight.position - v2f.worldPos);

	//��������ɫ = ��Դ��������ɫ * ������������ɫ * max(0,��׼�����������淨����������׼����Ĺ�Դ��������)
	vec3 lambert = pointLight.diffuse * materialDiffuseColor * max(0, dot(normal, lightDir));

	//�߹ⷴ����ɫ = ��Դ�߹ⷴ����ɫ * ���ʸ߹ⷴ����ɫ * (max(0, ��׼����Ķ��㷨����������׼����İ������)��N����)
	//������� = �۲췽������ + ����ⷽ������
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 blinnPhong = pointLight.specular * materialSpecularColor * pow(max(0, dot(halfDir, normal)) ,material.shininess);

	//������Դ˥������
	float d = distance(v2f.worldPos, pointLight.position);
	float atten = 1 / (pointLight.constant + pointLight.linear*d + pointLight.quadratic*pow(d,2));

	//������Ӱ˥��
	float shadow = isEnablePointLightShadowCaster ? CalculateOmniShadowMapping(pointLight.position, lightDir, pointLight.shadowMap) : 0.0f;

	//���չ�����ɫ = ��������ɫ + �߹ⷴ����ɫ + ��������ɫ
	return (lambert + blinnPhong) * atten * (1 - shadow);
}
vec3 CalculateSpotLight(SpotLight spotLight,vec3 normal, vec3 viewDir,vec3 materialDiffuseColor,vec3 materialSpecularColor)
{
	//���շ���
	vec3 lightDir = normalize(spotLight.position - v2f.worldPos);

	//��������ɫ = ��Դ��������ɫ * ������������ɫ * max(0,��׼�����������淨����������׼����Ĺ�Դ��������)
	vec3 lambert = spotLight.diffuse * materialDiffuseColor * max(0, dot(normal, lightDir));

	//�߹ⷴ����ɫ = ��Դ�߹ⷴ����ɫ * ���ʸ߹ⷴ����ɫ * (max(0, ��׼����Ķ��㷨����������׼����İ������)��N����)
	//������� = �۲췽������ + ����ⷽ������
	vec3 halfDir = normalize(viewDir + lightDir);
	vec3 blinnPhong = spotLight.specular * materialSpecularColor * pow(max(0, dot(halfDir, normal)) ,material.shininess);

	//����۹��˥������
	float theta = dot(lightDir,normalize(-spotLight.direction));
	float atten = clamp((theta - cos(spotLight.outCutOff))/(cos(spotLight.inCutOff) - cos(spotLight.outCutOff)), 0, 1);

	//������Ӱ˥��
	float shadow = isEnableSpotLightShadowCaster ? CalculateShadowMapping(v2f.spotLightSpacePos,lightDir,spotLight.shadowMap) : 0.0f;

	return (lambert + blinnPhong) * atten * (1 - shadow);
}

float CalculateShadowMapping(vec4 lightSpacePos, vec3 lightDir ,sampler2D shadowMap)
{
	if(lightSpacePos.x < -lightSpacePos.w || lightSpacePos.x > lightSpacePos.w ||
	   lightSpacePos.y < -lightSpacePos.w || lightSpacePos.y > lightSpacePos.w ||
	   lightSpacePos.z < -lightSpacePos.w || lightSpacePos.z > lightSpacePos.w )
	{
		return 0.0f;
	}

	vec3 projCoord = lightSpacePos.xyz / lightSpacePos.w;
	projCoord = projCoord * 0.5 + 0.5;	//ӳ�䵽[0,1]��Χ��

	float shadow = 0.0f;
	float bias = max(0.005f, 0.05f * (1 - dot(v2f.worldNormal, lightDir)));
	float currentDepth = projCoord.z;
	vec2 texelSize = 1 / textureSize(shadowMap,0);
	for(int x = -1;x <= 1;x++)
	{
		for(int y = -1;y <= 1;y++)
		{
			float shadowMapDepth = texture(shadowMap, projCoord.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth > shadowMapDepth ? 1.0f : 0.0f;
		}
	}

	return shadow / 9;
}
float CalculateOmniShadowMapping(vec3 lightPos, vec3 lightDir, samplerCube shadowMap)
{
	vec3 LightToVertex = v2f.worldPos - lightPos;
	float currentDepth = length(LightToVertex);
	if(currentDepth > POINT_LIGHT_FAR)
	{
		return 0.0f; //�ȹ�ԴԶ�ü��滹Զ��ʾ�ù�Դ��������Ӱӳ����������������
	}

	vec3 offsets[20] = vec3[]
	(
		//������8������
		vec3( 1.0f, 1.0f, 1.0f), vec3( 1.0f, 1.0f, -1.0f), vec3( 1.0f, -1.0f, 1.0f), vec3( 1.0f, -1.0f, -1.0f),
		vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, 1.0f, -1.0f), vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, -1.0f, -1.0f),
		//XYƽ��
		vec3( 1.0f, 1.0f, 0.0f), vec3( 1.0f, -1.0f,  0.0f), vec3(-1.0f, 1.0f, 0.0f), vec3(-1.0f, -1.0f,  0.0f),
		//XZƽ��
		vec3( 1.0f, 0.0f, 1.0f), vec3( 1.0f, 0.0f, -1.0f), vec3(-1.0f,  0.0f, 1.0f), vec3(-1.0f,  0.0f, -1.0f),
		//YZƽ��
		vec3( 0.0f, 1.0f, 1.0f), vec3( 0.0f, 1.0f, -1.0f), vec3( 0.0f, -1.0f, 1.0f), vec3( 0.0f, -1.0f, -1.0f)
	);

	float shadow = 0.0f;
	float radius = 0.05f;
	float bias = max(0.005f, 0.05f * (1 - dot(v2f.worldNormal, lightDir)));
	for(int i = 0;i < 20;i++)
	{
		float shadowMapDepth = texture(shadowMap,LightToVertex + offsets[i] * radius).r * POINT_LIGHT_FAR;
		shadow += currentDepth > shadowMapDepth ? 1.0f : 0.0f;
	}

	return shadow / 20;
}