#include "stm_regs.h"
#include "FIR_Filter.h"

// Filtre FIR_0


// Le filtre au format 8.24
const s32 h_0[5]= {
K_8_24*0.5,
-K_8_24*0.7,
K_8_24*0.8,
-K_8_24*0.7 ,
 K_8_24*0.5, 
 };

const u16  N_0=5;
s16 TabE_0[5];
s16 *Ptr_Tab_E_0;

