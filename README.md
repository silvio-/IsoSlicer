# IsoSlicer
System to directly slice Trimmed-NURBS

IsoSlicer is a custom-made software that takes a trimmed-Nurbs surface and produces sample points that represent the slicing curves in multiple layers as part of the 3D printing pipeline. The visualization in the software is based on the triangle scanline algorithm with Phong Illumination and shading, with z-buffer algorithm for visibility. The software was coded in C++ using the Embarcadero C++Builder IDE in the Windows 11 system.
Its URL is:
https://www.embarcadero.com/products/cbuilder

 The C++Builder 12 Community Edition is free for academic purposes. It can be found at:
https://www.embarcadero.com/products/cbuilder/starter

You just need to fill out a small form and press the Download button. Then you follow the instructions that will appear.

To run the IsoSlicer, you just need to download all listed files into any local directory, and click twice in the file 
IsoSlicer-1.0.cbproj
or start the C++Builder IDE and go to File->Open Project (Ctrl F11), and select this file in the chosen directory. You then press Run button, or Ctrl+Shift+F9.
To see the source codes, you go to View->Units (Ctrl+F12).

When runnning, you need to make a few selections to get what Figure1.jpg shows:

*1.Load a texture image:*
Select "Camera" Tab. Figure2.jpg shows the selected "Camera" tab. You can pick "Phong Shading with Texture". Then you load the texture from the "textures" directory (a bmp file). 





