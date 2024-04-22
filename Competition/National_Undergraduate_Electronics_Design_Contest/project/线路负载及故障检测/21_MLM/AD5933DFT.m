t = 0:1/100000:1-1/100000;                     % Time vector
x = 0.1*sin(2*pi*10000*t)+2.5 ;      % Signal

y = fft(x);                               % Compute DFT of x
m = abs(y);                               % Magnitude
y(m<1e-6) = 0;
p = unwrap(angle(y));                     % Phase
f = (0:length(y)-1)*100/length(y);        % Frequency vector

subplot(2,1,1)
plot(f,m)
title('Magnitude')
ax = gca;
ax.XTick = [15 40 60 85];

subplot(2,1,2)
plot(f,p*180/pi)
title('Phase')
ax = gca;
ax.XTick = [15 40 60 85];