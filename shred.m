#!/usr/bin/octave -qf
filename = "result.hof";
arg_list = argv();
if (nargin !=  0)
    filename = arg_list{1}
    exit
endif

command = cstrcat("tar xf ",  filename);
system(command);

task = load(".task.dat");
if (task(1:1) != 1)
  printf("Task not time-independent. Wont work. I'm lazy today. Bye!")
  exit
endif

u = load(".u.dat");
f = load(".real.dat");

minx = task(1,2)
maxx = task(1,3)
tau = 1e-4
#task(1,5) / 10
h = task(1,6)
gmma = tau / (h * h);
iterations = 9999;
steps = (maxx - minx) / h;

x = f(:, 1);
y = f(:, 2);
y2 = f(:, 2);
u = u(:, 2);

yrange = max(abs(y));

#gr = zeros(steps, iterations + 1);
#gr(:, 1) = x + 0i;
#gr(:, 2) = y + 0i;

a = gmma * (0 + 1i);
c = gmma * (0 + 1i);

#f = load('.real.dat');
#f = f';
#y = f;
#y2 = f;
#x = linspace(0, 1, 1000);
#u = load('.u.dat');
#u = u';

s = 1/2;
B = -2 + i * h^2 / (tau * s)
#K = spdiags(ones(steps - 1, 1))
K = spdiags([[ones(steps-1, 1);0] B * ones(steps, 1) [ones(steps-1, 1);0]], -1:1, steps, steps) * s;

for k=3:iterations
#  for l=1:steps
#     b = 1 + (-2 * gmma - tau * u(l)) * (0 + 1i);
#     if (l == 1)
#       #gr(l, k) = b * gr(l, k - 1) + c * gr(l + 1, k - 1);
#	 y2(l) = b * y(l) + c * y(l + 1);
#     elseif (l == steps)
#       #gr(l, k) = a * gr(l - 1, k - 1) + b * gr(l, k - 1);
#	 y(l) = a * y(l - 1) + b * y(l);
#     else
#       #gr(l, k) = a * gr(l - 1, k - 1) + b * gr(l, k - 1) + c * gr(l + 1, k - 1);
#	
#     endif
#    gr(l, k) = y(l) * exp(-0.585125 * tau * k * (0 + 1i));
    y2 = y - K\([y(2:end);0] - 2*y + [0; y(1:end-1)] - y.*u*h*h);
#endfor
  #if (rem(k,100) == 0)
    plot(x, [real(y2) imag(y2) abs(y2) u]);    
    axis([0 1 -yrange yrange]);
    pause(0.05);
  #endif
    printf("%d\n", k);
  y = y2;
endfor

ymin = min([min(min(real(gr(:, 2:iterations)))), min(min(imag(gr(:, 2:iterations)))), min(min(abs(gr(:, 2:iterations))))])
ymax = max([max(max(real(gr(:, 2:iterations)))), max(max(imag(gr(:, 2:iterations)))), max(max(abs(gr(:, 2:iterations))))])

re = real(gr);
im = imag(gr);
im(:, 1) = x;
ab = abs(gr);

save -ascii .real.dat re;
save -ascii .imag.dat im;
save -ascii .abs.dat ab;

command = sprintf("./plot.plg result.gif %f %f %f %f", minx, maxx, ymin, ymax)
system(command);
system("gwenview result.gif");
