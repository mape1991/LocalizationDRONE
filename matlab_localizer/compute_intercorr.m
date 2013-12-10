
clear all;
close all;
clc;

global c;      % sound celerity
c=330;

geo=new_geometry();
geo.target.v = (geo.grid.v_min+geo.grid.v_max)/2;
sig.t_start = 0.000;
sig.Valim=10;
sig.NoiseRMS = 0*10e-3;
sig.H = 1/geo.beacon.df;

sig = new_signal(geo,sig);


signal = 0*sin(2*pi*sig.f{2}*sig.t);

for k=1:geo.beacon.n
   sig.sin{k} = sin(2*pi*sig.f{k}*(sig.t)) .* ((sig.t)>0);
   sig.s = signal + sig.sin{k};
   filt = new_correlators(sig);
   corrsigs{k}=filt.corr;
   corrint(:,k)=squeeze(sum(abs(filt.corr),2));
   corrmax(:,k)=squeeze(max(abs(filt.corr)')');
   figure(k);
   subplot(211)
   plot(abs(filt.corr)'); hold on;
   subplot(212)
   plot(unwrap(angle(filt.corr)')/pi); hold on;
   
end

save 'corr_data' corrint corrmax corrsigs
    
%end