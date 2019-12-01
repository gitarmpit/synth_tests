# exact number of cycles results in very low noise (-120db) 
# just one extra step and it is -20 -40 db 
# so need to use windowing 

t <- seq(0,1.2,1/1000)
y1 = 100*cos(2*pi*100*t + pi/3) + 10*cos(2*pi*300*t + pi/8)
Y1 <- fft(y1[0:1000]);
a=abs(Y1)
aa=a/length(a)*2
amax=max(aa)
anorm <- 10*log10(aa/amax)
plot(anorm[1:300], type="h")

for (i in 1:length(Y1)) 
{
  if (abs(Y1[i]) <1) 
  {
     # print("zeroing"); 
     Y1[i] = 0;
  }
}

plot(Arg(Y1)[1:500], type="h")


# write(Arg(Y1), sep="\n", file="Y1.Arg")
#write(abs(Y1), sep="\n", file="Y1.abs")
