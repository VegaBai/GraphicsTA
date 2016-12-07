#version 430

in vec3 theColor;
in vec3 v_position;
in vec3 v_normal;
in vec2 v_uvPosition;
out vec4 daColor;

uniform vec3 ambientLight;
uniform vec3 diffuseLightPosition;
uniform mat4 modelToWorldTransform;
uniform vec3 eyePositionWorld;

uniform sampler2D Normal1;

void main()
{
	vec3 vertexWorld = vec3(modelToWorldTransform * vec4(v_position, 1.0));
	vec3 normalWorld = normalize(vec3(modelToWorldTransform * vec4(v_normal, 0.0f)));

	// normal map
	mat3 normalTangentTransform;
	normalTangentTransform[0] = vec3(1.0, 0.0, 0.0);
	normalTangentTransform[1] = vec3(0.0, 1.0, 0.0);
	float scaleNormal = 1.0f/normalWorld[2];

	normalTangentTransform[2] = normalize(normalWorld * vec3(scaleNormal, scaleNormal, scaleNormal));
	vec3 normalMap = normalize(vec3(texture(Normal1, v_uvPosition)));
	vec3 normalWorldFinal = normalize( normalTangentTransform * normalMap) ;
//	if(normalWorld[1] <0)
//	{
//		normalWorldFinal*= vec3(1.0f, -1.0f, 1.0f);
//	}  
//	if(normalWorld[2] <0)
//	{
//		normalWorldFinal*= vec3(1.0f, 1.0f, -1.0f);
//	}  

	normalWorld = normalWorldFinal;



	// diffuse light calculation
	vec3 lightVectorWorld = normalize(diffuseLightPosition - vertexWorld);
	float diffuseIntensity = dot(lightVectorWorld, normalWorld);
	vec3 diffuse = vec3(0.0, diffuseIntensity, 0.0);

	// specular light calculation
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexWorld);
	float specularity = dot(reflectedLightVectorWorld, eyeVectorWorld);
	specularity = pow(specularity, 50);
	vec3 specularLight = vec3(0.0, 0.0, specularity);

	// attenuation
	float d = length(diffuseLightPosition - vertexWorld);
	float k1 = 1.0;
	float k2 = 0.1;
	float k3 = 0.05;

	vec3 diffAttn = clamp(diffuse, 0.0, 1.0) * (1.0/(k1 + k2*d + k3*d*d));
	vec3 someLight = ambientLight + diffAttn;

//	daColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	daColor = vec4(someLight + specularLight, 1.0f);
//	daColor = vec4(texture(Normal1, v_uvPosition));//, 1.0f);
}