%%______________________________________________
% Conception d'un filtre FIR de la forme
%         _ N-1
%  s(k) = \        a(n).e(k-n)
%         /_ n=0
% Et g�n�ration du code C associ� � ce filtre

clc       % efface la console
clear all % efface les variables du workspace
close all % ferme toutes les fen�tres graphiques



%% MODIF s�lectionner le filtre � g�n�rer
%monfiltre='rect'; % dans ce cas modifez le M et Ns du filtre ligne 50-53
%monfiltre='sinc'; % dans ce cas modifiez le M et Ns du filtre lignes 69-72
monfiltre='pers'; %   d�finissez le M, Ns et h du filtre lignes 93-97

%% MODIF donner un nom au .c � g�n�rer
%Nom_Fich='FIR_0.c';
%Nom_Fich='FIR_1.c';
%Nom_Fich='FIR_2.c';
Nom_Fich='FIR_3.c';
%Nom_Fich='FIR_4.c';
%Nom_Fich='FIR_5.c';
%Nom_Fich='FIR_6.c';
%Nom_Fich='FIR_7.c';



%%
% Calcul des coeffitients a(n) du filtre :
% On rapelle que la r�ponse impulsionelle h(k) d'un FIR est la s�rie
% des coefficients a(n) 
% temps ------------------------------------------------------------->
% e(k) =  0   0   0   1       0       0       ...   0         0       0
% s(k) =  0   0   0   a(0)    a(1)    a(2)    ...   a(N-1)    0       0 
%                     =h(1)   =h(2)   =h(3)   ...   =h(N)
% les a(n) sont stock�s dans un vecteur h dont l'indice commence � 1
% car contrairement au langage C il n'y a pas d'indice 0 en Matlab 


%% Exemple d'un filtre rectangle
%_________________________________________________________________________
if monfiltre=='rect'
% temps ------------------------------------------------------------->
% e(k) =  0   0   0   1    0    0    ...   0    0       0
% s(k) =  0   0   0   1    1    1    ...   1    0       0
%                     h(1) h(2) h(3) ...   h(N)
M=10; % MODIF facteur de sur�chantillonage du moment de symbole :
      % M points par moment de symbole

Ns=1; % MODIF facteur d'�talement de la r�ponse impulsionelle :
      % la r�ponse d'une impulsion s'�tend sur Ns moments de symbole
      % Dans ce cas 1 seul moment

N = M*Ns; % L'ordre du filtre doit couvris Ns symboles de M �chantillons :
         % dans cet exemple 1 symbole de 10 �chantillons donne un filtre
         % d'ordre 10  
for k = 1:N
    h(k)=1;
end
end % Filtre rectangle
%_________________________________________________________________________

%% Exemple d'un filtre Sinus Cardinal tronqu� �tal� sur Ns symboles
%_________________________________________________________________________
if monfiltre=='sinc'
M=10; % MODIF facteur de sur�chantillonage du moment de ssymbole :
      % M points par moment de symbole

Ns=6; % MODIF facteur d'�talement de la r�ponse impulsionelle :
      % la r�ponse d'une impulsion s'�tend sur Ns moments de symbole
      % ATTENTION � la condition de non-interf�rence inter symbole !
      % la r�ponse d'une impulsion doit �tre nulle sur les moments suivants
      % pour ne pas interf�rer avec les moments suivants voir la figure 1
      
N = M*Ns; % L'ordre du filtre doit couvris Ns symboles de M �chantillons :
         % dans cet exemple 6 symboles de 10 �chantillons donne un filtre
         % d'ordre 60  

for k = 1:N
    t = (k-1)/M - Ns/2; % temps qui vaut -1 au premier symbole ; 0 au second etc
    h(k)=sinc(t);
end

end % Filtre sinc
%_________________________________________________________________________

%% Votre propre design de filtre
%_________________________________________________________________________
if monfiltre=='pers'
M=256;    % MODIF facteur de sur�chantillonnage
Ns=1;   % MODIF facteur d'�talement
N = M*Ns;

% h = 1; %MODIF r�ponse impulsionelle en N �chantillons
for k = 1:N
    h(k)=sin(2*pi*40750*k/128000);
end
end % Filtre personnel
%_________________________________________________________________________




%% Visualisation de la r�ponse du filtre
%_________________________________________________________________________
temps = 1 : (M+2*N);
temps = (temps - 1)/M;

entree = temps*0;
entree(N+1)=1;  %sur-�chantillonne de M de l'impulsion

sortie=filter(h,1,entree); % passage dans le filtre

figure
title('Temporel et fr�quentiel du filtre');
subplot(121);
stem(temps,entree,'gs');hold on;
stem(temps,sortie,'r')
plot(Ns,1.4)
line ([Ns Ns+Ns/2],[1.05 1.05]);
line ([Ns+Ns/2-0.1 Ns+Ns/2],[1.07 1.05]);
line ([Ns+Ns/2-0.1 Ns+Ns/2],[1.03 1.05]);
text(Ns,1.15,['Remarquez le d�lais'])
text(Ns,1.11,[' de groupe de ' num2str(Ns/2) ' symboles']);
grid on;
xlabel('temps en moments de symboles');
ylabel('R�ponse impulsionnelle');
subplot(122)
Np=2^nextpow2(max(1024,(M+N)));
Df = M/Np;
freq = 0 : Df : M-Df;
freq = freq-M/2;
Hf=abs(fft(h,Np)); % TFD
plot(freq,fftshift(Hf),'g'); hold on;
maxH=max(Hf);
line([-1 -1],[0 maxH]);
line([1 1],[0 maxH]);
text(1.1,maxH*0.91,'+/-B (Hz)');
xlabel('Fr�quence unitaire (en D�bit de moment B)');
ylabel('Spectre du filtre');
grid on;

%% Simulation de N symboles binaires transmit
%_________________________________________________________________________
L=10; % on simule L symboles

temps = 1 : (L*M+N);
temps = (temps - 1)/M;

bits = 2*(randn(1,L)>0)-1; % tire L bits au hasard
entree = temps*0;
entree(1:M:(M*L))=bits;  %sur-�chantillonne de M les L bits de data

sortie=filter(h,1,entree); % passage dans le filtre

% affichage
figure
title('Simulation de quelques bits transmis par ce filtre');
subplot(211);
stem(temps,entree,'g'); 
xlabel('temps en moments de symboles');
ylabel('entree');
grid on;
subplot(212)
stem(temps,sortie); hold on;
line ([0 Ns/2],[bits(1)*1.2 bits(1)*1.2]);
line ([Ns/2-0.1 Ns/2],[bits(1)*1.2+0.05 bits(1)*1.2]);
line ([Ns/2-0.1 Ns/2],[bits(1)*1.2-0.05 bits(1)*1.2]);

line ([Ns/2 Ns/2+0.5],[bits(1)*1.2 bits(1)*1.5]);
text(Ns/2+0.5,bits(1)*1.5,['Remarquez le d�lais de groupe de ' num2str(Ns/2) ' symboles']);
stem(temps(Ns/2*M+1:M:end),sortie(Ns/2*M+1:M:end),'rs');
xlabel('temps en moments de symboles');
ylabel('bande de base');
grid on;

%% g�n�ration ddu fichier .c avec les coefficients
%_________________________________________________________________________
disp('G�n�re les coefficients suivants :');
disp(h);
disp(['Dans le fichier ' Nom_Fich]);

fileID = fopen([Nom_Fich], 'w');
  fprintf(fileID,'#include "stm32f10x.h"\n');
 fprintf(fileID,'#include "FIR_Filter.h"\n');
 fprintf(fileID,'\n');
 fprintf(fileID,'\n');
 fprintf(fileID,'//_________________________________________________');
 fprintf(fileID,'\n');
 fprintf(fileID,'//      DESCRIPTION DU FILTRE FIR_0, 8.24 \n');
 fprintf(fileID,'//_________________________________________________');
 fprintf(fileID,'\n');
 fprintf(fileID,'\n');
 
 fprintf(fileID,'const u16 N_0 = %d; \n', N); 
 fprintf(fileID,'s16 TabE_0[%d]; \n', N); 
 fprintf(fileID,'s16 *Ptr_Tab_E_0; \n');  
 fprintf(fileID,'\n');
 fprintf(fileID,'const s32 h_0[%d]= {\n',N); 
 
 for  k = 1: N
     fprintf(fileID,'K_8_24*%d,\n',h(k));
 end
 fprintf(fileID,'};\n'); 
 
 fclose(fileID);
 

  
    
  