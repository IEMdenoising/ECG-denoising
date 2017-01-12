function u = isimf(x,env_min,env_max)
%UNTITLED Checks if function given as x vector meets the conditios of imf 
%   x- input function
%   env_min- lower envelope
%   env_max- upper envelope

N  = length(x);
u1 = sum(x(1:N-1).*x(2:N) < 0);
u2 = length(env_min)+length(env_max);
if abs(u1-u2) > 1, u = 0;
else              
    u = 1;
end

end

