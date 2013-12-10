% function [g]=g_emetteur_0dB(f)
% renvoie le vecteur de gains en dB de l'�metteur ma40b8s 
% pour les fr�quences donn�es dans le vecteur f
% 0db= 2e-4 uBar Sound Pressure Level capt�e � 30cm de la source aliment�e � 10Vrms
%____
% Exemple
% f = 30e3:1e3:50e3 ;
% plot(f,g_emetteur_0dB(f))
function [g]=g_emetteur_0dB(f)
 sf=[32.5 33    35 40   42.5    43  46  48  50]*1e3; % fr�quences des mesures
 ss=[80   90    98 120  112     110 103 103 96];     % gains lus sur la doc. 

 g=interp1(sf,ss,f,'linear',0); % interpolation lin�aire pour les fr�quences de f 
 
 % extrapolation si les �l�ments de f son en dehors de sf  
 g = g + ...
     (f<sf(1)).*(ss(1) + (f-sf(1)) / (sf(2)-sf(1)) * (ss(2)-ss(1))) + ...
     (f>sf(end)).*(ss(end) + (f-sf(end)) / (sf(end)-sf(end-1)) * (ss(end)-ss(end-1)));
