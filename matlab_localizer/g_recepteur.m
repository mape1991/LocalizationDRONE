% function [g]=g_recepteur(f)
% renvoie le vecteur de gains en dB du r�cepteur ma40s4r 
% pour les fr�quences donn�es dans le vecteur f
% 0dB = 1V/uBar  Sound Pressure level 
%____
% Exemple
% f = 30e3:1e3:50e3 ;
% plot(f,g_recepteur(f))
function [g]=g_recepteur(f)
g= g_recepteur_0dB(f)+20*log10(2/sqrt(2));      % gain lu dans la doc.
