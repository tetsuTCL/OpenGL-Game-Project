#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoordinator;

out vec2 fragTexCoordinator;

void main()
{
    vec4 position = vec4(inPosition, 1.0);

    gl_Position = position * uWorldTransform * uViewProjection;

    fragTexCoordinator = inTexCoordinator;
}