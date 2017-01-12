
ecg=load('109.dat','-ascii');

%% modu� pomiaru czasu filtracji
t=cputime;
filtered=WWF(ecg);
e=cputime-t

%% modu� wyznaczania RMSE

data=load('109_lab.dat','-ascii');
filtered=filtered(1:numel(data)); %wyr�wnanie d�ugo�ci sygna��w

rmse =sqrt(sum((data(:)-filtered(:)).^2)/numel(data));