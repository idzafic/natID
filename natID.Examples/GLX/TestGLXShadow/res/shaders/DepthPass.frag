#version 450

void mainDepthFS(){
	// For depth-only rendering, we don't need to write any color output
	// The depth value is automatically written to the depth buffer
	// by the GPU's built-in depth testing
}