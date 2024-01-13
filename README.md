# Struktura projektu

Projekt je realizován jako C++ knihovna.

```
.
├── src/
│   ├── ast/
│   │   ├── nodes/
│   │   │   ├── assignment_node.h
│   │   │   ├── block_node.h
│   │   │   └── ...
│   │   └── ast_nodes.h
│   ├── rules/
│   │   ├── grammar.y
│   │   └── lex.l
│   └── utils
├── data/
│   ├── complex.fjp
│   ├── complex.pl0
│   ├── echo.fjp
│   └── ...
├── .gitignore
├── build.sh
├── compiler.exe
├── doc.pdf
└── README.md
```

`nodes` - jednotlivé uzly AST stromu

`ast_nodes.h` - hromadný include

`grammar.y` - vstupní soubor pro nástroj bison

`lex.l` - vstupní soubor pro nástroj flex

`utils` - definice struktur, funkcí pro logování, údržbu tabulky symbolů, definice pro PL/0, ...

`data` - *use-cases* vytovřeného jazyka (uvnitř jsou 3 přeložené programy do rozšířené PL/0 - 
soboury jsou zakončené koncovkou `pl0`)

`build.sh` -  skript pro překlad

`compiler.exe` - přeložený program spustitelný v prostředí Windows

`doc.pdf` - elektronická forma této dokumentace

