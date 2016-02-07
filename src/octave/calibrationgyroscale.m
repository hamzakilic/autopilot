function [minx,maxx,miny,maxy,minz,maxz]=calibrationgyroscale
dof=load('/home/hframe/dof.gyroscale');
gyrox=dof(20:end,7);
gyroy=dof(20:end,8);
gyroz=dof(20:end,9);
len=length(gyrox);
t=[0:len-1];
minx=min(gyrox);
maxx=max(gyrox);
miny=min(gyroy);
maxy=max(gyroy);
minz=min(gyroz);
maxz=max(gyroz);



endfunction

