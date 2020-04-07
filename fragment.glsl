#version 450 core

struct material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D depth;
    sampler2D occlusion;
    float shininess;
};

in VS_OUT {
    vec2 uv_coord;
    vec3 frag_pos;
    vec3 tangent_light;
    vec3 tangent_view;
    vec3 tangent_frag;
} fs_in;

out vec4 fragColor;

uniform material mtl;
uniform vec3 light_color;

vec2 reliefMapping(vec2 texCoord, vec3 viewDir)
{
    vec2 ds = viewDir.xy / viewDir.z * -0.05;
    const int linear_search_steps = 10;
    const int binary_search_steps = 32;

    float size = 1.0 / linear_search_steps;
    float depth = 0.0;

    float best_depth = 1.0;

    for (int i = 0; i < linear_search_steps; i++)
    {
        depth += size;
        float off_depth = texture(mtl.depth, texCoord + ds*depth)[0];
        if (depth >= off_depth)
        {
            best_depth = depth;
            break;
        }
    }
    depth = best_depth;
    for (int i = 0; i < binary_search_steps; i++)
    {
        size *= 0.5;
        float off_depth = texture(mtl.depth, texCoord + ds*depth)[0];
        if (depth >= off_depth)
        {
            best_depth = depth;
            depth -= 2 * size;
        }
        depth += size;
    }

    return texCoord + ds * best_depth;
}

void main()
{
    // Mapping
    vec3 viewDir = normalize(fs_in.tangent_view - fs_in.tangent_frag);
    vec2 n_texCoord = reliefMapping(fs_in.uv_coord, viewDir);

    // Out of texture coordinates
    if (n_texCoord.x > 1.0 || n_texCoord.x < 0.0 || n_texCoord.y > 1.0 || n_texCoord.y < 0.0);
        // discard;

    // Calculating normal
    vec3 normal = texture(mtl.normal, n_texCoord).xyz;
    normal = normalize(normal * 2.0 - 1.0);

    // Ambient
    vec3 ambient = 0.1 * texture(mtl.diffuse, n_texCoord).rgb;

    // Diffuse
    vec3 lightDir = normalize(fs_in.tangent_light - fs_in.tangent_frag);
    float diff_coef = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = texture(mtl.diffuse, n_texCoord).rgb * diff_coef;

    // Specular
    // vec3 reflectDir = normalize(reflect(-lightDir, normal));
    // float spec_coef = pow(max(dot(viewDir, reflectDir), 0.0), mtl.shininess);
    // vec3 spec = texture(mtl.specular, n_texCoord).rgb * spec_coef * lightIntensity;

    // Ambient occlusion
    float occlusion = texture(mtl.occlusion, n_texCoord).r;

    fragColor = vec4((ambient + diffuse) * occlusion, 1.0);
}
