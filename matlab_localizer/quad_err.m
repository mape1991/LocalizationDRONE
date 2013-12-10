function err=quad_err(toas,sig,noise_and_sig_power,corrmax)


sig_rec = 0*sig.t;
for k=1:sig.nb
    sig_rec = sig_rec + noise_and_sig_power(k) / corrmax(1,1) * sin(2*pi*sig.f{k}*(sig.t-toas(k))).*((sig.t-toas(k))>0);
end

err = sig.s-sig_rec;
err = sum(err.^2);