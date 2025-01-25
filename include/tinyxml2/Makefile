# Makefile que percorre toda a biblioteca, procurando os códigos fontes .cpp para compilar

# INCLUDE   - é o diretório que contém os headers
# BUILDIR   - é o diretório onde estarão os binários
# LIBNAME   - nome base da biblioteca do projeto
# LIBTARGET - é o caminho para a biblioteca compilada
# SRCS      - lista com os caminhos para todos .c
INCLUDE = .
BUILDIR = build
RELEASEDIR = $(BUILDIR)/release
DEBUGDIR = $(BUILDIR)/debug
LIBNAME = tinyxml2
LIBTARGET = lib$(LIBNAME).a
SRCS = $(shell find . -type f -name '*.cpp' | sed 's|^./||')

OBJSRELEASE = $(patsubst %.cpp, $(RELEASEDIR)/%.o, $(notdir $(SRCS)))
OBJSDEBUG = $(patsubst %.cpp, $(DEBUGDIR)/%.o, $(notdir $(SRCS)))

# G++ - é o compilador usado
# CPPFLAGS - são as flags dadas ao compilador durante a compilação
CPP = g++
override CPPFLAGS += -Wall -Wextra -I $(INCLUDE) -lm

DEBUGFLAGS = -ggdb3 -O0 -DDEBUG
RELEASEFLAGS = -O3 -DNDEBUG

# ARchive - é o comando arquivador da biblioteca
# ARFLAGS - são as flags dadas ao arquivador
AR = ar
ARFLAGS = rcs

# vpath diretiva que adiciona todos os diretórios com códigos fontes para busca com o padrão %.c
#   - https://www.gnu.org/software/make/manual/html_node/Selective-Search.html
#   - não me pergunte mais nada, eu não sei como funciona
vpath %.cpp $(shell dirname $(SRCS) | uniq)

all : release

release : pre-build-release build-release

debug: pre-build-debug build-debug

# Compila todos os .o
$(RELEASEDIR)/%.o : %.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(RELEASEFLAGS)

$(DEBUGDIR)/%.o : %.cpp
	$(CPP) -o $@ -c $< $(CPPFLAGS) $(DEBUGFLAGS)

# Preparativos para o build; Garante que o diretório build/ existe
pre-build:
	mkdir -p $(BUILDIR)/

pre-build-release: pre-build
	mkdir -p $(RELEASEDIR)/

pre-build-debug: pre-build
	mkdir -p $(DEBUGDIR)/

build-release : $(OBJSRELEASE)
	$(AR) $(ARFLAGS) $(RELEASEDIR)/$(LIBTARGET) $^

build-debug : $(OBJSDEBUG)
	$(AR) $(ARFLAGS) $(DEBUGDIR)/$(LIBTARGET) $^

# Rode make clean para remover tudo que é gerado pela build
clean:
	rm -rf $(BUILDIR)/
