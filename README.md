
# setup

```
brew install emscripten             # tested with version 1.38.11
brew install glm                    # tested with version 0.9.9.0
brew install sdl2                   # tested with version 2.0.8
cd libs && https://github.com/Tencent/rapidjson
```

# build and run

## Desktop Version

to build: `make`
to run: `./bin/main`

## Web Version

to build: `make www`
ro run: `cd www && python -m SimpleHTTPServer 8000` and then open a web browser and load http://localhost:8000

# windows build - in progress - requires mingw

`mingw32-make.exe -f .\WMakefile`

x86_64-w64-mingw32-g++.exe -o main.exe .\src\main.cpp -I"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include" -I".\libs\glm-0.9.9.0\glm" -I".\libs\rapidjson\include" -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32
