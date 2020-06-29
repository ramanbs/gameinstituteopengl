#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 frag_color;
// in case of a single texture, the default location was 0, hence dont need to provide for the sampler uniform, but in multitexturing we need to assign the location
uniform sampler2D myTexture; //TODO built in sampler by GLSL. Have to find out more types of sampler available. 
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; 

void main()
{
	// Ambient
	float ambientFactor = 0.1f;
	vec3 ambient = lightColor * ambientFactor;

	// Diffuse
	vec3 normal = normalize(Normal); // normalization could be lost due to interpolation of vertices 
	vec3 lightDir = normalize(lightPos - FragPos);
	float NDotL = max(dot(normal, lightDir), 0.0f); // do not let it go to negative
	vec3 diffuse = lightColor * NDotL;
							
									
	// Specular
	float specularFactor = 0.8f; // determines intensity
	float shininess = 100.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	// Specular (Phong model)
	//vec3 reflectDir =  (-lightDir, normal); //TODO : Why -lightDir ? Ans : the lightDir is goinf towards the surface but the reflection vector moves away from the surface hence the negative
	//float RDotV = max(dot(reflectDir, viewDir), 0.0f);
	//vec3 specular = lightColor * specularFactor * pow(RDotV, shininess);

	// Specular (Blinn-Phong Model)
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = lightColor * specularFactor * pow(NDotH, shininess);


	vec4 texel = texture(myTexture, TexCoord);
	frag_color = vec4(ambient + diffuse + specular, 1.0f) * texel; // mix - does a linear interpolation between the colors of two texture witha certain blend factor(0.2).
};
