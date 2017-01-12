function [ filtered ] = WWF( signal )
%WWF performs Wavelet Wiener Filtering of an ECG signal
%   Returns filtered signal
% Signal must be a vector of numeric data
% Uses fourth level wavelet decomposition with bior2.2 wavelet

%% normalization - sets constant  component of signal to 0
signal = signal';
med = median(abs(signal));
signal = signal + med;



%% wavelet coefficient thresholding 

SWC = swt(signal,4,'bior2.2');

sigma(1,:)= median(abs(SWC(1,:)))/0.6745;
sigma(2,:)= median(abs(SWC(2,:)))/0.6745;
sigma(3,:)= median(abs(SWC(3,:)))/0.6745;
sigma(4,:)= median(abs(SWC(4,:)))/0.6745;
sigma(5,:)= median(abs(SWC(5,:)))/0.6745;

threshold = 1*sigma;

for i = 1:5
    for j = 1:length(SWC)
        if (abs(SWC(i,j))<threshold(i,:))
        SWCT(i,j) = 0;
        else SWCT(i,j) = (SWC(i,j)-(threshold(i,:)^2/SWC(i,j)));
        end
    end
end

ecgSWC = iswt(SWCT,'bior2.2');

SWCestimate = swt(ecgSWC,4,'bior2.2');  %% ecgSWC is time domain estimate of noise-free signal
SWCnoisy = swt(signal,4,'bior2.2');



%% Wiener correction factor

SWClength = length(SWCestimate);

for i = 1:5
    for j = 1:SWClength
        SWCestimateSquared(i,j) = SWCestimate(i,j)^2;
    end
end


for i = 1:5
    for j = 1:SWClength
        CORR(i,j) = SWCestimateSquared(i,j)/(SWCestimateSquared(i,j)+sigma(i,:));
        
    end
end


for i = 1:5
    for j = 1:SWClength
        SWCresult(i,j) = SWCnoisy(i,j)*CORR(i,j);
        
    end
end

filtered = iswt(SWCresult,'bior2.2');

% x = 1:length(result);
% figure;
% plot(x,ecg,x,result)
% xlabel('[n]')
% ylabel('Voltage [mV]')
% axis([0 2000 -0.4 1.2])
% legend('input signal', 'output signal')

end
