% DENOISING SIGNAL USING EMD

[imfs, res]= EMD(signal); 
imfs=imfs';
sig=zeros(1,1);
n=3;                                    % number of noisy imfs
for i=1:n
sig=sig+imfs(:,i);                      % add noisy imfs
end
noised=sig;
figure;
plot(sig);
title('noisy imfs');

Wn=[5 60]*1/360;
[a,b] = butter(3,Wn);
filt_sig = filter(a,b,sig);             %filtration using IIR filter

figure;
plot(filt_sig)                          % show filtered signal
title('filtred imfs')
imfs(:,1:n-1)=[];                       % replace noisy imfs with filtered signal

imfs(:,1)=filt_sig;
sig=sum(imfs,2)+res;                    % add imfs and residue
sig=sig/(max(sig));
figure;
plot(sig)                               % show final filtered signal
title('final filtered signal')