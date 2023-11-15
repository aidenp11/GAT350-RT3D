#version 430

#define INVERT_MASK		(1 << 0)
#define GRAYSCALE_MASK	(1 << 1)
#define TINT_MASK		(1 << 2)
#define GRAIN_MASK		(1 << 3)
#define SCANLINE_MASK	(1 << 4)
#define CUSTOM_MASK		(1 << 5)

//in layout(location = 0) vec3 color;
in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tintColor;
uniform float grainAmount;
uniform float scanlineIntensity;
uniform float kernel[9];
uniform float texelSizeX;
uniform float texelSizeY;

layout(binding = 0) uniform sampler2D screenTexture;

vec4 invert(in vec4 color)
{
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale (in vec4 color)
{
	return vec4(vec3((color.r + color.b + color.g) / 3), color.a);
}

vec4 tint (in vec4 color)
{
	return vec4(color.rgb * tintColor, color.a);
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 grain (in vec4 color)
{
	vec3 noise = vec3(rand(ftexcoord * grainAmount));
    vec3 grain1 = mix(vec3(0.0), noise, grainAmount);
    return vec4(color.rgb + grain1, color.a);
}

vec4 scanline (in vec4 color)
{
	float scanline = sin(ftexcoord.y * 1000.0) * scanlineIntensity;
    vec3 scanlineColor = vec3(0.0); 
    
    vec3 fcolor = mix(color.rgb, scanlineColor, scanline);
    return vec4(fcolor, color.a);
}

vec4 custom (in vec4 color)
{
        vec2 texelSize = vec2(texelSizeX, texelSizeY);
        vec3 result = vec3(0.0);

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                vec2 offset = vec2(float(i), float(j)) * texelSize;
                result += texture(screenTexture, ftexcoord + offset).rgb * kernel[(i + 1) * 3 + (j + 1)];
            }
        }

        return vec4(result, 1.0);
}
void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))			 postprocess = invert(postprocess);
	if (bool(params & GRAYSCALE_MASK))		 postprocess = grayscale(postprocess);
	if (bool(params & TINT_MASK))			 postprocess = tint(postprocess);
	if (bool(params & GRAIN_MASK))			 postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK))		 postprocess = scanline(postprocess);
	if (bool(params & CUSTOM_MASK))			 postprocess = custom(postprocess);


	ocolor = mix(basecolor, postprocess, blend);
}
