cls

del *.obj

del *.exe

cl.exe /c /EHsc /I "C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.8\include" DevProp.c

link.exe DevProp.obj OpenCL.lib /LIBPATH:"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.8\lib\x64" /SUBSYSTEM:CONSOLE
