function test=analyzer()
dof=load('/tmp/dof.calibration');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
magx=dof(20:end,4);
magy=dof(20:end,5);
magz=dof(20:end,6);
gyrx=dof(20:end,7);
gyry=dof(20:end,8);
gyrz=dof(20:end,9);
pres=dof(20:end,10);
temp=dof(20:end,11);
len=length(accx);
t=dof(20:end,12)/1000000;
subplot(3,3,1);
plot(t,accx);
subplot(3,3,2);
plot(t,accy);
subplot(3,3,3);
plot(t,accz);

subplot(3,3,4);
plot(t,gyrx);
subplot(3,3,5);
plot(t,gyry);
subplot(3,3,6);
plot(t,gyrz);


subplot(3,3,7);
plot(t,magx);
subplot(3,3,8);
plot(t,magy);
subplot(3,3,9);
plot(t,magz);
