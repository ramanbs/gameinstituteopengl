#version 330 core

struct Material
{
	vec3 ambient;
	sampler2D diffuseMap;
	vec3 specular;
	float shininess;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//attenuation
	float constant;
	float linear;
	float exponent;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;

uniform PointLight light;
uniform Material material;
uniform vec3 viewPos; 

void main()
{
	// Ambient

	vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseMap, TexCoord)); // multiplying by texels so they are visible with there is only ambient light

	// Diffuse
	vec3 normal = normalize(Normal); // normalization could be lost due to interpolation of vertices 
	vec3 lightDir = normalize(light.position - FragPos); // negative as it is from the fragment to the light
	float NDotL = max(dot(normal, lightDir), 0.0f); // do not let it go to negative
	vec3 diffuse = light.diffuse * vec3(texture(material.diffuseMap, TexCoord)) * NDotL;
							
									
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Specular (Blinn-Phong Model)
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = light.specular * material.specular * pow(NDotH, material.shininess);

	float d = length(light.position - FragPos); 
	float attenuation = 1.0f / (light.constant + light.linear * d + light.exponent * (d * d));

	diffuse *= attenuation;
	specular *= attenuation;

	frag_color = vec4(ambient + diffuse + specular, 1.0f); // mix - does a linear interpolation between the colors of two texture witha certain blend factor(0.2).
};
