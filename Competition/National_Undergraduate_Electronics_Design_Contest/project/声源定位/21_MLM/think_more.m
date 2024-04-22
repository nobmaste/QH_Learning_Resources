clc
clear 
close all

% 创建模拟数据
fs = 1000;  % 抽样频率
T = 1/fs;  % 抽样周期
L = 1000;  % 信号长度
t = (0:L-1)*T;  % 时间向量

%% 产生Chirp
f0 = 10;  % 起始频率
f1 = 200;  % 终止频率
signal_chirp = chirp(t, f0, 1, f1);  % 生成chirp信号

noise_power = 0.1;  % 噪声功率
noise1 = sqrt(noise_power)*randn(size(t));  % 生成高斯白噪声
noise2 = sqrt(0.2)*randn(size(t));  % 生成高斯白噪声
y = signal_chirp + noise1;  % 添加噪声到chirp信号
y2 = signal_chirp + noise2;
delay = 50;  % 延迟
y_delayed = circshift(y2, delay);  % 创建有延迟的chirp信号
% 绘制原始的、添加噪声的和有延迟的chirp信号
figure;
subplot(3,1,1);
plot(t, signal_chirp);
title('Original Chirp Signal');
subplot(3,1,2);
plot(t, y);
title('Noisy Chirp Signal');
subplot(3,1,3);
plot(t, y_delayed);
title('Delayed Chirp Signal');



%算法
[rcc,lag]=xcorr(y_delayed,y);
T_S1=xcorr(y);
T_S2=xcorr(y_delayed);
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