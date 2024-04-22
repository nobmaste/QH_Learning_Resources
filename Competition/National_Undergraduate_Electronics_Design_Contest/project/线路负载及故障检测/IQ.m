t=0:1/1000000:0.001;
% I路信号
I = 0.1*cos(2*pi*5000*t) ;
% 设置噪声参数
noise_amplitude = 0.02; % 噪声振幅
% 生成随机噪声
noise_signal = noise_amplitude * randn(size(t));
% Q路信号
Q_signal = 0.06*cos(2*pi*5000*t+pi/4);
Q = Q_signal + noise_signal;

plot(t,I);
hold on;
plot(t,Q);
title('IQ两路输入信号');
% 进行I路信号相乘操作
I_multiply = I.*Q;
%进行Q路信号相乘操作
Q_multiply = -0.1*sin(2*pi*5000*t).*Q;
% plot(t,I_multiply);
% hold on;
% plot(t,Q_multiply);
% title('IQ两路相乘信号');

Q_out = lowpass(Q_multiply,1000,1000000);
I_out = lowpass(I_multiply,1000,1000000);
% plot(t,I_out);
% hold on;
% plot(t,Q_out);
% title('IQ两路输出信号');

AD_Q = rms(Q_out);
AD_I = rms(I_out);

effect_value= sqrt(AD_Q^2 + AD_I^2);
mag_out=2*effect_value/0.1;

angle = atan(AD_Q/AD_I);
% 将结果从弧度转换为角度
result_deg = rad2deg(angle);
% 显示结果

fprintf('幅度计算结果：%f\n', mag_out);
fprintf('角度计算结果：%f\n', result_deg);

