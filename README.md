# Doctrina SDK
 
Doctrina SDK is a small hobby SDK with a 3D C++ game engine and an editor that uses the game engine.
There are no plans to make fully-fledged games with this sdk

- [Trello Board](https://trello.com/b/0X4gj5H7/doctrina-sdk)
- [Features](FEATURES.md)


## How to build 

### Windows

You can build and run from the terminal:

    cmake -S. -BBUILD
    cmake --build BUILD --config Release
    & "BUILD\Editor\Release\Editor.exe"

Just make sure that you are running this in the same folder as the root [CMakeLists.txt](CMakeLists.txt)

