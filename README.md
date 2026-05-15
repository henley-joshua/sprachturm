# sprachturm

Sprachturm is planned to be a horror game based on the tower of babel. Nothing is done yet, only the directory structure thus far.

For dependencies (Raylib), I believe the compiled lib should be added to the lib/ for reproducibility and easy team use.

## Raylib

Raylib is added as a submodule in the vendor/raylib folder. This forces a single version of raylib after compilation. In order to update the dependency, run `git submodule update --init`

The Makefile is set to compile raylib along with our build (no linking). This means raylib headers can be included with quotes just like our other header files.
For our header files, I am specifying the -I flag for the src and include directories, which will allow the compiler to search for the header path in that directory if not found relative to the file that includes that header file.
