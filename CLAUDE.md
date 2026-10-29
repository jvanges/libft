# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Contexte

Ceci est la copie de travail sur un VPS de la **libft de John (jvan-ges), étudiant à 42 Belgium**. Le projet officiel est déjà pushé sur le serveur de l'école (vogsphere) — cette copie sert à **préparer la défense** : on peut commenter, tester, bidouiller librement, ne rien re-pusher n'est requis.

Version du sujet : **v19.3** (nouveau format) — il n'y a **pas de partie bonus**, les fonctions de listes chaînées (`ft_lst*`) sont **obligatoires**, et le README.md est exigé avec un format précis (première ligne en italique, sections Description / Instructions / Resources + usage de l'IA déclaré).

## Commandes

```sh
make          # compile libft.a à la racine (cc -Wall -Wextra -Werror)
make clean    # supprime les .o
make fclean   # supprime les .o et libft.a
make re       # rebuild complet
```

Pas de règle `bonus` (inutile avec le sujet v19.3).

## Tests et vérification

- **Norminette** : installée dans `~/.venvs/norminette` sur le VPS → `~/.venvs/norminette/bin/norminette`. Doit retourner 0 erreur (le code est normé, même si la Norme n'est pas le focus ici).
- **libftTester (Tripouille)** : cloné dans `/tmp/libftTester` (temporaire, à re-cloner si absent) avec deux patches : `LIBFT_PATH = /home/john/42/libft` et `clang++` → `g++`. Lancer `make m` depuis ce dossier : compile chaque test avec valgrind intégré (`MOK`/`OK` = tout passe ; le test `ft_lstdelone` n'affiche rien par design, il ne vérifie que les fuites).
  - Test individuel : `make <fonction>` sans le préfixe `ft_` (ex. `make calloc`, `make lstmap`).
- **Test manuel rapide** : compiler un petit main contre l'archive, depuis la racine du repo : `cc -Wall -Wextra -Werror main.c -L. -lft -o demo && ./demo`. Mettre le `main.c` de test dans `/tmp` pour ne pas polluer le repo, et ajuster les chemins (`-I/home/john/42/libft -L/home/john/42/libft`).
- **Sujet** : `libft.pdf` à la racine = sujet v19.3, référence canonique en cas de doute sur un prototype ou comportement.

## Architecture

- Une fonction par fichier `ft_*.c`, prototypes et `t_list` dans **libft.h** ; `Makefile` archive tout dans `libft.a` avec `ar rcs`.
- **libft.h** inclut `stddef.h`, `stdlib.h`, `unistd.h` (seules fonctions externes autorisées : `malloc`, `free`, `write`).
- Fonctions helpers internes déclarées `static` (ex. `count_words`, `free_all`, `next_word` dans ft_split.c ; `count_len` dans ft_itoa.c).
- ⚠️ À ne pas "corriger" sans demander : `ft_lstsize` retourne `unsigned int` (le sujet v19.3 dit `int`) — écart connu et validé, tous les tests passent.

## Notes pour la défense

- Le code est commenté en anglais au-dessus de chaque fonction (bloc `/* ** ... */` hors des corps de fonctions) — ces commentaires servent de fiches de révision.
- **SOUTENANCE.md** à la racine : fiche de révision complète (toutes les fonctions, questions classiques, pièges). Si on modifie une fonction, mettre à jour ce fichier pour qu'il reste cohérent avec le code — et le README.md, dont le format précis est exigé par le sujet.
- Points d'explication à connaître : gestion du chevauchement dans `ft_memmove`, anti-overflow dans `ft_calloc` (`total / nmemb != size`), `long` dans `ft_itoa`/`ft_putnbr_fd` pour INT_MIN, cleanup complet sur échec d'alloc dans `ft_split`/`ft_lstmap`, `\0` cherchable dans `ft_strchr`/`ft_strrchr`.

## Artefacts de build

- Les `*.o` et `libft.a` sont non trackés dans cette copie : ne pas les `git add` (il n'y a pas de `.gitignore`). Lancer `make fclean` avant un éventuel commit ou archive.
- Le Makefile re-link automatiquement si `libft.h` change (règle `%.o: %.c libft.h`) : un simple `make` suffit après modif du header, pas besoin de `make re`.
