% x1,y1,z1,t1,x2,y2,z2,t2, k
%  1  2  3  4  5  6  7  8  9
% x(1), ... , x(9)
function F = myfun(x)
x1=x(1);y1=x(2);z1=x(3);t1=x(4);
x2=x(5);y2=x(6);z2=x(7);t2=x(8);
 k=x(9); C=x(10); a= x(11); b=x(12); 
%x3=x(13);y3=x(14);z3=x(15);t3=x(16);c=x(17);
 

F = [-x1+1/6*z1^3+1; -y1-1/2*t1^2; -z1-t1;...
     -x2+1/6*(t2-t1)^3+z1/2*(t2-t1)^2+y1*(t2-t1)+x1;...
     -y2+1/2*(t2-t1)^2+z1*(t2-t1)+y1; -z2+t2-t1+z1;...
     %x2^2-((a^2)^3*x1)^2; y2^2-((a^2)^2*y1)^2;z2^2-(a^2*z1)^2;...
     x2+((a^2)^3*x1); y2-((a^2)^2*y1);z2+(a^2*z1);...
     k^8-7*k^7-2*k^6+8*k^5+17*k^4+8*k^3-2*k^2-7*k+1;...
     (y1+1/2*z1^2); x1/y1/z1-C; a-k
    
];
end
