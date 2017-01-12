function [IMFs, res] = EMD (signal)
%INFO:  Function that calculates  Intrinsic Mode Functions
%       of given signal using Empirical Mode Decomposition
%IN:    signal - signal to decompose
%OUT:   IMFs - an array of IMFs (each row include one IMF)
%       res - residual of the signal (signal minus sum of IMFs)


res = signal;                  % set initial value of residue as raw signal
n=1;
while ~(ismonotonic(res))      % check if residue is monothonic
    
    [mode,flag] = Extract_Mode (res );     % mode=imf
    
    if(flag==true)
        break
    end
 
    IMFs(n,:) = mode;
    
     
%   figure;
%   plot(time,IMFs(n,:))        % plot imf
%   hold on;
%   title(strcat(int2str(n), ' IMF'));
%   xlabel ('Czas [s]');
    n=n+1;
    res = signal-mode;     % residue = residue from previous iteration - imf
    signal=res;            % set new value of signal as residue
end

