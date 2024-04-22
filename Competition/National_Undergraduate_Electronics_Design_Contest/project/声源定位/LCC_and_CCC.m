% 生成模拟数据
Fs = 100;  % 抽样频率
T = 1/Fs;  % 抽样周期
L = 1000;  % 信号长度
t = (0:L-1)*T;  % 时间向量

% 生成两个含有相同频率成分的信号
S1 = sin(2*pi*10*t);  % 第一个信号
S2 = [zeros(1,100) S1(1:end-100)];  % 第二个信号，是第一个信号的延迟版本

% 计算线性互相关
[R_linear, lag_linear] = xcorr(S1, S2, 'unbiased'); 

% 计算循环互相关
R_cyclic = ifft(fft(S1) .* conj(fft(S2)));

% 绘制图形
figure
subplot(2,1,1)
plot(lag_linear, R_linear)
title('Linear Cross-Correlation of Signals S1 and S2')
xlabel('Lag')
ylabel('Correlation')

subplot(2,1,2)
plot(abs(R_cyclic))
title('Cyclic Cross-Correlation of Signals S1 and S2')
xlabel('Lag')
ylabel('Correlation')
