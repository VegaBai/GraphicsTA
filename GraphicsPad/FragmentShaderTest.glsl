#version 430

in vec3 theColor;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_uvPosition;
in vec3 v_tangent;
out vec4 daColor;

uniform vec3 ambientLight;
uniform vec3 diffuseLightPosition;
uniform mat4 modelToWorldTransform;
uniform vec3 eyePositionWorld;
uniform mat4 modelToWorldInvertTrans;

uniform sampler2D Tex1;
uniform sampler2D Normal1;
uniform sampler2D Spec1;
uniform sampler2D Alpha1;
uniform sampler2D Ao1;

void main()
{
	vec3 vertexWorld = vec3(modelToWorldTransform * vec4(v_position, 1.0));
	vec3 normalWorld = normalize(vec3(modelToWorldInvertTrans * vec4(v_normal, 1.0f)));

	// normal map
	vec3 tangentWorld = normalize(vec3(modelToWorldTransform * vec4(v_tangent, 0.0f)));
	vec3 bitangentWorld = cross(normalWorld, tangentWorld);
	mat3 tbn = mat3(tangentWorld, bitangentWorld, normalWorld);

	vec3 normalMap = vec3(texture(Normal1, v_uvPosition));
	normalMap = (normalMap * 2.0 - 1.0);
	normalWorld = normalize(tbn * normalMap);

	// diffuse light calculation
	vec3 lightVectorWorld = normalize(diffuseLightPosition - vertexWorld);
	float diffuseIntensity = dot(lightVectorWorld, normalWorld);
	vec3 diffuse = vec3(diffuseIntensity, diffuseIntensity, diffuseIntensity);

	// specular light calculation
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexWorld);
	float specularity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specularity = pow(specularity, 50);
	vec3 specMap = vec3(texture(Spec1, v_uvPosition))*0.5f;
	vec3 specIntense = vec3(specularity, specularity, specularity);
	vec3 specularLight = specMap * specIntense;

	// attenuation
	float d = length(diffuseLightPosition - vertexWorld);
	float k1 = 1.0;
	float k2 = 0.1;
	float k3 = 0.05;

	vec3 diffAttn = clamp(diffuse, 0.0, 1.0) * (1.0/(k1 + k2*d + k3*d*d));
	vec3 lightColor = (ambientLight + diffAttn + clamp(specIntense, 0, 1));//*specMap);

	// texture
	vec4 texColor = texture(Tex1, v_uvPosition);

	// ambient occlusion
	vec3 aoColor = vec3(texture(Ao1, v_uvPosition));

	daColor = vec4(vec3(texColor) * lightColor * aoColor, texColor[3]);
}