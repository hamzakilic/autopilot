function [minx,maxx,miny,maxy,minz,maxz]=calibration2
dof=load('/home/hframe/dof.calibration');
accx=dof(20:end,1);
accy=dof(20:end,2);
accz=dof(20:end,3);
len=length(accx);
t=[0:len-1];
minx=min(accx);
maxx=max(accx);
miny=min(accy);
maxy=max(accy);
minz=min(accz);
maxz=max(accz);



endfunction

