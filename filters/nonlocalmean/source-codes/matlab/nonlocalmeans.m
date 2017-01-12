function [ filtered_signal ] = nonlocalmeans( signal, lambda, P, PatchHW)
%nonlocalmeans Summary of this function goes here
%   Implementacja algorytmu fast NLM dla sygna³u EKG
% INPUTS:
% signal - sygna³ do odfiltrowania
% lambda - wspó³czynnik Gaussa
% P - maksymalny zakres poszukiwañ
% PatchHW - skala zakresu porównania
%
%OUTPUTS:
% filtered_signal - sygna³ przefiltrowany

N = length(signal);

%Obszar poszukiwañ
Delta = -P:P;

% Inicjalizacja sygna³u przefiltrowanego
filtered_signal = NaN*ones(size(signal));

% Przygotowanie brzegów sygna³u
iStart=1;
iEnd = N;
filtered_signal(iStart:iEnd) = 0;

% inicjalizacja wag normalizacji
Z = zeros(size(signal));   

% Zamiana lambdy na parametr h 
Npatch = 2*PatchHW+1;
h = 2*Npatch*lambda^2;

for idx = Delta  % iteracja po wszystkich mo¿liwych ró¿nicach s-t
 
    % sumowanie po p  - Równanie 3
    ka=1:N;
    kplus = ka+idx;
    igood = find(kplus>0 & kplus<=N);  % dane OOB s± ignorowane
    SSD=zeros(size(ka));
    SSD(igood) = (signal(ka(igood))-signal(kplus(igood))).^2;
    Sdx = cumsum(SSD);
   

    for ii=iStart:iEnd  % iteracja po wszystkich punktach 's'
        if ii-PatchHW-1 > 0 && ii+PatchHW < N
            distance = Sdx(ii+PatchHW) - Sdx(ii-PatchHW-1); % Równanie 4
            % -1 aby uwzglednic punkt ii-iPatchHW

            w = exp(-distance/h); 
            t = ii+idx;  

            if t>1 && t<=N
                filtered_signal(ii) = filtered_signal(ii) + w*signal(t); % Równanie 1
                Z(ii) = Z(ii) + w;
            end
        end

    end
end % iteracja po wszystkich mo¿liwych przesuniêciach

% Normalizacja
filtered_signal = filtered_signal./(Z+eps);

end

