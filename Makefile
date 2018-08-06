# remember - Makefiles MUST use tabs!!! :set paste
UNAME = $(shell uname -s)

WWWDIR = www
BINDIR = bin
BUILDDIR = build

# immediate set mode, when this line is evaluated we make the dirs
_MKDIRS := $(shell mkdir -p $(BINDIR) $(WWWDIR) $(BUILDDIR))

MODULES = main
ASSETS = models/cube_m.json assets/multi.bmp shaders/simple.es300.vert shaders/simple.es300.frag

# looks like installing SDL gets you this cool little utility for finding lib install locations
LOCALLIBS = $(shell sdl2-config --libs)
ifeq ($(UNAME),Darwin)
	LOCALLIBS += -Wl,-dead_strip -framework OpenGL
else
	LOCALLIBS += -lGL
endif

COMMONFLAGS = -std=c++11 -MMD -MP -I"libs/rapidjson/include" -I"/usr/local/Cellar/glm/0.9.9.0/include"
LOCALFLAGS = -g -O2 $(COMMONFLAGS)

EMXX = emcc
EMXXFLAGS = $(COMMONFLAGS) -Oz -s -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_SDL=2 --js-library library_foo.js
EMXXLINK = -s TOTAL_MEMORY=50331648

$(WWWDIR): $(WWWDIR)/index.html $(WWWDIR)/main.js

$(WWWDIR)/index.html: index.html
	cp index.html $(dir $@)index.html

$(WWWDIR)/main.js: $(MODULES:%=$(BUILDDIR)/%.em.o) $(ASSETS) Makefile
	$(EMXX) $(EMXXFLAGS) $(EMXXLINK) $(filter %.o,$^) $(ASSETS:%=--preload-file %) -o $@

$(BINDIR)/main: $(MODULES:%=$(BUILDDIR)/%.o) Makefile
	$(CXX) $(LOCALFLAGS) $(filter %.o,$^) $(LOCALLIBS) -o $@

$(BUILDDIR)/%.em.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	@echo $(EMXX) -c $< -o $@
	@$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	@echo $(CXX) -c $< -o $@
	@$(CXX) $(LOCALFLAGS) $(if $(filter-out $(NOWARNDIRS),$(dir $<)),$(LOCALWARN)) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/* $(WWWDIR)/*
