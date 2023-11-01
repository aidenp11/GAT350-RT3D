#version 430

in layout(location = 0) vec3 position;

out layout(location = 0) vec3 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	otexcoord = position;

	mat4 mvp = projection * view;
	gl_Position = mvp * vec4(position, 1.0);
}
