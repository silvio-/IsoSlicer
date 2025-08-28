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
Select "Camera" Tab. Figure2.jpg shows the selected "Camera" tab. You can pick "Phong Shading with Texture". Then you load the texture from the "textures" directory (a bmp file) inside the "dataset" directory. Choose "Trimmed-CInfo.bmp"

*2.Render the Utah Teapot:*  
Figure1.jpg shows the options in the "NURBS" tab. In the "Slicing" area select "zbuffer"  and "Trim"  and press the button "Multiple NURBS" in the "Rendering NURBS" area. In the "multiple_patches" directory (inside "dataset") select the file "UtahTeapot.txt".

*3.Present the slicing samples over the rendered image:* 
In the slicing area, press the button "Multiple NURBS-Multilayer" and select nn the "multiple_patches" directorythe same file "UtahTeapot.txt".

That's it! You can change several parameters to change the views, objects and textures. The trimming area is the pure black region in the texture file. You can show a single NURBS by pressing the button "Load a NURBS" (files in the "single-patch" directory inside dataset), and render it with the button "Single NURBS". Commas (',') are used to indicate decimal digits (ex: 0,234 is 0.234). 







