function err=step_err_corr(toas,sig,corrs,noise_and_sig_power,corrsigs)

for k=1:sig.nb
    corr_rec(k,:) =   noise_and_sig_power(k)*((sig.t-toas(k))>0);
end
err = corrs-corr_rec;
err = sum(sum(err.^2));