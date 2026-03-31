To start the program, open the GL_TerrainGen folder in CMake and build the project.

Upon launching the application, a terrain will be generated using default Perlin noise parameters.
The user can modify the terrain generation by clicking the "Frekvencija" button, which adjusts the
noise frequency and thereby alters the terrain topology. The lower input range is value 1 for
frequency, the higher input range is value 24.

-Camera Movement
	The user can navigate the terrain using keyboard input relative to the camera's orientation:
		1.) W — Move forward
		2.) S — Move backward
		3.) A — Move left (strafe)
		4.) D — Move right (strafe)
-Mouse Control
	Pressing the Spacebar toggles the mouse capture mode.
	When enabled, mouse movement adjusts the camera's orientation.
	When disabled, the mouse is released for UI interaction or to exit the program.

The generated terrain includes a variety of surface types, such as water bodies, sandy regions,
grassy fields, rocky slopes, and snow-covered peaks, creating a visually rich and diverse landscape.
