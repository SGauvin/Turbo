#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 cameraPosition;
uniform sampler2D texture1;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_textCoord;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 ambient = material.ambient * lightColor;
    
    vec3 result = ambient * light.ambient;

    vec3 normal = normalize(v_normal);
    vec3 lightDirection = normalize(light.position - v_position);

    float diffuseImpact = dot(lightDirection, normal);

    if (diffuseImpact > 0.0)
    {
        vec3 diffuse = lightColor * diffuseImpact * material.diffuse;

        vec3 viewDirection = normalize(cameraPosition - v_position);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        vec3 specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess) * lightColor * material.specular;

        result += diffuse * light.diffuse + specular * light.specular;
    }

    vec4 texelColor = texture(texture1, v_textCoord);
    color = vec4(texelColor.rgb * result, texelColor.a);
}