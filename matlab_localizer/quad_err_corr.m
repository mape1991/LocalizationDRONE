function err=quad_err_corr(toas,sig,corrs,noise_and_sig_power,corrsigs)

nstop=sig.Nh;
ntoas = toas*sig.Fs;
for k=1:sig.nb
    corr_rec(k,:) = sig.t*0;
    for m=1:sig.nb
    corr_rec(k,ntoas(m):(ntoas(m)+sig.Nh-1)) =  corr_rec(k,ntoas(m):(ntoas(m)+sig.Nh-1))+  noise_and_sig_power(m)/abs(corrsigs{m}(m,end))*abs(corrsigs{m}(k,1:sig.Nh));
    end
    corr_rec(k,(ntoas(k)+sig.Nh):end) = corr_rec(k,(ntoas(k)+sig.Nh):end) + ones(1,sig.N-ntoas(k)-sig.Nh+1)*noise_and_sig_power(k);
end
err = corrs-corr_rec;
err = sum(sum(err.^2));