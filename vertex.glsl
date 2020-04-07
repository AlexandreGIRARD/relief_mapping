#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec2 uv_coord;
    vec3 frag_pos;
    vec3 tangent_light;
    vec3 tangent_view;
    vec3 tangent_frag;
} vs_out;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.frag_pos = vec3(model * vec4(aPos, 1.0));
    vs_out.uv_coord = aTexCoord;

    mat3 normalMat = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.tangent_frag = TBN * vs_out.frag_pos;
    vs_out.tangent_view = TBN * viewPos;
    vs_out.tangent_light = TBN * lightPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);

}
