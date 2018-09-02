# remember - Makefiles MUST use tabs!!! :set paste
UNAME = $(shell uname -s)

WWWDIR = www
BINDIR = bin
BUILDDIR = build

# immediate set mode, when this line is evaluated we make the dirs
_MKDIRS := $(shell mkdir -p $(BINDIR) $(WWWDIR) $(BUILDDIR))

MODULES = main sqlite
ASSETS = assets/models/hideout_m.json \
				 assets/images/hideout.bmp \
				 assets/shaders/simple.es300.vert \
				 assets/shaders/simple.es300.frag \
				 assets/data/gang.db

HEADERS = \
    src/sim.h \

# looks like installing SDL gets you this cool little utility for finding lib install locations
LOCALLIBS = $(shell sdl2-config --libs)
ifeq ($(UNAME),Darwin)
	LOCALLIBS += -Wl,-dead_strip -framework OpenGL
else
	LOCALLIBS += -lGL
endif

COMMONFLAGS = -std=c++11 -MMD -MP -I"libs/sqlite3" -I"libs/rapidjson/include" -I"/usr/local/Cellar/glm/0.9.9.0/include"
LOCALFLAGS = -g -O2 $(COMMONFLAGS)

cc = gcc

EMXX = emcc
EMXXFLAGS = $(COMMONFLAGS) -Oz -s USE_WEBGL2=1 -s FULL_ES3=1 -s USE_SDL=2 \
						--js-library library_foo.js -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'
EMXXLINK = -s TOTAL_MEMORY=50331648

# default action (ie, you just type `make`)
all: $(BINDIR)/main

$(WWWDIR): $(WWWDIR)/index.html $(WWWDIR)/main.js

$(WWWDIR)/index.html: index.html
	cp index.html $(dir $@)index.html

$(WWWDIR)/main.js: $(MODULES:%=$(BUILDDIR)/%.em.o) $(ASSETS) Makefile
	$(EMXX) $(EMXXFLAGS) $(EMXXLINK) $(filter %.o,$^) $(ASSETS:%=--preload-file %) -o $@

$(BINDIR)/main: $(MODULES:%=$(BUILDDIR)/%.o) Makefile
	$(CXX) $(LOCALFLAGS) $(filter %.o,$^) $(LOCALLIBS) -o $@

$(BUILDDIR)/%.em.o: src/%.cpp $(HEADERS) Makefile
	@mkdir -p $(dir $@)
	@echo $(EMXX) $(EMXXFLAGS) -c $< -o $@
	@$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(BUILDDIR)/sqlite.em.o: src/sqlite3.c Makefile
	@mkdir -p $(dir $@)
	@echo $(EMXX) -c $< -o $@
	@$(EMXX) -c $< -o $@

$(BUILDDIR)/%.o: src/%.cpp Makefile
	@mkdir -p $(dir $@)
	@echo $(CXX) -c $< -o $@
	@$(CXX) $(LOCALFLAGS) $(if $(filter-out $(NOWARNDIRS),$(dir $<)),$(LOCALWARN)) -c $< -o $@

$(BUILDDIR)/sqlite.o: src/sqlite3.c Makefile
	@mkdir -p $(dir $@)
	@echo $(cc) -c $< -o $@
	@$(cc) -lpthread -ldl $(if $(filter-out $(NOWARNDIRS),$(dir $<)),$(LOCALWARN)) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/* $(WWWDIR)/*
