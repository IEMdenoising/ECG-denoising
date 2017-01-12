function [mode,flag] = Extract_Mode( signal )
%INFO:  Function that calculate the Intrinsic Mode Function of given signal
%       based on its extemas using b-spline interpolation
%IN:    signal - analysed signal
%OUT:   mode - calculated IMF
%       res - residual signal (signal minus mode)

len = numel(signal);
t_spline = 1:len;          
d = signal;                 % detail, initially set as raw signal
sd = inf;                   % standard deviation, 
n=1;                        % numer of rows of details
flag=false;                 
% Flag indicates if sum of vectors lenghts of maximas and minimas is less
% than 3. If yes, there is no possible to evaluate pchip function.

 while(sd>0.3 )                    % threshold of sd=0.3

  [imin, imax] = extr(d);          % finding extrema

    vmin = d(imin);
    vmax = d(imax);
    
    if (length(imin)+length(imax))<3 | length(vmax)<=1 | length(vmin)<=1
        flag=true;
        break   % break if sum of vectors lenghts of maximas and minimas is 
                % less than 3
    end
     
    env_min = pchip(imin,vmin,t_spline);    % lower envelope
    env_max = pchip(imax,vmax,t_spline);    % upper envelope
    env_mean = (env_min + env_max) ./ 2 ;   % mean envelope

         

    d1 = d - env_mean';                     % extract detail for next iteration

  
    if isimf(d,env_min,env_max)
        break   % break if found imf
    end
    
    sd = sum((d-d1).^2)/sum(d.^2);
    
    n=n+1;
    d=d1;
  
end

                            
mode = d;                     % calculated IMF
if size(mode)~= size(signal)  % transpose imf
    mode=mode';
end

end



