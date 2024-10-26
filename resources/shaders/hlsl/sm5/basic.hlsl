struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 normal : NORMAL;
};

cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix MVP;
};

PS_INPUT VS_Main(VS_INPUT input)
{
    PS_INPUT output;
    float4 pos = float4(input.Pos, 1.0);
    output.Pos = mul(MVP, pos);
    output.normal = input.Normal;
    return output;
}

float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    // Define a red color
    float4 redColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    // Simple diffuse shading based on normal direction
    float3 normal = normalize(input.normal);
    float3 lightDir = normalize(float3(1.0f, 2.0f, -1.0f)); // Example light direction
    float diffuse = max(dot(normal, lightDir), 0.0f);
    
    // Apply the diffuse factor to the red color
    return redColor * diffuse + 0.1;
}