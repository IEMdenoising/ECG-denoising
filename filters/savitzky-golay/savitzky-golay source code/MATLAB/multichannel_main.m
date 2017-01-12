clc, clear;
close all
[TIME, M] = read_dat_file(109);

sig = M(1:5000,:);
time = TIME(1:5000);

lp = sav_gol_multichannel(sig, 1, 11);
hp = sav_gol_multichannel(lp, 2, 85);

%% section plot
figure;
plot(time, sig(:,1))
hold on
plot(time, lp(:,1), 'g')
hold on
plot(time, hp(:,1), 'r')
title('\fontsize{16}Filtracja pasmowoprzepustowa filtrem Savitzky-Golay (kana� 1)')
legend('Sygna� oryginalny', 'Sygna� po filtracji dolnoprzepustowej', 'Sygna� po filtracji pasmowoprzepustowej')
xlabel('Time [s]')

figure;
plot(time, sig(:,2))
hold on
plot(time, lp(:,2), 'g')
hold on
plot(time, hp(:,2), 'r')
title('\fontsize{16}Filtracja pasmowoprzepustowa filtrem Savitzky-Golay (kana� 2)')
legend('Sygna� oryginalny', 'Sygna� po filtracji dolnoprzepustowej', 'Sygna� po filtracji pasmowoprzepustowej')
xlabel('Time [s]')