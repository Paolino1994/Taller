
# Trabajamos con c++
extension = cpp

# Definiciones
math = si
pthreads = si
sdl2 = si

# Definir si se quiere ver la llamada al compiler
verbose = si

####

# Flags y configuraciones
CXXFLAGS = -Wall -Werror -pedantic -pedantic-errors

# Para optimizar
OPTIMIZEFLAGS = -O3 -DNDEBUG

# Para valgrind o debug
DEBUGFLAGS = -Og -ggdb -DDEBUG -fno-inline

# Estandar de C++ a usar
CXXSTD = c++11

#
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif

####

# Linkeo con libm
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

# Agrego flags y libs de sdl2
ifdef sdl2
CXXFLAGS += $(shell sdl2-config --cflags)
SDL_LD_FLAGS = $(shell pkg-config --libs sdl2)
SDL_LD_FLAGS += $(shell pkg-config --libs SDL2_image)
SDL_LD_FLAGS += $(shell pkg-config --libs SDL2_mixer)
SDL_LD_FLAGS += $(shell pkg-config --libs SDL2_ttf)
LDFLAGS += $(sort $(SDL_LD_FLAGS)) # para eliminar duplicados
endif

# Linkeamos con yaml-cpp
LDFLAGS += -lyaml-cpp

#### 

# Manejo de archivos y directorios

DEPDIR = dep
OBJDIR = obj
SRCDIR = src
# si queremos meter los headers en otro directorio
INCLDIR = $(SRCDIR)

# Si no especifica archivos, tomo todos.
getsrc = $(shell find $(SRCDIR)/$(1) -name "*.$(extension)")
fuentes_client ?= $(call getsrc,client)
fuentes_server ?= $(call getsrc,server)
fuentes_common ?= $(call getsrc,common)

gen_obj_files = $(patsubst $(SRCDIR)_%.$(extension),$(OBJDIR)/%.o,$(subst /,_,$(1)))
o_common_files = $(call gen_obj_files,$(fuentes_common))
o_client_files = $(call gen_obj_files,$(fuentes_client))
o_server_files = $(call gen_obj_files,$(fuentes_server))
o_files = $(o_common_files) $(o_client_files) $(o_server_files)
d_files = $(o_files:$(OBJDIR)%.o=$(DEPDIR)%.d) #dependencias

# REGLAS
#########

.PHONY: all clean release

# Por defecto hacemos debug
all: client server

# Para hacer un release optimizado
release: DEBUGFLAGS := $(OPTIMIZEFLAGS)
release: clean
release: all

client: client.out

server: server.out

client.out: $(o_common_files) $(o_client_files)
	@if [ -z "$(o_client_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente (archivos client*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $^ -o client.out $(LDFLAGS)

server.out: $(o_common_files) $(o_server_files)
	@if [ -z "$(o_server_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor (archivos server*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $^ -o server.out $(LDFLAGS)

$(DEPDIR):
	mkdir $(DEPDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	@$(RM) -fv $(o_files) $(d_files) client.out server.out

-include $(d_files)

.SECONDEXPANSION:

$(o_files) : $(OBJDIR)/%.o : $(SRCDIR)/$$(subst _,/,%).$(extension) | $(OBJDIR) $(DEPDIR)
	@echo "  DEP $(@:$(OBJDIR)%.o=$(DEPDIR)%.d)"; $(ocxx) $(CXXFLAGS) $(DEBUGFLAGS) -MM -MT $@ -MF $(@:$(OBJDIR)%.o=$(DEPDIR)%.d) -I$(INCLDIR) $<
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -I$(INCLDIR) -c $< -o $@
