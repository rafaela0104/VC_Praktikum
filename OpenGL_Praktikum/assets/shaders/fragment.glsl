#version 330 core

in vec3 normal;
in vec3 colorVS;
in vec3 FragPos;
in vec3 lightDir;
in vec3 viewDir;

uniform float time;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 matSpecular;
uniform float matShinyness;

out vec4 color;

void main() {

    // Diffuse
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    float cosa = max(0.0, dot(N,L));
    vec3 diffuseTerm = colorVS * lightColor;
    color = vec4( diffuseTerm * cosa, 1.0);

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambientColor = colorVS;
    vec3 ambientTerm = ambientColor * ambientStrength * lightColor;
    color += vec4(ambientTerm, 1.0);




    // Specular
    vec3 V = normalize(viewDir);
    vec3 R = normalize(reflect(-L,N));
    float cosBeta = max(0.0, dot(R,V));
    float cosBetak = pow(cosBeta, matShinyness);
    vec3 specularTerm = matSpecular * cosBetak * lightColor;

    color += vec4(specularTerm * cosBetak, 1.0);

    //vec3 result = ambientTerm + diffuseTerm + specularTerm;
    //color = vec4(result, 1.0);

}