# exact number of cycles results in very low noise (-120db) 
# just one extra step and it is -20 -40 db 
# so need to use windowing 


# 
y3=scan(file = f, what = double())

d=read.csv(file = f, header = F, sep = ",")[ ,2]
y2=as.double(d)


t <- seq(0,0.9999,1/10000)
y1 = 100*cos(2*pi*100*t)
[995]  -80.9017 -100.0000  -80.9017  -30.9017   30.9017   80.9017  100.0000


y1 = 100*cos(2*pi*100*t + pi/3) + 10*cos(2*pi*300*t + pi/8)
Y1 <- fft(y3)
a=abs(Y1)
aa=a/length(a)*2
amax=max(aa)
anorm <- 10*log10(aa/amax)
plot(anorm[1:300], type="h")
plot(abs(Y1), type="h")

for (i in 1:length(anorm)) 
{
  if (anorm) <1) 
  {
     # print("zeroing"); 
     Y1[i] = 0;
  }
}

for (i in 1:length(Y1)) 
{
  if (abs(Y1[i]) <1) 
  {
     # print("zeroing"); 
     Y1[i] = 0;
  }
}



plot(Arg(Y1)[1:500], type="h")

sqrt(sum(aa[12:50]^2))/aa[11]

 write(y1, file=f2, ncolumns=1)

# write(Arg(Y1), sep="\n", file="Y1.Arg")
#write(abs(Y1), sep="\n", file="Y1.abs")

# plot(bartlett(n), type = "l", ylim = c(0,1))
