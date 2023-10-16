#version 430

//in layout(location = 0) vec3 color;
in layout(location = 1) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex1;
layout(binding = 1) uniform sampler2D tex2;

void main()
{
	vec4 texcolor1 = texture(tex1, (tiling * texcoord) + offset);
	vec4 texcolor2 = texture(tex2, (tiling * texcoord) + offset);
	//if (texcolor.a < 0.8) discard;
	vec4 texcolor = mix(texcolor1, texcolor2, 0.7);

	ocolor = texcolor1 * color;
}
