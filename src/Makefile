# stolen from
#  - https://github.com/srjheam/ufes-tbo2/blob/b0fa0c0ab919e591a2f171ffc4ac8b27e83b9a04/src/Makefile

# Makefile que percorre toda a biblioteca, procurando os códigos fontes .cpp para compilar

LIBTINYXML2DIR        = ../include/tinyxml2
LIBTINYXML2           = tinyxml2
LIBTINYXML2DIRRELEASE = $(LIBTINYXML2DIR)/build/release

SRCS = $(shell find . -type f -name '*.cpp' | sed 's|^./||')

INCLUDES = -lGL -lGLU -lglut -lm -I $(LIBTINYXML2DIR)

MAIN              = main
OUT               = trab1

BUILDIR           = build
RELEASEDIR        = $(BUILDIR)/release

OBJSRELEASE = $(patsubst %.cpp, $(BUILDIR)/release/%.o, $(notdir $(SRCS)))

# G++ - é o compilador usado
# CFLAGS - são as flags dadas ao compilador durante a compilação
CPP = g++
override CPPFLAGS += -Wall -Wextra $(INCLUDES)

RELEASEFLAGS = -O3 -DNDEBUG

MAKE = make

# vpath diretiva que adiciona todos os diretórios com códigos fontes para busca com o padrão %.cpp
#   - https://www.gnu.org/software/make/manual/html_node/Selective-Search.html
#   - não me pergunte mais nada, eu não sei como funciona
vpath %.cpp $(shell dirname $(SRCS) | uniq)

all : release

release : pre-build-release build-release

# Compila todos os .o
$(BUILDIR)/release/%.o : %.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(RELEASEFLAGS)

# Preparativos para o build; Garante que o diretório build/ existe
pre-build:
	mkdir -p $(BUILDIR)/

pre-build-release: pre-build
	mkdir -p $(RELEASEDIR)/
	$(MAKE) -C $(LIBTINYXML2DIR)/

build-release : $(OBJSRELEASE)
	$(CPP) -o $(RELEASEDIR)/$(OUT) $^ -L $(LIBTINYXML2DIRRELEASE) -l$(LIBTINYXML2) $(CPPFLAGS) $(RELEASEFLAGS)

clean:
	rm -rf $(BUILDIR)/
	$(MAKE) -C $(LIBTINYXML2DIR)/ clean
