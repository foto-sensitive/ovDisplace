# ovDisplace
This sketch offers an application of Displacement Mapping for viewing 360° media. 

Displacement Mapping normally is a 3D rendering technique that displaces the position of vertices within a mesh. It is used in games and 3D renders to create more intricate surfaces. 

This example displaces the sphere onto which your 360 media is projected. It does so from the inside. This provides you with different perspectives of your 360° media. By extruding the vertices it adds depth to the image. 

The vertices distance from the centre of the sphere is determined by the value stored in the texture map.

## Usage
Use the GUI to toggle between 3 different modes of displacment: Texture, Sinusoidal and Pattern.
Drag & Drop your own media onto the app window to have it displaced.
Adjust polygonal, texture and viewing paramaters with the sliders.

## Dependencies:
### Library itself
None (C++11)

### Examples
openFrameworks 0.9.0

should work on any platform, but project files currently exist only for Windows / VS2015
