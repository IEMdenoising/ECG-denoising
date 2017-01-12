load('100_raw.dat');
load('101_raw.dat');
load('102_raw.dat');
load('103_raw.dat');
load('104_raw.dat');
load('105_raw.dat');
load('106_raw.dat');
load('107_raw.dat');
load('108_raw.dat');
load('109_raw.dat');

signal1 = X100_raw(1:649911)';
signal2 = X101_raw(1:649911)';
signal3 = X102_raw(1:649911)';
signal4 = X103_raw(1:649911)';
signal5 = X104_raw(1:649911)';
signal6 = X105_raw(1:649911)';
signal7 = X106_raw(1:649911)';
signal8 = X107_raw(1:649911)';
signal9 = X108_raw(1:649911)';
signal10 = X109_raw(1:649911)';

izolinia1 = mean(signal1 - X100_lab);
izolinia2 = mean(signal2 - X101_lab);
izolinia3 = mean(signal3 - X102_lab);
izolinia4 = mean(signal4 - X103_lab);
izolinia5 = mean(signal5 - X104_lab);
izolinia6 = mean(signal6 - X105_lab);
izolinia7 = mean(signal7 - X106_lab);
izolinia8 = mean(signal8 - X107_lab);
izolinia9 = mean(signal9 - X108_lab);
izolinia10 = mean(signal10 - X109_lab);

load('ecg_BW100_mat_filtered.txt');
load('ecg_BW101_mat_filtered.txt');
load('ecg_BW102_mat_filtered.txt');
load('ecg_BW103_mat_filtered.txt');
load('ecg_BW104_mat_filtered.txt');
load('ecg_BW105_mat_filtered.txt');
load('ecg_BW106_mat_filtered.txt');
load('ecg_BW107_mat_filtered.txt');
load('ecg_BW108_mat_filtered.txt');
load('ecg_BW109_mat_filtered.txt');

filtered_cpp_100 = ecg_BW100_mat_filtered(1:649911)' - izolinia1;
filtered_cpp_101 = ecg_BW101_mat_filtered(1:649911)' - izolinia2;
filtered_cpp_102 = ecg_BW102_mat_filtered(1:649911)' - izolinia3;
filtered_cpp_103 = ecg_BW103_mat_filtered(1:649911)' - izolinia4;
filtered_cpp_104 = ecg_BW104_mat_filtered(1:649911)' - izolinia5;
filtered_cpp_105 = ecg_BW105_mat_filtered(1:649911)' - izolinia6;
filtered_cpp_106 = ecg_BW106_mat_filtered(1:649911)' - izolinia7;
filtered_cpp_107 = ecg_BW107_mat_filtered(1:649911)' - izolinia8;
filtered_cpp_108 = ecg_BW108_mat_filtered(1:649911)' - izolinia9
filtered_cpp_109 = ecg_BW109_mat_filtered(1:649911)' - izolinia10;

load('100_lab.dat');
load('101_lab.dat');
load('102_lab.dat');
load('103_lab.dat');
load('104_lab.dat');
load('105_lab.dat');
load('106_lab.dat');
load('107_lab.dat');
load('108_lab.dat');
load('109_lab.dat');

filtered_matlab_100 = X100_lab;
filtered_matlab_101 = X101_lab;
filtered_matlab_102 = X102_lab;
filtered_matlab_103 = X103_lab;
filtered_matlab_104 = X104_lab;
filtered_matlab_105 = X105_lab;
filtered_matlab_106 = X106_lab;
filtered_matlab_107 = X107_lab;
filtered_matlab_108 = X108_lab;
filtered_matlab_109 = X109_lab;




rmse1 = sqrt(mean((filtered_matlab_100 - filtered_cpp_100).^2));
rmse2 = sqrt(mean((filtered_matlab_101 - filtered_cpp_101).^2));
rmse3 = sqrt(mean((filtered_matlab_102 - filtered_cpp_102).^2));
rmse4 = sqrt(mean((filtered_matlab_103 - filtered_cpp_103).^2));
rmse5 = sqrt(mean((filtered_matlab_104 - filtered_cpp_104).^2));
rmse6 = sqrt(mean((filtered_matlab_105 - filtered_cpp_105).^2));
rmse7 = sqrt(mean((filtered_matlab_106 - filtered_cpp_106).^2));
rmse8 = sqrt(mean((filtered_matlab_107 - filtered_cpp_107).^2));
rmse9 = sqrt(mean((filtered_matlab_108 - filtered_cpp_108).^2));
rmse10 = sqrt(mean((filtered_matlab_109 - filtered_cpp_109).^2));
