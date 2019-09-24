# planets

win32 api screen saver based on the NeHe OpenGL tutorials lesson 6 and the screen saver tutorials by Brian Tegart and Lucian Wischik.

http://nehe.gamedev.net/tutorial/texture_mapping/12038/
http://www.wischik.com/scr/index.html

Here I have managed to get rid of the Glaux library.

You can use your own textures. Mine are RAW RGB 128x128 graphics files.
You can save RAW RGB data in Gimp or Photoshop.

Use Borland 5.5 Compiler and command-line tools to build the executable.
Screen savers are plain .exe files that are renamed as .scr files.

To build the planets.RES resource file:
brc32 -r planets.rc

To build the executable:
bcc32 -tW planets.c

You can test it with:
planets /s

Or rename the .exe file into .scr and then right click it and left click Test.

