
# setup

brew install emscripten # tested with version 1.38.11
brew install glm # tested with version 0.9.9.0
brew install sdl2 # tested with version 2.0.8

# build

to build the desktop version: `make`
to build the web version `make www` (and then serve dir www with `python -m SimpleHTTPServer 8000`)
