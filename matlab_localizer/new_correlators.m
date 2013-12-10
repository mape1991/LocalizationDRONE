function filt = new_correlators(sig)


for k=1:sig.nb
    %% correlation with windowed frequencies f1, f2...
    
        filt.b{k}= exp(i*2*pi*sig.f{k} * sig.th);
    %   filt.b{k}= cos(2*pi*geo.beacon.f{k} * sig.th);
    filt.corr(k,:) = filter(filt.b{k},1,sig.s);

    %% compute sliding max filter to remove oscilations and get envelope
    Nm = ceil(sig.Fs/(sig.df+sig.df*(sig.nb-1)));
    for n=1:length(filt.corr(k,:))
       filt.maxcorr(k,n) = max(abs(filt.corr(k,max(1,n-Nm):n))); 
    end
end
