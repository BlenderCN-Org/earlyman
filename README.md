
# setup

brew install emscripten # tested with version 1.38.11
brew install glm # tested with version 0.9.9.0
brew install sdl2 # tested with version 2.0.8

# build

to build the desktop version: `make`
to build the web version `make www` (and then serve dir www with `python -m SimpleHTTPServer 8000`)

NOTE: cmake files are legacy, not using these anymore

# windows build - in progress

x86_64-w64-mingw32-g++.exe -o main.exe .\src\main.cpp -I"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include" -
I"C:\Users\jharding\Downloads\glm-0.9.9.0\glm" -I".\libs\rapidjson\include" -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32
