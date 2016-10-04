#include "syscall.h"

int main()
{
	PutString("Que se passe-t-il si vous enlevez l’appel à
Halt()
à la fin de la fonction
main
de
putchar.c
? Décryptez le message d’erreur et expliquez. Que changer po
ur éviter cette erreur?
Vous n’aurez ainsi plus besoin d’appeler explicitement
Halt()
dans vos programmes. Comment
faire pour prendre en compte la valeur de retour
return n
de la fonction
main
si celle-ci est décla-
rée à valeur entière ? (cherchez dans
test
qui appelle
main
)\n\0");
	Halt();
}

