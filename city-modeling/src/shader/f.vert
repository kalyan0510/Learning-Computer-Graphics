// Vertex Shader

#version 410 core

void main()
{
	const vec4 vertices[] =
		vec4[] ( vec4(  0.0,  0.8, 0.5, 1.0 ),
				 vec4( -0.8, -0.8, 0.5, 1.0 ),
				 vec4(  0.8, -0.8, 0.5, 1.0 ) );

	gl_Position = vertices[gl_VertexID];
}