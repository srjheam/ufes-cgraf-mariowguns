<div align="center">
  <h3 align="center">Laboratório de Computação Gráfica</h3>
</div>

## Sobre

Esses laboratórios e exercícios são da disciplina de Computação Gráfica, ministrada pelo professor Thiago.

Você pode encontrar os trabalhos avaliativos dessa disciplina no meu perfil do GitHub.

## Começando

### Pré-requisitos

- Linux (sem suporte pro windows, sra. microsoft)
- [GNU Compiler Collection](https://gcc.gnu.org/)

### Instalando

1. Clone o repositório

   ```sh
   git clone https://github.com/srjheam/ufes-cgraf-mariowguns.git
   ```

2. Dirija-se ao seu diretório

   ```sh
   cd ufes-cgraf-mariowguns/
   ```

### Compilando

1. Compile o programa

   ```sh
   make
   ```

2. Execute o programa

   ```sh
   ./trabalhocg <arena_path>
   ```

Onde,

- `arena_path` - é o caminho para o arquivo .svg da arena.

## Contribuindo

Este projeto está configurado e integrado com o Visual Studio Code.

### Valgrind

Há uma regra no [Makefile](./Makefile) para lançar rapidamente o Valgrind. O resultado será salvo em [valgrind-out.txt](./valgrind-out.txt).

```sh
make valgrind VALARGS=<args>
```

Onde,

- `args` - são os argumentos para passar ao programa. Opcional.

## Suporte

Esse programa não funciona em ambientes não-POSIX e pode até mesmo não funcionar em sistemas não-Linux.

Isso se deve ao fato de usar APIs que são extensões do POSIX e não fazem parte do padrão C.

## Licença

Licenciado sob [Unlicense](./LICENSE).
