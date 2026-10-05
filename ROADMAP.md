# ft_irc : Répartition du travail

**A = Réseau · B = Protocole · C = Channels**

## A : Réseau
- Makefile, arborescence, `.gitignore`
- `Server` : socket, bind, listen, non-bloquant, boucle `poll()`
- Accept / déconnexion des clients
- Buffers de réception et d'envoi (`POLLOUT`)
- Signaux, fuites mémoire, robustesse
- `KICK`

## B : Protocole et authentification
- Parser de messages IRC + dispatcher de commandes
- `PASS`, `NICK`, `USER`, message de bienvenue (001)
- `PRIVMSG` (user), `NOTICE`, `PING`, `PONG`, `QUIT`
- `INVITE`, `TOPIC`
- Choix du client de référence, script de tests

## C : Channels et modes
- Classe `Channel`, `Replies.hpp`
- `JOIN`, `PART`, `NAMES`, `PRIVMSG` (channel)
- `MODE` : `i`, `t`, `k`, `o`, `l` + vérifications dans `JOIN`

## Ensemble
- `DECISIONS.md` et squelettes de classes

## Ordre

| Phase | A | B | C |
|---|---|---|---|
| 0. Apprendre | serveur echo `poll()` | parser d'une ligne IRC | découpage de buffer |
| 1. Fondations | Makefile, `Server`, accept, buffer de réception | dispatcher, `PASS` `NICK` `USER`, 001 | `Channel`, `Replies.hpp` |
| 2. Base | envoi `POLLOUT`, déconnexion, signaux | `PRIVMSG` user, `NOTICE`, `PING/PONG`, `QUIT` | `JOIN`, `PART`, `NAMES`, `PRIVMSG` channel |
| 3. Opérateurs | `KICK` | `INVITE`, `TOPIC` | `MODE` + vérifs `JOIN` |


Bonus (bot, transfert de fichiers) seulement si le mandatory est parfait.

---

## Règles Git
- **Personne ne travaille jamais sur `main`.**
- Chacun sa branche par ticket : `feature/<nom>` (ex. `feature/poll-loop`, `feature/join`).
- Pour intégrer son travail : **Pull Request vers `main`**, relue par au moins un autre membre.
- Avant la PR : `git pull origin main` sur sa branche, vérifier que ça compile avec `-Wall -Wextra -Werror -std=c++98`.

## Règles du sujet
- Un **seul** `poll()` (ou équivalent) pour tout : accept, lecture, écriture.
- **Jamais** de `recv`/`send` en dehors du `poll()` (sinon note = 0).
- I/O **non-bloquantes**, pas de `fork`.
- Le programme **ne doit jamais crasher** (sinon note = 0).
- C++98, flags `-Wall -Wextra -Werror`, pas de lib externe ni Boost.
- Makefile : `$(NAME)`, `all`, `clean`, `fclean`, `re`, pas de relink inutile.
- Tout le monde doit comprendre tout le code : en soutenance, n'importe qui peut être interrogé sur n'importe quelle partie, et une modification à chaud peut être demandée.

## Ressources

**Réseau**
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) (sockets, `poll()`)
- [man poll](https://man7.org/linux/man-pages/man2/poll.2.html) · [man recv](https://man7.org/linux/man-pages/man2/recv.2.html) · [man send](https://man7.org/linux/man-pages/man2/send.2.html) · [man fcntl](https://man7.org/linux/man-pages/man2/fcntl.2.html)

**Protocole IRC**
- [Modern IRC Client Protocol](https://modern.ircdocs.horse/) (le plus lisible, à lire en premier)
- [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459) · [RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812) (compléments)

**Test**
- `nc -C 127.0.0.1 <port>` puis Ctrl+D pour envoyer une commande en plusieurs morceaux (sujet, section IV.3)
