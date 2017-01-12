function [TIME, SIGNAL] = read_dat_file (REC_NR)
    clc; clear all;

    %------ SPECIFY DATA ------------------------------------------------------
    PATH= '..\'; % path where data are saved

    %--------------------------------------------------------------------------
    HEADERFILE = strcat(num2str(REC_NR), '.hea');   % header-file in text format
    DATAFILE = strcat(num2str(REC_NR), '.dat');     % data-file
    SAMPLES2READ=30000;                             % number of samples to be read

    %------ LOAD HEADER DATA --------------------------------------------------
    signalh= fullfile(PATH, HEADERFILE);
    fid1=fopen(signalh,'r');
    z= fgetl(fid1);
    A= sscanf(z, '%*s %d %d %d',[1,3]);
    nosig= A(1);  % number of signals
    sfreq=A(2);   % sample rate of data
    clear A;
    for k=1:nosig
        z= fgetl(fid1);
        A= sscanf(z, '%*s %d %d %d %d %d',[1,5]);
        dformat(k)= A(1);           % format; here only 212 is allowed
        gain(k)= A(2);              % number of integers per mV
        bitres(k)= A(3);            % bitresolution
        zerovalue(k)= A(4);         % integer value of ECG zero point
        firstvalue(k)= A(5);        % first integer value of signal (to test for errors)
    end;
    fclose(fid1);
    clear A;

    %------ LOAD BINARY DATA --------------------------------------------------
    if dformat~= [212,212], error('this script does not apply binary formats different to 212.'); end;
    signald= fullfile(PATH, DATAFILE);            % data in format 212
    fid2=fopen(signald,'r');
    A= fread(fid2, [3, SAMPLES2READ], 'uint8')';  % matrix with 3 rows, each 8 bits long, = 2*12bit
    fclose(fid2);
    M2H= bitshift(A(:,2), -4);
    M1H= bitand(A(:,2), 15);
    PRL=bitshift(bitand(A(:,2),8),9);     % sign-bit
    PRR=bitshift(bitand(A(:,2),128),5);   % sign-bit
    M( : , 1)= bitshift(M1H,8)+ A(:,1)-PRL;
    M( : , 2)= bitshift(M2H,8)+ A(:,3)-PRR;
    if M(1,:) ~= firstvalue, error('inconsistency in the first bit values'); end;
    switch nosig
    case 2
        M( : , 1)= (M( : , 1)- zerovalue(1))/gain(1);
        M( : , 2)= (M( : , 2)- zerovalue(2))/gain(2);
        TIME=(0:(SAMPLES2READ-1))/sfreq;
    case 1
        M( : , 1)= (M( : , 1)- zerovalue(1));
        M( : , 2)= (M( : , 2)- zerovalue(1));
        M=M';
        M(1)=[];
        sM=size(M);
        sM=sM(2)+1;
        M(sM)=0;
        M=M';
        M=M/gain(1);
        TIME=(0:2*(SAMPLES2READ)-1)/sfreq;
    otherwise  % this case did not appear up to now!
        % here M has to be sorted!!!
        disp('Sorting algorithm for more than 2 signals not programmed yet!');
    end;
    SIGNAL = M;
    clear A M1H M2H PRR PRL;

%     %------ DISPLAY DATA ------------------------------------------------------
%     figure(1); clf, box on, hold on
%     plot(TIME, M(:,1),'r');
%     if nosig==2
%         plot(TIME, M(:,2),'b');
%     end;
% 
%     xlim([TIME(1), TIME(end)]);
%     xlabel('Time / s'); ylabel('Voltage / mV');
%     string=['ECG signal ',DATAFILE];
%     title(string);
end
