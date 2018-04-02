
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

#### 

# Manejo de archivos y directorios

DEPDIR = dep
OBJDIR = obj
SRCDIR = src
# si queremos meter los headers en otro directorio
INCLDIR = $(SRCDIR)

# Si no especifica archivos, tomo todos.
getsrc = $(shell find $(SRCDIR) -name "*.$(extension)")
fuentes ?= $(call getsrc)

gen_obj_files = $(patsubst $(SRCDIR)_%.$(extension),$(OBJDIR)/%.o,$(subst /,_,$(1)))
o_files = $(call gen_obj_files,$(fuentes))
d_files = $(o_files:$(OBJDIR)%.o=$(DEPDIR)%.d) #dependencias


# REGLAS
#########

.PHONY: all clean release

# Por defecto hacemos debug
all: client

# Para hacer un release optimizado
release: DEBUGFLAGS := $(OPTIMIZEFLAGS)
release: clean
release: all

client: $(o_files)
	@if [ -z "$(o_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente (archivos client*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $^ -o client.out $(LDFLAGS)

$(DEPDIR):
	mkdir $(DEPDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	@$(RM) -fv $(o_files) $(d_files) client.out

-include $(d_files)

.SECONDEXPANSION:

$(o_files) : $(OBJDIR)/%.o : $(SRCDIR)/$$(subst _,/,%).$(extension) | $(OBJDIR) $(DEPDIR)
	@echo "  DEP $(@:$(OBJDIR)%.o=$(DEPDIR)%.d)"; $(ocxx) $(CXXFLAGS) $(DEBUGFLAGS) -MM -MT $@ -MF $(@:$(OBJDIR)%.o=$(DEPDIR)%.d) -I$(INCLDIR) $<
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -I$(INCLDIR) -c $< -o $@
