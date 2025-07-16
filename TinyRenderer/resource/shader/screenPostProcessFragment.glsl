#version 330 core
#define OFFSET (1.0f/300.0f)
#define NoneType (0)
#define	GreyType (1)
#define	InvertType (2)
#define	SharpenType (3)
#define	BlurType (4)
#define	EdgeType (5)

in vec2 uv;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform int type;

vec4 GreyColor();
vec4 InvertColor();
vec4 SharpenColor(vec2 offset[9]);
vec4 BlurColor(vec2 offset[9]);
vec4 EdgeColor(vec2 offset[9]);

void main()
{
	vec2 offset[9] = vec2[](
		vec2(-OFFSET, OFFSET),
		vec2(      0, OFFSET),
		vec2( OFFSET, OFFSET),
		vec2(-OFFSET,      0),
		vec2(      0,      0),
		vec2( OFFSET,	   0),
		vec2(-OFFSET,-OFFSET),
		vec2(      0,-OFFSET),
		vec2( OFFSET,-OFFSET)
	);

	vec4 color = vec4(0.0f);
	switch(type)
	{
		case NoneType:
			color = texture(screenTexture,uv);
			break;

		case GreyType:
			color = GreyColor();
			break;

		case InvertType:
			color = InvertColor();
			break;

		case SharpenType:
			color = SharpenColor(offset);
			break;

		case BlurType:
			color = BlurColor(offset);
			break;

		case EdgeType:
			color = EdgeColor(offset);
			break;
	}
	FragColor = color;
}

vec4 GreyColor()
{
	vec4 color = texture(screenTexture,uv);
	float grey = 0.2126*color.r + 0.7152*color.g + 0.0722*color.b;
	return vec4(grey, grey, grey, color.a);
}
vec4 InvertColor()
{
	return 1 - texture(screenTexture,uv);
}
vec4 SharpenColor(vec2 offset[9])
{
	float kernel[9] = float[](
		-1.0f, -1.0f, -1.0f,
		-1.0f,  9.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	);

	vec4 color = vec4(0.0f);
	for(int i = 0;i < 9;i++)
	{
		color += kernel[i] * texture(screenTexture,uv + offset[i]);
	}
	return color;
}
vec4 BlurColor(vec2 offset[9])
{
	float kernel[9] = float[](
		 1.0f/16.0f,  2.0f/16.0f,  1.0f/16.0f,
		 2.0f/16.0f,  4.0f/16.0f,  2.0f/16.0f,
		 1.0f/16.0f,  2.0f/16.0f,  1.0f/16.0f
	);

	vec4 color = vec4(0.0f);
	for(int i = 0;i < 9;i++)
	{
		color += kernel[i] * texture(screenTexture,uv + offset[i]);
	}
	return color;
}
vec4 EdgeColor(vec2 offset[9])
{
float kernel[9] = float[](
		1.0f,  1.0f, 1.0f,
		1.0f, -8.0f, 1.0f,
		1.0f,  1.0f, 1.0f
	);

	vec4 color = vec4(0.0f);
	for(int i = 0;i < 9;i++)
	{
		color += kernel[i] * texture(screenTexture,uv + offset[i]);
	}
	return color;
}