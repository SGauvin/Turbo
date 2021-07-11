#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

in vec3 v_position;
in vec3 v_normal;
in vec4 v_color;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    float ambientStrength = 0.1;
    float specularStrength = 0.3;
    float shininess = 128;

    vec3 ambientColor = ambientStrength * lightColor;
    
    vec3 result = ambientColor;

    vec3 normal = normalize(v_normal);
    vec3 lightDirection = normalize(lightPosition - v_position);

    float diffuseImpact = dot(lightDirection, normal);


    if (diffuseImpact > 0.0)
    {
        vec3 diffuseColor = diffuseImpact * lightColor;

        vec3 viewDirection = normalize(cameraPosition - v_position);
        vec3 reflectionDirection = reflect(-lightDirection, normal);
        vec3 specularColor = specularStrength * pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess) * lightColor;

        result += diffuseColor + specularColor;
    }

    color = vec4(result * vec3(v_color), 1.0);
}