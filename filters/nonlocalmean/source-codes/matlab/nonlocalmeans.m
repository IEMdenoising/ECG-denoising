function [ filtered_signal ] = nonlocalmeans( signal, lambda, P, PatchHW)
%nonlocalmeans Summary of this function goes here
%   Implementacja algorytmu fast NLM dla sygna�u EKG
% INPUTS:
% signal - sygna� do odfiltrowania
% lambda - wsp�czynnik Gaussa
% P - maksymalny zakres poszukiwa�
% PatchHW - skala zakresu por�wnania
%
%OUTPUTS:
% filtered_signal - sygna� przefiltrowany

N = length(signal);

%Obszar poszukiwa�
Delta = -P:P;

% Inicjalizacja sygna�u przefiltrowanego
filtered_signal = NaN*ones(size(signal));

% Przygotowanie brzeg�w sygna�u
iStart=1;
iEnd = N;
filtered_signal(iStart:iEnd) = 0;

% inicjalizacja wag normalizacji
Z = zeros(size(signal));   

% Zamiana lambdy na parametr h 
Npatch = 2*PatchHW+1;
h = 2*Npatch*lambda^2;

for idx = Delta  % iteracja po wszystkich mo�liwych r�nicach s-t
 
    % sumowanie po p  - R�wnanie 3
    ka=1:N;
    kplus = ka+idx;
    igood = find(kplus>0 & kplus<=N);  % dane OOB s� ignorowane
    SSD=zeros(size(ka));
    SSD(igood) = (signal(ka(igood))-signal(kplus(igood))).^2;
    Sdx = cumsum(SSD);
   

    for ii=iStart:iEnd  % iteracja po wszystkich punktach 's'
        if ii-PatchHW-1 > 0 && ii+PatchHW < N
            distance = Sdx(ii+PatchHW) - Sdx(ii-PatchHW-1); % R�wnanie 4
            % -1 aby uwzglednic punkt ii-iPatchHW

            w = exp(-distance/h); 
            t = ii+idx;  

            if t>1 && t<=N
                filtered_signal(ii) = filtered_signal(ii) + w*signal(t); % R�wnanie 1
                Z(ii) = Z(ii) + w;
            end
        end

    end
end % iteracja po wszystkich mo�liwych przesuni�ciach

% Normalizacja
filtered_signal = filtered_signal./(Z+eps);

end

