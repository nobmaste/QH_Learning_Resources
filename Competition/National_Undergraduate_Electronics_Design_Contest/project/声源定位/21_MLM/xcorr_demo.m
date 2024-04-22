N = 16;
  n = 0:N-1;
  a = 0.84;
  b = 0.92;
  xa = a.^n;
  xb = b.^n;
  [r,lags] = xcorr(xa,xb);
  stem(lags,r)