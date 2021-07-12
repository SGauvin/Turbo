#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform sampler2D texture1;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_textCoord;

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

    vec4 texelColor = texture(texture1, v_textCoord);
    color = vec4(texelColor.rgb * result, texelColor.a);
}