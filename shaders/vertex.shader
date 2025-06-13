#version 460 core

layout(location = 0) in vec2 aPos;

out vec2 fragCoord;

void main()
{
    fragCoord = aPos * 0.5f + 0.5f;

    gl_Position = vec4(aPos, 0.f, 1.f);
}