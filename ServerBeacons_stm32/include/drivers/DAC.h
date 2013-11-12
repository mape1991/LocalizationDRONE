

//=======================================================================================

// 	Auteur T.Rocacher
//  revu Mars 2011
//  Processeur : STM32F107VC
//  Logiciel dev : µVision 4
//  Module qui configure les DAC
//
//=======================================================================================


#ifndef _DAC_H__
#define _DAC_H__


//=======================================================================================
// Rappel sur les ressources 
// Broche : DAC1-PA4      DAC2-PA5 
//=======================================================================================

/*
Explications rapides sur le périph:

Approche "mono", écriture à la main dans chaque DAC
DAC_DHR8R1 ---------> DHR1-----trig ext ou pas--> DOR1 ----> DAC1
DAC_DHR8L1 ------------^
DAC_DHR12R1------------^

DAC_DHR8R2 ---------> DHR2-----trig ext ou pas--> DOR2 ----> DAC2
DAC_DHR8L2 ------------^
DAC_DHR12R2------------^

Approche "stéréo"
DAC_DHR8RD ---------> DHR1,DHR2 ----trig ext ou pas-->  DOR1,DOR2 ----> DAC1,DAC2 
DAC_DHR12LD -------->--^
DAC_DHR12RD	-------->--^




DOR : Data Output Reg read only, c'est lui qui est directement converti en tension
DHR1,DHR2 : DAC Holding Register non mappé 
DHR12R1 ou 2  : DAC Holding Register 12 bits Right justified 1 or 2 
              : mot à placer de en 11:0
DHR12L1 ou 2  : DAC Holding Register 12 bits Left justified 1 or 2  
			  : mot à placer de en 15:4
DHR8R1 ou 2	  :	DAC Holding Register 8 bits Right justified 1 or 2  
			  : mot à placer de en 7:0


DHR12RD		  : DAC Holding Register 12 bits Right justified Dual
			  : mot à placer 27:16 pour DAC2, 11:0 pour DAC1
DHR12LD	      : DAC Holding Register 12 bits Left justified Dual
			  : mot à placer 31:20 pour DAC2, 15:4 pour DAC1
DHR8RD	      : DAC Holding Register 8 bits Right justified Dual
			  : mot à placer 16:8 pour DAC2, 7:0 pour DAC1

*/
//_____________________________________________________________________________
// MONO
//_____________________________________________________________________________

// Ecriture sur DAC en 12 bits justifié à droite (16.0)
// Intervalle : [0 à 4095] logé dans un 16bits non signé
#define DAC_Mono_12_R(Channel,Valeur16bits) DAC->DHR12R##Channel=Valeur16bits

// Ecriture sur DAC en 12 bits justifié à gauche (12.4)
// Intervalle : [0 à 4095<<4] logé dans un 16bits non signé
#define DAC_Mono_12_L(Channel,Valeur16bits) DAC->DHR12L##Channel=Valeur16bits

// Ecriture sur DAC en 8 bits justifié à droite (16.0)
// Intervalle : [0 à 255] logé dans un 16bits non signé
#define DAC_Mono_8_R(Channel,Valeur16bits)  DAC->DHR8R##Channel=Valeur16bits

//_____________________________________________________________________________
// STEREO
//_____________________________________________________________________________

// Ecriture simultanée sur les deux DAC en 12 bits justifié à droite (32.0)
// Intervalle pour chaque variable: [0 à 4095] logé dans un 32bits non signé
#define DAC_Stereo_12_R(Valeur32bits_1,Valeur32bits_2)	DAC->DHR12RD=(Valeur32bits_2<<16)| (Valeur32bits_1&0x0000FFFF)

// Ecriture simultanée sur les deux DAC en 12 bits justifié à gauche (28.4)
// Intervalle pour chaque variable: [0 à 4095<<4] logé dans un 32bits non signé
#define DAC_Stereo_12_L(Valeur32bits_1,Valeur32bits_2)	DAC->DHR12LD=(Valeur32bits_2<<16)| (Valeur32bits_1&0x0000FFFF)

// Ecriture simultanée sur les deux DAC en 8 bits justifié à droite (32.0)
// Intervalle pour chaque variable: [0 à 255] logé dans un 32bits non signé
#define DAC_Stereo_8_R(Valeur32bits_1,Valeur32bits_2)	DAC->DHR8RD=(Valeur32bits_2<<8)| (Valeur32bits_1&0x000000FF)

void Init_DAC(void);
/*
on peut bufferiser la sortie (sortir à basse impédance). C'est pas fait (bit BOFF1 et 2).
Le DAC n'est pas triggé. Les sorties sont mises à jour à l'écriture des DAC holding reg.
*/

#endif
