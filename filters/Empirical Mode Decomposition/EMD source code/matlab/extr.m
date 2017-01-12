function [indMin, indMax,z] =  extr(signal)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here


%INFO:  Function finds localizations of extrema (maxima and minima) in the
%       given signal.
%IN:    signal - analysed signal
%OUT:   indMin - indexes of minima in the given signal
%       indMax - indexes of maxima in the given signal

len = length(signal);
z=0;
%finding extrema based on first derivative
%crossing-zero points
d = diff(signal);     
n = length(d);
d1 = d(1:n-1);
d2 = d(2:n);
a=0;
indMin = find(d1.*d2<0 & d1<0)+1;
for i = 1:length(d)
    if d(i)==0
        a=a+1;
    end
end   	


indMax = find(d1.*d2<0 & d1>0)+1;

%zero points 
if any(d==0)
  imax = [];
  imin = [];
  dZero = (d==0);
  dZero=dZero';
  dd = diff([0 dZero 0]);   %second derivative
  debs = find(dd == 1);
  fins = find(dd == -1);
  if debs(1) == 1
    if length(debs) > 1
      debs = debs(2:end);
      fins = fins(2:end);
    else
      debs = [];
      fins = [];
    end
  end
  if ~isempty(debs)
    if fins(end) == len
      if length(debs) > 1
        debs = debs(1:(end-1));
        fins = fins(1:(end-1));
      else
        debs = [];
        fins = [];
      end      
    end
  end
  lc = length(debs)
  if lc > 0
    for k = 1:lc
      if d(debs(k)-1) > 0
        if d(fins(k)) < 0
          imax = [imax round((fins(k)+debs(k))/2)];
        end
      else
        if d(fins(k)) > 0
          imin = [imin round((fins(k)+debs(k))/2)];
        end
      end
    end
  end
  imax=imax';
   siz=size(imax)
   siz2=size(indMax)
  if ~isempty(imax)
    indMax = sort([indMax; imax]);
  end
 imin=imin';
  if ~isempty(imin)
    indMin = sort([indMin; imin]);
  end
   
end  
end
% [EOF]



