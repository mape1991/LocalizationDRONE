
clear all;
close all;
clc;

global c;      % sound celerity
c=330;

geo=new_geometry()

%% set parameters of signals
sig.t_start = 0.004;
sig.Valim=10;
sig.NoiseRMS = 1*10e-3;
sig.H = 0.060;

sig = new_signal(geo,sig)




%% plot spectrums
figure
plot(sig.sfreqs,abs(fftshift(fft(sig.s)))/sig.N);
hold on;
plot(sig.sfreqs,abs(fftshift(fft(sig.s((end-sig.Nh+1):end),sig.N)))/sig.Nh,'r');



%% apply correlators filters
filt = new_correlators(sig);

%% 
figure
    subplot(211);
   plot(sig.t,abs(filt.corr)); hold on;
 
   
%% measure noise
noise_power = mean(abs(filt.corr(:,sig.Nh:sig.t_start*sig.Fs)),2);
noise_and_sig_power = mean(abs(filt.corr(:,sig.t_stop*sig.Fs:end)),2);
snr = noise_and_sig_power./noise_power

integration = cumsum(abs(filt.corr),2);
subplot(212);
plot(sig.t,integration);hold on;
plot(sig.t, noise_power * (0:sig.N-1),'--');
plot(sig.t,integration(:,end)* ones(1,sig.N) + (noise_and_sig_power) * (-(sig.N-1):0),'--');

%% compute toa from intégrals
toas = ( integration(:,end) - noise_and_sig_power * (sig.N-1))./(noise_power - noise_and_sig_power) * sig.Ts


load corr_data


%toas = fminsearch(@(x) step_err_corr(x,sig,abs(filt.corr),noise_and_sig_power,corrsigs),toas)

toas = fminsearch(@(x) quad_err_corr(x,sig,abs(filt.corr),noise_and_sig_power,corrsigs),toas)

sig_rec = 0*sig.t;
for k=1:sig.nb
    sig_rec = sig_rec + noise_and_sig_power(k) / corrmax(1,1) * sin(2*pi*sig.f{k}*(sig.t-toas(k))).*((sig.t-toas(k))>0);
end

nstop=sig.Nh;
ntoas = toas*sig.Fs;
for k=1:sig.nb
    corr_rec(k,:) = sig.t*0;
    for m=1:sig.nb
    corr_rec(k,ntoas(m):(ntoas(m)+sig.Nh-1)) =  corr_rec(k,ntoas(m):(ntoas(m)+sig.Nh-1))+  noise_and_sig_power(m)/abs(corrsigs{m}(m,end))*abs(corrsigs{m}(k,1:sig.Nh));
    end
    corr_rec(k,(ntoas(k)+sig.Nh):end) = corr_rec(k,(ntoas(k)+sig.Nh):end) + ones(1,sig.N-ntoas(k)-sig.Nh+1)*noise_and_sig_power(k);
end

figure
subplot(211);
plot(sig.t,sig_rec); hold on;
plot(sig.t,sig.s,'r'); hold on;
subplot(212);
plot(sig.t,sig_rec-sig.s); hold on;

for nn=1:4
figure

subplot(211);
plot(sig.t,corr_rec(nn,:)); hold on;
plot(sig.t,abs(filt.corr(nn,:)),'r'); hold on;
subplot(212);
plot(sig.t,corr_rec(nn,:)-abs(filt.corr(nn,:))); hold on;
end


figure(2)
subplot(211)
stem(toas', noise_and_sig_power' );
%filt = new_toas(geo,sig);

%for k=1:geo.beacon.n
%   plot(sig.t,real(filt.corr));hold on;
%   plot(sig.t,imag(filt.corr));
   
   
%end