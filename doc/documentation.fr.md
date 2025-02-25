
# SOARE documentation

---

## Sommaire

- [L'interpréteur](#linterpréteur)

  - [Installer SOARE](#installer-soare)
  - [Compiler l'interpréteur](#compiler-linterpréteur)
  - [Les commandes interpréteur](#les-commandes-interpréteur)
  - [Charger un fichier](#charger-un-fichier)

- [SOARE](#soare)

  - [Introduction](#introduction)
  - [Textes et nombres](#textes-et-nombres)
  - [Stocker des données](#stocker-des-données)
  - [Définition des fonctions](#définition-des-fonctions)
  - Les boucles et les structures conditionnelles
  - Les tableaux
  - Les entrées utilisateur

---

## L'interpréteur

### Installer SOARE

Pour installer SOARE, vous pouvez téléchager le code source ou la release via [le repo officiel](https://github.com/AntoineLandrieux/SOARE/).

**avec git** :

```sh
git clone https://github.com/AntoineLandrieux/SOARE.git
```

Sur Windows, après avoir [compilé l'interpréteur](#compiler-linterpréteur), vous pouvez lancer le fichier `install.bat`

Pour lancer SOARE:

```sh
# Linux
bin/soare
# Windows
%SOARE%\soare.exe
```

### Compiler l'interpréteur

**Dépendances** :

- `ar`
- `g++`
- `gcc`
- `make`
- `windres` (pour Windows seulement)

```sh
make
```

### Les commandes interpréteur

- `?run`

Permet d'executer le code écrit dans la console

```txt
>>> write "Hello!"
>>> ?run
Hello!
>>> ?run
Hello!
```

- `?commit`

Permet d'executer un seule fois le code écrit dans la console

```txt
>>> write "Hello!";
>>> ?commit
Hello!
>>> ?commit
>>>
```

- `?clear`

Permet d'effacer le contenu de la console

```txt
>>>
```

- `?exit`

Ferme l'interpréteur

```txt
>>>?exit

Bye!

```

### Charger un fichier

Pour charger un fichier vous pouvez utiliser la commande

```sh
soare "filename.soare"
```

---

## SOARE

### Introduction

De nombreux exemples de ce manuel, même ceux saisis à l’invite interactive, incluent des commentaires. Les commentaires dans SOARE commencent par le caractère `?`, et s’étendent jusqu’à la fin de la ligne physique. Un commentaire peut apparaître au début d’une ligne ou après des espaces ou du code, mais pas à l’intérieur d’un littéral de chaîne. Étant donné que les commentaires visent à clarifier le code et ne sont pas interprétés par SOARE, ils peuvent être omis lors de la saisie d’exemples.

**Quelques exemples** :

```txt
? Ceci est le premier commentaire
$number = 1; ? Et c'est le deuxième commentaire
             ? ... et maintenant le troisième !
$text = "? Ce n'est pas un commentaire parce qu'il est à l'intérieur de guillemets.";
```

> [!NOTE]
>
> Vous pouvez mettre « ; » à la fin d’une instruction pour dire qu’elle est terminée
>

### Textes et nombres

SOARE peut manipuler du texte (représenté par une chaîne de caractères) ainsi que des nombres. Cela inclut les caractères "!", les mots "lapin", les noms "Paris", les phrases "Je te soutiens.", etc. "Youpi !". Ils peuvent être placés entre guillemets simples ('...') ou entre guillemets doubles ("...") avec le même résultat.

Pour afficher le texte, nous utiliserons le mot-clé « **write** » :

```txt
write "Hello World!";
write 123.456;
```

### Stocker des données

Une variable vous permet de stocker un nombre ou un texte. Une variable est définie par un nom et une valeur.

```txt
? $ pour créer un nouvelle variable
$name = "Antoine"
$age = 15

? ici la variable age existe déjà, donc pas besoin du signe $
age = age + 1

? le signe + avec des chaînes de caractères permet de les concatener
write "Hello " + name + " " + age + "yo";
```

### Définition des fonctions

Nous pouvons créer une fonction qui écrit la série de Fibonacci:

```txt
@fib(n)
  ? Print a Fibonacci series up to n.
  $a = 0;
  $b = 1;
  $next = b;
  
  while a < n do
    write a;
    a = b;
    b = next;
    next = a + b;
  end;
  
  return a;
end;

write "The last value is " + fib(100);
```

Le mot-clé `@` introduit une définition de fonction. Il doit être suivi du nom de la fonction et de la liste des paramètres formels entre parenthèses et séparé par un « ; ».

Le mot-clé `return` quitte la fonction en renvoyant une valeur à l'endroit où la fonction a été appelé

### Les boucles et les structures conditionnelles
