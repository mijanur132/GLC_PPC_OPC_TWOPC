#version 330 core

layout(location = 0) in vec2 pos;		// Model-space position
layout(location = 1) in vec2 tex_uv;	// Texture uv coordinates

// Passes the uv coordinates to the fragment shader.
// The uv coordinates will be automatically interpolated.
smooth out vec2 vsout_tex_uv;

void main() {
	// Transform vertex position
	gl_Position = vec4(pos, 0.0, 1.0);

	// Interpolate normals
	vsout_tex_uv = tex_uv;
}