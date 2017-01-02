const char* Color3D_vert = R"(
#version 420
layout(location = 0) in vec3 vertex_position;
layout(std140, binding = 0) uniform SharedTransformUniforms
{
    mat4 ModelView;
    mat4 ModelViewProjection;
    mat4 UIProjection;
};

void main()
{
    gl_Position = ModelViewProjection * vec4(vertex_position, 1.0);
}
)";

const char* Color3D_frag = R"(
#version 420
out vec4 frag_colour;

void main()
{
    frag_colour = vec4(1.0, 0.0, 0.0, 1.0);
}
)";