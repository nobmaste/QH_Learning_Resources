clc
clear
close all
% 创建模拟数据
fs = 1000;  % 抽样频率
T = 1/fs;  % 抽样周期
L = 1000;  % 信号长度
t = (0:L-1)*T;  % 时间向量

% 创建两个含有相同频率成分的信号
f = 50;  % 频率
% S1 = [zeros(1, 10) sin(2*pi*f*t(1:end-10))];  % 第一个信号
S1 = sin(2*pi*f*t);
delay = 60;  % 延迟
% S2 =  sin(2*pi*f*t-delay) ;%+ 0.5*randn(size(t))  ;  % + 1*sin(2*pi*100*t(1:end-delay))第二个信号，是第一个信号的延迟版本
S2 = [zeros(1, delay) S1(1:end-delay) + 0.5*randn(size(t(1:end-delay)))+ 1*sin(2*pi*100*t(1:end-delay))   ];  % 第二个信号，是第一个信号的延迟版本
S3= bandpass(S2,[20 80],fs);
S4= bandpass(S1,[20 80],fs);
figure('Name','原始信号');
subplot(211)
plot(t,S1);
hold on;
plot(t,S2);
axis([0 0.15 -1 1]);
title('orignal_signal');
hold off
subplot(212)
plot(t,S1);
hold on;
plot(t,S3);
axis([0 0.15 -1 1]);
title('filter_signal');

%算法
[rcc,lag]=xcorr(S2,S1);
T_S1=xcorr(S1);
T_S2=xcorr(S2);
S11=fft(T_S1);
S22=fft(T_S2);
RGCC=fft(rcc);
HB = abs(RGCC)./(S11.*S22);
Roth = 1./S11;
SCOT = 1./sqrt(S11.*S22);

%根据公式写
rgcc1=ifft(RGCC*1./abs(RGCC));%PATH加权
rgcc2=ifft(RGCC*1);%直接CC
rgcc3=ifft(RGCC.*HB);%HB加权
rgcc4=ifft(RGCC.*Roth);%Roth加权
rgcc5=ifft(RGCC.*SCOT);%SCOT加权

%% 
figure('Name','延时0.05，叠加高斯噪声','NumberTitle','off');
subplot(511)
plot(lag/fs,rgcc1,'.');
[M,I] = max(abs(rgcc1));

lagDiff = lag(I);
timeDiff = lagDiff/fs;
disp(timeDiff);

% 在图上标注出最大值的位置
hold on;
plot(timeDiff, rgcc1(I), 'ro');
text(timeDiff, rgcc1(I), ['  Max at ', num2str(timeDiff)]);
hold off;
title ('PATH加权');
%% 

%% 
subplot(512)
plot(lag/fs,rgcc2,'.');
[~,Q] = max(abs(rgcc2));
lagDiff2 = lag(Q);
timeDiff2 = lagDiff2/fs;
disp(timeDiff2);
% 在图上标注出最大值的位置
hold on;
plot(timeDiff2, rgcc2(Q), 'ro');
text(timeDiff2, rgcc2(Q), ['  Max at ', num2str(timeDiff2)]);
hold off;
title ('CC');
%% 

%% 
subplot(513)
plot(lag/fs,rgcc3,'.');
[~,W] = max(abs(rgcc3));
lagDiff3 = lag(W);
timeDiff3 = lagDiff2/fs;
disp(timeDiff3);
% 在图上标注出最大值的位置
hold on;
plot(timeDiff3, rgcc3(W), 'ro');
text(timeDiff3, rgcc3(W), ['  Max at ', num2str(timeDiff3)]);
hold off;
title ('HB加权');

%% 
subplot(514)
plot(lag/fs,rgcc4,'.');
[~,V] = max(abs(rgcc4));
lagDiff4 = lag(V);
timeDiff4 = lagDiff4/fs;
disp(timeDiff4);
% 在图上标注出最大值的位置
hold on;
plot(timeDiff4, rgcc4(V), 'ro');
text(timeDiff4, rgcc4(V), ['  Max at ', num2str(timeDiff4)]);
hold off;
title ('Roth加权');

%% 
subplot(515)
plot(lag/fs,rgcc5,'.');
[~,U] = max(abs(rgcc5));
lagDiff5 = lag(U);
timeDiff5 = lagDiff5/fs;
disp(timeDiff5);
% 在图上标注出最大值的位置
hold on;
plot(timeDiff5, rgcc5(U), 'ro');
text(timeDiff5, rgcc5(U), ['  Max at ', num2str(timeDiff5)]);
hold off;
title ('SCOT加权');