[vertex]
#version 460 core

// Vertex attributes
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 4) in vec4 a_Color; // optional per-vertex color

// Uniforms
layout (std140, binding = 0) uniform Matrices
{
    mat4 lp_Projection;
    mat4 lp_View;
};

uniform mat4 lp_Transform; // particle model matrix

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec4 v_VertexColor;

void main()
{
    gl_Position = lp_Projection * lp_View * lp_Transform * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_Normal = mat3(lp_Transform) * vec3(0.0, 0.0, 1.0); // default normal for quad
    v_VertexColor = a_Color;
}

[fragment]
#version 460 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec4 v_VertexColor;
out vec4 FragColor;

// Particle uniforms
uniform sampler2D u_Albedo;
uniform vec4 u_Color;          // per-particle color
uniform int u_SpriteIndex;     // current sprite index
uniform int u_Rows;            // rows in sprite sheet
uniform int u_Columns;         // columns in sprite sheet

void main()
{
    // --- Sprite sheet UV calculation ---
    vec2 frameUV = v_TexCoord;

    int safeRows = max(u_Rows, 1);
    int safeCols = max(u_Columns, 1);
    int safeIndex = u_SpriteIndex;

    int row = safeIndex / safeCols;
    int col = safeIndex % safeCols;

    frameUV.x = (frameUV.x + float(col)) / float(safeCols);
    frameUV.y = (frameUV.y + float(row)) / float(safeRows);

    // Sample the particle texture
    vec4 texColor = texture(u_Albedo, frameUV);

    // Discard fully transparent pixels
    if(texColor.a < 0.05)
        discard;

    // --- Simple directional lighting ---
    vec3 lightDir = normalize(vec3(0.3, 0.7, 0.5));
    vec3 normal = normalize(v_Normal);
    float diff = max(dot(normal, lightDir), 0.4);
    diff = mix(1.0, diff, 0.8);

    vec3 litColor = texColor.rgb * diff;

    // Combine with particle color and per-vertex color
    FragColor = vec4(litColor, texColor.a) * u_Color * v_VertexColor;
}

