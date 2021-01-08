#version 330

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

// Attribute 0 is position, 1 is normal, 2 is tex coords.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;

// Normal (in world space)
out vec3 fragNormal;

// Position (in world space)
out vec3 fragWorldPos;

void main()
{
	//Convert position
	vec4 position = vec4(inPosition, 1.0);

	//Transform position to world space
	position = position * uWorldTransform;

	//Save world position
	fragWorldPos = position.xyz;

	//Transform to clip space
	gl_Position = position * uViewProjection;

	//Transform normal into world space (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	//Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
}