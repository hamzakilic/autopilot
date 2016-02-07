function [stdx,stdy,stdz]=calibrationgyrobias
dof=load('/home/hframe/dof.gyrobias0');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
stdx0=std(gyrox);
stdy0=std(gyroy);
stdz0=std(gyroz);

dof=load('/home/hframe/dof.gyrobias1');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
stdx1=std(gyrox);
stdy1=std(gyroy);
stdz1=std(gyroz);



dof=load('/home/hframe/dof.gyrobias2');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,gyrox);
%subplot(3,1,2);
%plot(t,gyroy);
%subplot(3,1,3);
%plot(t,gyroz);
stdx2=std(gyrox);
stdy2=std(gyroy);
stdz2=std(gyroz);



dof=load('/home/hframe/dof.gyrobias3');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,gyrox);
%subplot(3,1,2);
%plot(t,gyroy);
%subplot(3,1,3);
%plot(t,gyroz);
stdx3=std(gyrox);
stdy3=std(gyroy);
stdz3=std(gyroz);



dof=load('/home/hframe/dof.gyrobias4');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
%subplot(3,1,1);
%plot(t,gyrox);
%subplot(3,1,2);
%plot(t,gyroy);
%subplot(3,1,3);
%plot(t,gyroz);
stdx4=std(gyrox);
stdy4=std(gyroy);
stdz4=std(gyroz);

stdx=(stdx0+stdx1+stdx2+stdx3+stdx4)/5;
stdy=(stdy0+stdy1+stdy2+stdy3+stdy4)/5;
stdz=(stdz0+stdz1+stdz2+stdz3+stdz4)/5;





endfunction

