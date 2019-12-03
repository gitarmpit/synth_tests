y3=scan(file = f, what = double())
Y1 <- fft(y3)
a=abs(Y1)
aa=a/length(a)*2
amax=max(aa)
anorm <- 10*log10(aa/amax)
sqrt(sum(aa[12:50]^2))/aa[11]
which(aa>0.0000000000001)
which(aa>0.000000000001)
which(aa>0.00000000001)