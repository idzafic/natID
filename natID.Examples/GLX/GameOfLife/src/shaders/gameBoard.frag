#version 450

// SSBO buffer with game board data structure
layout(set = 0, binding = 0, std430) readonly buffer GameBoardData {
    uint width;
    uint height;
    uint data[];
} gameBoard;

layout(location = 0) out vec4 outColor;

void mainFS()
{
    uint gridWidth = 25;
    uint borderSize = gridWidth/25; //(no border when gridWidth is below 25)


    uvec2 index = ivec2(gl_FragCoord.xy / gridWidth);


    uvec2 pxInCell = uvec2(gl_FragCoord.xy) - index*gridWidth;
    if (pxInCell.x < borderSize || pxInCell.y < borderSize) //border on left side
    {
        outColor = vec4(0.8, 0.8, 0.8, 1.0);  // border
        return;
    }
    

    if((index.x >= gameBoard.width) || (index.y >= gameBoard.height)) //out of bounds
    {
        outColor = vec4(1, 0, 0, 0.50);
        return;
    }

    float v = float(gameBoard.data[index.y * gameBoard.width + index.x] != 0u);
    outColor = vec4(v, v, v, 1.0);

    
}
