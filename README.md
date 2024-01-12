# KIV/FJP compiler - making C-like clone


## TODOs:

- dodelat statickou typovou kontrolu



--- 


	dal nejak nabrat 5-10b ze seznamu dole + doplnit test casy

### 1b
- další typ cyklu
- ternární operátor
- paralelní přiřazení
- datový typ string (s operátory pro spojování řětezců)

### 2b
- příkaz GOTO
- datový typ ratio (s celočíselnými instrukcemi)
- složený datový typ (pro nas `struct`)
- pole a práce s jeho prvky
- operátor pro porovnání řetězců
- objekty bez polymorfismu
- anonymní vnitřní funkce (lambda výrazy)

### 3b
- dynamicky přiřazovaná paměť - práce s ukazateli
- parametry předávané odkazem
- objektové konstrukce s polymorfním chováním
- instanceof operátor
- anonymní vnitřní funkce (lambda výrazy) které lze předat jako parametr
- mechanismus zpracování výjimek
	
### ?b
- Další rozšíření je možno doplnit po konzultaci, s ohodnocením podle odhadnuté náročnosti.



## Tools installation guide for macOS:

Tools and requirements:
- flex 2.6.4 Apple(flex-34)
- bison (GNU Bison) 3.8.2
- g++ with C++11 features

### flex
`brew install flex`

### bison
`brew install bison@3.8 `

### gcc/g++ with C++11 features
`brew install gcc`
