function sig = new_signal(geo,sig)

global c;      % sound celerity


%sig.Fs = (geo.beacon.f_base)/5;  % underample to alias f_base on 0 Hz
sig.Fs = 192e3;  % oversample
sig.Ts = 1 / sig.Fs;

%% start , delay spread and stop time
sig.delay_spread = geo.delay_spread;
sig.t_stop = sig.delay_spread +sig.t_start;
sig.f=geo.beacon.f;
sig.df=geo.beacon.df;
sig.f_base=geo.beacon.f_base;
sig.nb=geo.beacon.n;


%% all signal parameters

sig.t = 0 : sig.Ts : sig.H;
sig.N = length(sig.t);
sig.s = sig.t*0;
sig.freqs = (0:(sig.N-1))*sig.Fs/sig.N ;   % frequencies of the length N fft
sig.sfreqs = sig.freqs - sig.freqs(floor(sig.N/2)+1); % symetric frequencies for plot with fft shift)


%% orthogonal window parameters
sig.Hh = 1/geo.beacon.df;
sig.th = 0 : sig.Ts : sig.Hh-sig.Ts;
sig.Nh = length(sig.th);
sig.freqsh = (0:(sig.Nh-1))*sig.Fs/sig.Nh ;   % frequencies of the length N fft
sig.sfreqsh = sig.freqsh - sig.freqsh(floor(sig.Nh/2)+1); % symetric frequencies for plot with fft shift)



for k=1:geo.beacon.n
   sig.d{k} = sqrt(sum((geo.beacon.v{k} - geo.target.v).^2)); 
   sig.gain{k} = g_emetteur(geo.beacon.f{k})+g_recepteur(geo.beacon.f{k})+ 20*log10(0.6/sig.d{k});
   sig.a{k} = sig.Valim * 10^(sig.gain{k}/20);
   sig.delay{k} = sig.d{k}/c;
%   sig.sin{k} = sig.a{k} * sin(2*pi*geo.beacon.f{k}*(sig.t-sig.delay{k})) .* ((sig.t-sig.delay{k}-sig.t_start)>0);
   sig.sin{k} = sig.a{k} * sin(2*pi*geo.beacon.f{k}*(sig.t-sig.delay{k}-sig.t_start)) .* ((sig.t-sig.delay{k}-sig.t_start)>0);
   sig.s = sig.s + sig.sin{k};
end

%% insert noise on signal
sig.s = sig.s + randn(1,sig.N)*sig.NoiseRMS;
