---------------Jammin Session Client---------------

Authors :
	Chitimbo Manyanda Charles
	Larbi Youcef Mohamed Reda
	
0) Outils nécessaires

Pour pouvoir utiliser ce client, il faut avoir installé les paquets de developpement pour ALSA. Par exemple sous Debian/Ubuntu :
	sudo apt-get install libasound2-dev

1) Contenu du rendu

- dossier bin/ contenant les binaires du client
- dossier obj/ contenant les fichiers objets compilés
- dossier src/ contenant les fichiers sources
- dossier include/ contenant les fichier en-tête
- dossier lib/ contenant les bibliothèques extérieure (vide)
- fichier makefile permettant de compiler le client
- ce fichier readme :)

2) Comment utiliser le client ?

Pour utiliser ce client, il faut tout d'abord le compiler, si ce n'est pas fait.
Pour cela, il faut lancer la commande "make".
Une fois compilé, il faut le lancer de cette manière :
	bin/jamminClient ?[-user <name>] ?[-port <port>] ?[-address <address>]
	
où name est le nom avec lequel l'utilisateur se connecte, port le port de connexion, et address l'adresse du serveur.
Si ces paramètres ne sont pas donnés, name vaudra "pc2r", port "2015" et address "localhost".

Si l'on souhaite effacer les binaires, ainsi que les fichiers objets, il suffit de lancer la commande "make clean"
Pour arrêter le client, on peut taper "\EXIT" sur le chat, ou encore en utilisant le fameux Ctrl+C.

3) Enjoy
