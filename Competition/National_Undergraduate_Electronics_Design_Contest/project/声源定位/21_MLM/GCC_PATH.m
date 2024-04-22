clc
clear 
close all

%加载一段声音（matlab自带敲锣声）
load gong;
%采样频率
Fs = 8192;  
%采样周期
dt=1/Fs;
%music_src为声源
music_src=y;       

%设置两个麦克风坐标
mic_d=1;
mic_x=[-mic_d mic_d];
mic_y=[0 0];
plot(mic_x,mic_y,'x');
axis([-5 5 -5 5])
hold on;
quiver(-5,0,10,0,1,'color','black');
quiver(0,-5,0,10,1,'color','black');

%声源位置
s_x=10;
s_y=10;
plot(s_x,s_y,'o');
quiver(s_x,s_y,-s_x-mic_d,-s_y,1);
quiver(s_x,s_y,-s_x+mic_d,-s_y,1);

%求出距离
dis_s1=sqrt((mic_x(1)-s_x).^2+(mic_y(1)-s_y).^2);
dis_s2=sqrt((mic_x(2)-s_x).^2+(mic_y(2)-s_y).^2);
c=340;
delay=abs((dis_s1-dis_s2)./340);

%设置延时
music_delay = delayseq(music_src,delay,Fs);
figure(2);
% subplot(211);
plot(music_src,'b');
axis([0 length(music_src) -2 2]);
% subplot(212);
hold on;
plot(music_delay,'c');
axis([0 length(music_delay) -2 2]);

%gccphat算法,matlab自带
[tau,R,lag] = gccphat(music_delay,music_src,Fs);
disp(tau);
figure(3);
t=1:length(tau);
plot(lag,real(R(:,1)));

%cc算法
[rcc,lag]=xcorr(music_delay,music_src);
figure(4);
plot(lag/Fs,rcc);
[~,I] = max(abs(rcc));
lagDiff = lag(I);
timeDiff = lagDiff/Fs;
disp(timeDiff);

%gcc+phat算法，根据公式写
RGCC=fft(rcc);
rgcc=ifft(RGCC*1./abs(RGCC));
figure(5);
plot(lag/Fs,rgcc);
[M,I] = max(abs(rgcc));
lagDiff = lag(I);
timeDiff = lagDiff/Fs;
disp(timeDiff);


%计算角度,这里假设为平面波
dis_r=tau*c;
angel=acos(tau*c./(mic_d*2))*180/pi;
if dis_s1<dis_s2
    angel=180-angel;
end
disp(angel);
