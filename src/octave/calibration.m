function [stdx,stdy,stdz]=calibration
dof=load('/home/hframe/dof.calibration0');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,accx);
%subplot(3,1,2);
%plot(t,accy);
%subplot(3,1,3);
%plot(t,accz);
stdx0=std(accx);
stdy0=std(accy);
stdz0=std(accz);

dof=load('/home/hframe/dof.calibration1');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,accx);
%subplot(3,1,2);
%plot(t,accy);
%subplot(3,1,3);
%plot(t,accz);
stdx1=std(accx);
stdy1=std(accy);
stdz1=std(accz);



dof=load('/home/hframe/dof.calibration2');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,accx);
%subplot(3,1,2);
%plot(t,accy);
%subplot(3,1,3);
%plot(t,accz);
stdx2=std(accx);
stdy2=std(accy);
stdz2=std(accz);



dof=load('/home/hframe/dof.calibration3');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,accx);
%subplot(3,1,2);
%plot(t,accy);
%subplot(3,1,3);
%plot(t,accz);
stdx3=std(accx);
stdy3=std(accy);
stdz3=std(accz);



dof=load('/home/hframe/dof.calibration4');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,accx);
%subplot(3,1,2);
%plot(t,accy);
%subplot(3,1,3);
%plot(t,accz);
stdx4=std(accx);
stdy4=std(accy);
stdz4=std(accz);

stdx=(stdx0+stdx1+stdx2+stdx3+stdx4)/5;
stdy=(stdy0+stdy1+stdy2+stdy3+stdy4)/5;
stdz=(stdz0+stdz1+stdz2+stdz3+stdz4)/5;





endfunction

