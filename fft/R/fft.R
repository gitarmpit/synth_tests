t <- seq(0,0.999,0.0001)
y1 = cos(2*pi*124*t + pi/3)
Y1 <- fft(y1);
write(Arg(Y1), sep="\n", file="Y1.Arg")
write(abs(Y1), sep="\n", file="Y1.abs")
