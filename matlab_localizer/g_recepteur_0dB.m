% function [g]=g_recepteur_0dB(f)
% renvoie le vecteur de gains en dB du récepteur ma40s4r 
% pour les fréquences données dans le vecteur f
% 0dB = 1Vrms/uBar  Sound Pressure level 
%____
% Exemple
% f = 30e3:1e3:50e3;
% plot(f,g_recepteur(f))
function [g]=g_recepteur_0dB(f)
 sf=[36     39      39.5        39.8  40    41.5    43.2    44.3    44.4]*1e3; % fréquence des mesures
 ss=[-80    -67      -64       -63    -63   -73     -80     -83     -83.1];      % gain lu dans la doc.

 % interpolation du gain pour les fréquences de f
 g=interp1(sf,ss,f,'linear',0);
 
 % extrapolation pour les éléments de f en dehors de sf
 g = g + ...
     (f<sf(1)).*(ss(1) + (f-sf(1)) / (sf(2)-sf(1)) * (ss(2)-ss(1))) + ...
     (f>sf(end)).*(ss(end) + (f-sf(end)) / (sf(end)-sf(end-1)) * (ss(end)-ss(end-1)));
