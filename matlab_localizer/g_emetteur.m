% function [g]=g_emetteur(f)
% renvoie le vecteur des gains (uBar/V) en dB de l'émetteur ma40b8s 
% pour les fréquences données dans le vecteur f
%____
% Exemple
% f = 30e3:1e3:50e3 ;
% plot(f,g_emetteur(f))
function [g]=g_emetteur(f)
g =  g_emetteur_0dB(f) + 20*log10(2e-4/(10*2/sqrt(2)));     % gains lus sur la doc. 
