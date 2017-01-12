
ecg=load('109.dat','-ascii');

%% modu³ pomiaru czasu filtracji
t=cputime;
filtered=WWF(ecg);
e=cputime-t

%% modu³ wyznaczania RMSE

data=load('109_lab.dat','-ascii');
filtered=filtered(1:numel(data)); %wyrównanie d³ugoœci sygna³ów

rmse =sqrt(sum((data(:)-filtered(:)).^2)/numel(data));