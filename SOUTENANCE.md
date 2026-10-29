# Soutenance libft — jvan-ges (42 Belgium)

Fiche de révision complète : pour chaque fonction, ce qu'elle fait, comment elle est implémentée, et **les questions à anticiper**. Les commentaires en anglais dans le code reprennent ces mêmes explications.

---

## 1. Vue d'ensemble

- **43 fichiers `ft_*.c`**, une fonction par fichier, tous archivés dans `libft.a`.
- **`libft.h`** : protégé par include guard (`#ifndef LIBFT_H`), déclare `t_list` et tous les prototypes. Inclut `stddef.h` (pour `size_t`, `NULL`), `stdlib.h` (malloc/free), `unistd.h` (write) — les **seules fonctions externes autorisées** par le sujet.
- Fonctions internes déclarées **`static`** donc invisibles hors de leur fichier : `count_words` / `free_all` / `next_word` (ft_split), `count_len` (ft_itoa).

### Makefile

```make
NAME    = libft.a
AR      = ar rcs        # r: remplace, c: crée, s: index de symboles
all / clean / fclean / re
%.o: %.c libft.h        # chaque .o dépend aussi de libft.h (recompile si le header change)
$(NAME): $(OBJS)        # ne relink que si un .o est plus récent
```

Questions possibles :
- *« Pourquoi une lib .a et pas .so ? »* → sujet l'impose ; l'archive statique est copiée dans le binaire au link.
- *« Que fait `ar rcs` ? »* → crée/met à jour l'archive et son index de symboles.
- *« Pourquoi `.PHONY` ? »* → `clean`, `fclean`, etc. ne sont pas des fichiers ; sinon make pourrait les confondre avec des fichiers existants.

### La structure `t_list`

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
```

- **`void *content`** : générique, la liste ne sait pas ce qu'elle stocke.
- Le **caller** est responsable de la durée de vie du contenu (libéré via `del` dans lstdelone/lstclear).
- **« Pourquoi `t_list **` (double pointeur) dans add_front/add_back/clear ? »** → parce que la fonction doit pouvoir modifier la tête de liste du caller (liste vide → le premier nœud devient la tête ; clear → remettre la tête à `NULL`).

---

## 2. Fonctions de classification — `ft_is*`, `ft_to*`

Toutes prennent un `int` (pas un `char`) comme la libc : ça permet d'y passer `EOF` ou n'importe quel int sans comportement indéfini.

| Fonction | Test effectué | Valeur de retour |
|---|---|---|
| `ft_isalpha(c)` | `'A' ≤ c ≤ 'Z'` ou `'a' ≤ c ≤ 'z'` | 1 / 0 |
| `ft_isdigit(c)` | `'0' ≤ c ≤ '9'` | 1 / 0 |
| `ft_isalnum(c)` | `ft_isalpha(c) \|\| ft_isdigit(c)` | 1 / 0 |
| `ft_isascii(c)` | `0 ≤ c ≤ 127` | 1 / 0 |
| `ft_isprint(c)` | `32 ≤ c ≤ 126` (espace inclus) | 1 / 0 |
| `ft_toupper(c)` | si minuscule → `c - 32` | car transformé, sinon `c` |
| `ft_tolower(c)` | si majuscule → `c + 32` | car transformé, sinon `c` |

- *« Pourquoi ±32 ? »* → en ASCII, majuscules et minuscules sont exactement à 32 de distance.

---

## 3. Fonctions mémoire — `ft_mem*`, `ft_bzero`, `ft_calloc`

### `ft_memset(void *s, int c, size_t n)`
Écrit le byte `c` (converti en `unsigned char`) sur les `n` premiers bytes de `s`, retourne `s`. Travaille à travers un `unsigned char *` (manipulation par byte).

### `ft_bzero(void *s, size_t n)`
Juste `ft_memset(s, 0, n)` — rien à retourner.

### `ft_memcpy(void *dst, const void *src, size_t n)`
Copie `n` bytes byte par byte. **Interdiction de chevauchement** (sinon → `memmove`).
Le test `if (!dst && !src) return (NULL);` sécurise l'appel `ft_memcpy(NULL, NULL, 3)` que certains testeurs vérifient.
- *« Pourquoi `unsigned char *` ? »* → le sujet l'exige comme la libc (manipulation byte par byte, la conversion en unsigned char garantit la comparaison correcte des bytes > 127).

### `ft_memmove(void *dst, const void *src, size_t len)` — ⭐ question classique
Identique à memcpy **mais gère le chevauchement** :

```
dst = [........]
src =    [........]     ← chevauchement
```

- Si **`dst < src`** : copier dans l'ordre croissant (index 0 → len) **ne peut jamais écraser un byte pas encore lu** → on délègue à `ft_memcpy`.
- Si **`dst > src`** : une copie forward écraserait les données de src avant de les avoir lues → on copie **depuis la fin** (i = len-1 → 0).
- Si `dst == src` : les deux branches marchent (copie inutile mais correcte).

### `ft_memchr(const void *s, int c, size_t n)`
Scanne les `n` premiers bytes à la recherche du byte `c` (comparé en `unsigned char`). Retourne un pointeur sur le byte trouvé ou `NULL`. Contrairement aux fonctions de **string**, on ne s'arrête pas à `'\0'` : de la mémoire brute, pas une chaîne.

### `ft_memcmp(const void *s1, const void *s2, size_t n)`
Compare byte par byte en **`unsigned char`** (sinon un byte comme `0xFF` serait négatif en `char` signé et trierait mal). Retourne 0 si égal, sinon la différence du premier couple différent.

### `ft_calloc(size_t nmemb, size_t size)` — ⭐ question classique
Alloue `nmemb * size` bytes **initialisés à zéro** (via `ft_bzero`).

```c
total = nmemb * size;
if (total / nmemb != size)   // ← anti-overflow
	return (NULL);
```

- *« À quoi sert cette division ? »* → si `nmemb * size` déborde (`size_t` qui boucle), `malloc` recevrait une petite taille alors qu'on croit avoir un grand buffer → corruption mémoire. Si le produit déborde, la division ne redonne pas `size` → on détecte et on refuse.

---

## 4. Fonctions chaînes (partie 1) — les wrappers libc

### `ft_strlen(const char *s)`
Compte jusqu'à `'\0'` non inclus.

### `ft_strlcpy(char *dst, const char *src, size_t size)` — ⭐
Copie **au plus `size - 1`** caractères, **termine toujours par `'\0'`** si `size > 0` (d'où la condition `i < size - 1` qui réserve la place du nul).
**Retourne `strlen(src)`, pas le nombre copié** : en comparant `ret >= size` l'appelant sait s'il y a eu troncature. Si `size == 0`, dst n'est pas touché.

### `ft_strlcat(char *dst, const char *src, size_t size)` — ⭐
Concatène `src` à `dst` en écrivant au plus `size - 1 - dst_len` caractères (`i + 1 < size` réserve le `'\0'`).
**Retourne la longueur de la chaîne qu'elle aurait voulu créer** : `dst_len + src_len`, ou `size + src_len` quand `size <= dst_len` (buffer trop petit pour contenir dst en entier). C'est ce qui permet à l'appelant de détecter la troncature (`ret >= size`).

### `ft_strchr(const char *s, int c)` / `ft_strrchr(...)` — ⭐
- `strchr` : **première** occurrence de `c` (converti en `char`), `strrchr` : la **dernière**.
- **Le `'\0'` est cherchable** : `ft_strchr(s, 0)` retourne un pointeur sur la fin de chaîne. C'est pour ça qu'après la boucle (qui s'arrête sur `'\0'`), on teste encore `s[i] == (char)c` — ou, dans strrchr, que la boucle avance d'abord jusqu'au `'\0'` inclus.
- Conversion `(char)c` : le standard impose de chercher `c` converti en char (un int comme 1024 se comporte comme `(char)1024`).

### `ft_strncmp(const char *s1, const char *s2, size_t n)`
Compare au plus `n` caractères **en `unsigned char`** (bytes > 127 triés correctement). Retourne 0 si égalité, sinon `a[i] - b[i]`.

### `ft_strnstr(const char *big, const char *little, size_t len)` — ⭐
Cherche la première occurrence de `little` dans `big`, sans jamais **lire** au-delà de `len` bytes.
- `little` vide → retourne `big`.
- La condition **`i + j < len`** dans la boucle interne garantit que la correspondance candidate ne lit jamais hors de la fenêtre `[0, len)`.
- Retourne `NULL` si le match déborderait de `len`.

### `ft_atoi(const char *nptr)` — ⭐
1. Saute les whitespaces `' '` + `'\t'` à `'\r'` (`(c >= 9 && c <= 13) || c == ' '`).
2. Un signe optionnel `+`/`-`.
3. Lit les chiffres : `acc = acc * 10 + (c - '0')`.
4. Multiplie par le signe.

**Pas de gestion d'overflow** : comme le vrai `atoi`, le comportement après dépassement d'un int est indéfini. (C'est pour ça que le sujet déconseille d'utiliser atoi pour vérifier un int.)

### `ft_strdup(const char *s)`
`malloc(len + 1)` puis `ft_memcpy` de `len + 1` bytes (le `'\0'` est copié « gratuitement »). Retour fourni au caller → **le caller doit free**.

---

## 5. Partie 2 — les fonctions avec malloc

Point commun : **retourner NULL si malloc échoue**, et toute string allouée est sous la responsabilité du caller.

### `ft_substr(char const *s, unsigned int start, size_t len)`
Sous-chaîne d'au plus `len` caractères à partir de `start`.
- `start >= strlen(s)` → chaîne vide (pas NULL !).
- **Clamp** : si `len > slen - start`, on réduit à `slen - start` → on ne malloc que le nécessaire.

### `ft_strjoin(char const *s1, char const *s2)`
`malloc(l1 + l2 + 1)`, deux `ft_memcpy` (le second copie `l2 + 1` bytes pour embarquer le `'\0'`).

### `ft_strtrim(char const *s1, char const *set)`
1. Avance `start` tant que `s1[start] ∈ set` (test via `ft_strchr(set, s1[start])`).
2. Recule `end` tant que `s1[end - 1] ∈ set` — la condition **`end > start`** empêche le cas « la chaîne ne contient que des chars de set » de déborder (résultat = chaîne vide).
3. `ft_substr(s1, start, end - start)` fait le reste.

### `ft_split(char const *s, char c)` — ⭐⭐ la plus demandée
Trois helpers `static` :

| Helper | Rôle |
|---|---|
| `count_words(s, c)` | Compte les mots (les suites de `c` ne créent pas de mot vide : `"  a  b "` → 2). |
| `next_word(s, c, &pos)` | Saute les délimiteurs, extrait le mot suivant via `ft_substr`, avance `pos`. |
| `free_all(tab, i)` | Libère les `i` strings déjà allouées puis le tableau, quand une alloc échoue à mi-chemin. |

Le malloc fait `nb_words + 1` pointeurs : **+1 pour le `NULL` final**.

*« Pourquoi free_all ? »* → si le i-ème malloc échoue, retourner NULL directement fuirait les i-1 précédents. On apprend à **nettoyer complètement à chaque échec**.

### `ft_itoa(int n)` — ⭐ question classique
- `n` est copié dans un **`long nb`** : `-n` déborderait sur `INT_MIN` (-2147483648) en int, pas en long.
- `count_len` compte les chiffres (démarre à 1 → gère le cas `0`).
- `len = count_len(nb) + (n < 0)` : booléen utilisé comme valeur (1 case de plus pour `-`).
- **Remplissage depuis la fin** (`res[--len] = last digit`) → pas besoin d'inverser ; le `-` tombe à l'index 0.

### `ft_strmapi` / `ft_striteri` — ⭐ différence
- `strmapi` : **alloue** une nouvelle chaîne où chaque car est `f(index, c)` — s intact.
- `striteri` : applique `f(index, &s[i])` **en place** — f peut modifier la chaîne car on lui passe l'adresse du caractère.

### `ft_put*_fd(int fd)`
- `ft_putchar_fd` : un `write(fd, &c, 1)`.
- `ft_putstr_fd` : **un seul** `write(fd, s, ft_strlen(s))` (`ft_putendl` = putstr + putchar '\n').
- `ft_putnbr_fd(int n)` — ⭐ : `n` passé en **`long`** (INT_MIN encore), signe écrit d'abord, puis **récursion** : `ft_putnbr(nb / 10)` avant d'écrire `nb % 10 + '0'` → l'appel récursif imprime les chiffres hauts **avant** le dernier chiffre, donc l'ordre sort naturellement de gauche à droite.

---

## 6. Listes chaînées (obligatoires, pas de « bonus » en v19.3)

Rappel : **`t_list **` = adresse de la tête → permet de la modifier (ajout en liste vide, clear).

| Fonction | Comportement | Piège à citer |
|---|---|---|
| `ft_lstnew(content)` | malloc un nœud, `next = NULL` | ne duplique PAS content, stocke le pointeur |
| `ft_lstadd_front(**lst, *new)` | `new->next = *lst; *lst = new;` | ordre des deux lignes : lire l'ancienne tête **avant** d'écraser `*lst` |
| `ft_lstsize(list)` | compte en itérant | retourne `unsigned int` (écart connu avec le sujet v19.3 qui dit `int`, validé — tous les testeurs passent) |
| `ft_lstlast(list)` | avance jusqu'au `next == NULL` | liste vide → NULL |
| `ft_lstadd_back(**lst, *new)` | si liste vide → `*lst = new`, sinon `ft_lstlast(*)->next = new` | le cas liste vide, d'où le `t_list **` |
| `ft_lstdelone(node, del)` | `del(node->content); free(node);` | **ne touche pas au next**, comme l'exige le sujet |
| `ft_lstclear(**lst, del)` | boucle : sauvegarde `next` → delone → avance ; termine par `*lst = NULL` | sauvegarder `next` AVANT free, sinon on lit de la mémoire libérée |
| `ft_lstiter(list, f)` | applique f à chaque `content` | pas d'alloc |
| `ft_lstmap(list, f, del)` — ⭐ | construit une **nouvelle** liste avec `f(content)` ; jamais la liste d'origine | **cleanup** : si `ft_lstnew` échoue, on `del(content)` (le contenu fraîchement créé par f) **et** `ft_lstclear(&head, del)` la partie déjà construite avant de retourner NULL |

```c
// ft_lstmap — le passage critique
content = f(lst->content);
node = ft_lstnew(content);
if (!node)
{
	del(content);            // content n'a jamais rejoint la liste → fuite sinon
	ft_lstclear(&head, del); // détruit le début de liste déjà construit
	return (NULL);
}
ft_lstadd_back(&head, node);
```

---

## 7. Questions de soutenance à maîtriser à fond

1. **`memmove` overlap** : pouvoir dessiner les deux cas et dire pourquoi le sens de copie change selon `dst < src`.
2. **Overflow `calloc`** : `total / nmemb != size` — malloc ne détecte pas le dépassement de la multiplication.
3. **`INT_MIN`** : pourquoi `long` dans itoa et putnbr (`-n` déborde car |INT_MIN| = INT_MAX + 1).
4. **`split` cleanup** : montrer `free_all`, expliquer pourquoi 42 est intransigeant sur les fuites.
5. **`lstmap` cleanup** : double libération (`del(content)` + `ft_lstclear`).
6. **`'\0'` cherchable** dans strchr/strrchr : `strchr("abc", 0)` renvoie un pointeur vers la fin — comportement imposé par la libc.
7. **Valeurs de retour de strlcpy/strlcat** : longueur « tentée », pas écrite → détection de troncature.
8. **`unsigned char`** dans memcmp/strncmp/memchr : comparaison correcte des bytes > 127.
9. **Pointeurs de fonction** : `void (*f)(void *)` — syntaxe, pourquoi `del` est paramétrable (la liste ne connaît pas le type du content).
10. **`t_list **` vs `t_list *`** : quand la fonction doit modifier la tête du caller.
11. **`static` helpers** : visibilité limitée au fichier, pas de symbole exporté dans libft.a pour les helpers.
12. **Re-link du Makefile** : dépendance `%.o: %.c libft.h`, `ar` ne recrée que si un .o est plus récent.

## 8. Vérifications (à montrer en début de défense si demandé)

```sh
make                                          # -Wall -Wextra -Werror, 0 warning
~/.venvs/norminette/bin/norminette            # 0 erreur
cd /tmp/libftTester && make m                 # Tripouille, valgrind intégré (OK/MOK partout)
```
