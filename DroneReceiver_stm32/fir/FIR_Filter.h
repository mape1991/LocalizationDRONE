

//=======================================================================================

// 	Auteur T.Rocacher
// Test� avec un filtre � 10 coeff (FIR1 MATLAB)
// Semble OK (module non mesur� avec pr�cision
// 6,2 us environ � 40 MHz de CPU
// une estimation sur la dur�e donne
// Duree = 1,2us + 500ns*N,  N �tant le nbre de coeff
// du filtre.
//
//=======================================================================================


#ifndef _FIR_FILTER_H__
#define _FIR_FILTER_H__

/*

Configuration, fichiers, projet :

- Inclure dans le fichier C qui utilise  le service l'API : FIR_Filter.h
- Inclure dans le projet les fichiers  FIR_0.c � FIR_7.c  
ainsi que le fichier assembleur FIR_Filter.s.



Logiciel, codage :

- Ouvrir le fichier FIR_x.c que vous souhaitez et remplir les valeurs 
des coefficients du filtre. Vous pourrez vous aider du script MATLAB : 
GenereFIR_vers_C.m
- Dans la fonction appelante, lancer Init_FIR_asm(x); pour initialiser la
table d'entr�e du filtre x � 0
- Utiliser la fonction  s32 FIR_asm(char NumFiltre,s16 entree) pour r�cup�rer 
le nouvel �chantillon en sortie de filtre. L'�chantillon d'entr�e est cod� en 
1.15 [-1,0 � +1,0[ et la sortie est en 8.24 soit un intervalle de [-128,0 � + 128,0[.
NumFiltre est le num�ro du filtre, soit x.
- Pour le remplissage des filtres et pour le programme appelant, 
des constantes seront utiles : K_8_24 et K_1_15. Par exemple, si le nouvel 
�chantillon vaut -0,5 en 1.15, il faudra lancer la commande  
FIR_asm(x,-K_1_15*-0.5) ; cela permet de bien faire appara�tre la valeur d�cimale.


*/


#define K_8_24 16777216 // constante 2^24
#define K_1_15 32768 // constante 2^15


//void Init_FIR_asm(char NumFiltre);
//s32 FIR_asm(char NumFiltre,s16 entree);

/** Multiplication des deux tableaux (Les coef en 8.24)*/



#endif
