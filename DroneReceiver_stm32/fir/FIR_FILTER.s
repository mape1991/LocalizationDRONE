 

/*__________________________________________________________

Fonction proposée :
FIR_asm()

__________________________________________________________*/





	.syntax unified		/* Required in order to be able to use Thumb-2 instructions
						   Otherwise, produce only Thumb-1 code (16 bits instructions) */
	.cpu cortex-m3

 	.align	2 


	.section ".text"
	.thumb
	.thumb_func
	.align	2

	/* Exportation des fonctions visibles, accessibles depuis le .h associé. */ 
	.global FIR_asm 
	.type	FIR_asm , %function
	.global Init_FIR_asm 
	.type	Init_FIR_asm  , %function

	 /* Importation des variables déclarée dans FIR_i.c, i de 0 à 7 */
	 .EXTERN  h_0  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_0  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_0 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_0	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_1  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_1  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_1 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_1	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_2  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_2  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_2 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_2	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_3  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_3  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_3 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_3	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_4  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_4  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_4 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_4	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_5  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_5  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_5 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_5	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_6  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_6  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_6 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_6	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/

	 .EXTERN  h_7  			/* @ ROM du filtre i, éléments du tableau : const s32 8.24 signé */
	 .EXTERN  N_7  			/* @ ROM Nombre de coef du filtre  : un const u16 */
	 .EXTERN  TabE_7 	 	/* @ RAM de la table des entrées : type s16 */
	 .EXTERN  Ptr_Tab_E_7	/* @ RAM du pointeur sur la table d entree : 32 bit pointeur*/


FIR_asm:
/*_____________________________________________________

	FIR : s32 FIR_asm(NumFiltre,s16 entree);
	R0 : numero de filtre
	R1 : Entrée e(n), au format 1.15
	R0 : Sortie au format 8.24
	 

_______________________________________________________*/


/* sauvegarde registres */
	push {R1-R11}	
	
/* tri des 8 filtres*/
	cmp R0,#0
	beq Init_Reg_FIR_0
	cmp R0,#1
	beq Init_Reg_FIR_1
	cmp R0,#2
	beq Init_Reg_FIR_2
	cmp R0,#3
	beq Init_Reg_FIR_3
	cmp R0,#4
	beq Init_Reg_FIR_4
	cmp R0,#5
	beq Init_Reg_FIR_5
	cmp R0,#6
	beq Init_Reg_FIR_6
	cmp R0,#7
	beq Init_Reg_FIR_7
	b Fin_Fonction_FIR	
	

Init_Reg_FIR_0:	
		
	ldr R11,=Ptr_Tab_E_0
	ldr r2,[R11] /*R2 est maintenant le pointeur de tableE*/
	ldr R3,=h_0
	ldr R4,=TabE_0
	ldr R5,=N_0
	ldrsh R5,[R5]
	b FIR

Init_Reg_FIR_1:	
Init_Reg_FIR_2:	
Init_Reg_FIR_3:	
Init_Reg_FIR_4:	
Init_Reg_FIR_5:	
Init_Reg_FIR_6:	
Init_Reg_FIR_7:	


FIR:
	
/*_____________________________________________________

	FIR : partie de la fonction FIR_asm qui caclule effectivement la sortie
	Entree
	R0 : numero de filtre
	R1 : Entrée e(n), au format 1.15
	R2 : Ptr_Tab_E_i
	R11 : @ du pointeur sur TabE_i, 
	R3 : @ table du filtre en ROM (32 bits)
	R4 : @ table d'entrée, TabE_0
	R5 : Longueur de la table qui est aussi celle du filtre, N_i
	R0 : Sortie au format 8.24
	 

_______________________________________________________*/

/*rangement entree courante*/
	strh r1,[r2]

/* Caclul derniere ad pour buff circ, la première est R4, la dernière R10 */ 
	add R10,R4,R5,lsl 1 /*R10=après la dernière case = R4+2*N_i  */


/* Filtrage (rappel smulal : R7:R6 = R8*R9 + R7:R6
	s=0 (R7 et R6 à 0)*/
	mov R7,#0
	mov r6,#0
	/*Pour i de N-1 à 0 (i = R5) */

Pour_i_de_N_1_a_0:
	    /*R8=*Ptr_E post inc 2, 17.15*/
		ldrsh r8,[R2],#2 
		/*R9=*ptr_h post inc 4, 8.24*/
		ldr r9,[R3],#4
		/*	 R7:R6 = R7:R6 + R8*R9   res en 25.39*/
		smlal r6,r7,r8,r9
		/*si R2=R10 (pointe juste après le dernier élément	 */
Si_R10_Pointe_apres_dernier_Elt:
		cmp r10,r2
		bne Fin_Si
		mov R2,R4 /*(R2 pointe en début de table)	*/
Fin_Si:

Fin_Pour:
		subs R5,#1
		bne Pour_i_de_N_1_a_0
	
/*recalage en 8.24 en décalant droite pour tps d'exec	
25.39 >> 15 = 42.24*/
	lsr r6,#15 /* 15 bits laisses libres à 0, soit R6(31:9)*/
	/* nnnn nnnn nnnn nnnn | nnnn nnnn nnnn nnnn donne
	   0000 0000 0000 000n | nnnn nnnn nnnn nnnn */
	bfi r6,r7,#17,#15 /* R6(31:17) = r7(14:0)*/
	/* bit field insert : prend les 23 bits faibles de r7 et les
	range dans R6 à partir du bit 9
	R6 est donc le résultat en 8.24*/

/*Test de saturation, saturation*/
	ssat r0,#31,r6 /*r6 est saturé si nécessaire à +2^31-1 ou -2^31*/
	/* r0 est la sortie en 8.24*/	   
	
/* Recalage pointeur R10 pointe à nouveau sur e(n). Il faut le
décrementer pour le positionner sur e(n+1), futur e(n)*/
Si_2: 
	cmp R2,R4
	bne Sinon_2     	
		sub R2,R10,#2 /*(pointage dernière case table, R10 au départ = finTabE +2)*/
	b Fin_Si_2
Sinon_2:	  
	  	sub R2,R2,#2
Fin_Si_2:
	  /*memo pointeur*/
	  str r2,[r11]	  	




Fin_Fonction_FIR:
/* restaure registres*/
	pop {R1-R11} 
	bx lr






















Init_FIR_asm:
/*_____________________________________________________

	void Init_FIR_asm(char NumFiltre);
						 R0			

   - Initialise le tableau de variables d'entrée à 0.

   - Positionne le pointeur de la table d'entrée concernée 
	(première case de la table d'entrée TabE_i) :

_______________________________________________________*/

/* sauvegarde registres */
  	push {R1-R5}


/* tri des 8 filtres*/
	cmp R0,#0
	beq Init_Table_0
	cmp R0,#1
	beq Init_Table_1
	cmp R0,#2
	beq Init_Table_2
	cmp R0,#3
	beq Init_Table_3
	cmp R0,#4
	beq Init_Table_4
	cmp R0,#5
	beq Init_Table_5
	cmp R0,#6
	beq Init_Table_6
	cmp R0,#7
	beq Init_Table_7
	b Fin_Fonction

/* chargement des registres pour chaque filtre*/
Init_Table_0:
	ldr R1,=TabE_0
	ldr R2,=Ptr_Tab_E_0
	ldr R3,=N_0
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_1:
	ldr R1,=TabE_1
	ldr R2,=Ptr_Tab_E_1
	ldr R3,=N_1
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_2:
	ldr R1,=TabE_2
	ldr R2,=Ptr_Tab_E_2
	ldr R3,=N_2
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_3:
	ldr R1,=TabE_3
	ldr R2,=Ptr_Tab_E_3
	ldr R3,=N_3
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_4:
	ldr R1,=TabE_4
	ldr R2,=Ptr_Tab_E_4
	ldr R3,=N_4
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_5:
	ldr R1,=TabE_5
	ldr R2,=Ptr_Tab_E_5
	ldr R3,=N_5
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_6:
	ldr R1,=TabE_6
	ldr R2,=Ptr_Tab_E_6
	ldr R3,=N_6
	ldrsh R3,[R3]
	b Init_Ptr
Init_Table_7:
	ldr R1,=TabE_7
	ldr R2,=Ptr_Tab_E_7
	ldr R3,=N_7
	ldrsh R3,[R3]
	b Init_Ptr


Init_Ptr:
	str R1,[R2]
/* boucle d init */
	mov R4,#0
	mov R5,#0
Boucle_Init_Table:
	strh R5,[R1,R4,lsl #1] /* (R5) => [@Tab_E_i + R4*2]*/
	add  R4,#1
	cmp R4,R3 /* compare lindice avec N_i*/ 
	bne Boucle_Init_Table



Fin_Fonction:
	/* restaure registres*/
	pop {R1-R5}

    bx lr

/* VALIDATION
  Init_FIR_asm OK, check des filtre 0 et 5, avec une init pipo à 0xaa !
  	- Ptr bien réglé
	- table bien initialisée.
  
  s32 FIR_asm(char NumFiltre,s16 entree); OK
  	- testé en détail au niveau des gestions de tables
	- validé sur les même stimuli que sous matlab avec un sin cardinal de 60 pts.
		à 40MHz, simu 103, on tombe à 24us pour le calcul d'un nouveau point.

  

*/
