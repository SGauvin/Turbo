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
    float specularStrength = 0.5;
    float shininess = 32;

    vec3 ambientColor = ambientStrength * lightColor;
    
    vec3 normal = normalize(v_normal);
    vec3 lightDirection = normalize(lightPosition - v_position);

    float diffuseImpact = max(dot(lightDirection, normal), 0.0);
    vec3 diffuseColor = diffuseImpact * lightColor;

    vec3 viewDirection = normalize(cameraPosition - v_position);
    vec3 reflectionDirection = reflect(-lightDirection, normal);

    vec3 specularColor = specularStrength * pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess) * lightColor;

    vec3 result = (ambientColor + diffuseColor + specularColor) * vec3(v_color);
    color = vec4(result, 1.0);
}