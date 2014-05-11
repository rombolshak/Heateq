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
tau = task(1,5) / 500
h = task(1,6)
gmma = tau / (h * h);
iterations = 1000;
steps = (maxx - minx) / h;

x = f(:, 1);
y = f(:, 2);
u = u(:, 2);

gr = zeros(steps, iterations + 1);
gr(:, 1) = x + 0i;
gr(:, 2) = y + 0i;

a = gmma * (0 + 1i);
c = gmma * (0 + 1i);

for k=3:iterations
  for l=1:steps
     b = 1 + (-2 * gmma - tau * u(l)) * (0 + 1i);
     if (l == 1)
       gr(l, k) = b * gr(l, k - 1) + c * gr(l + 1, k - 1);
     elseif (l == steps)
       gr(l, k) = a * gr(l - 1, k - 1) + b * gr(l, k - 1);
     else
       gr(l, k) = a * gr(l - 1, k - 1) + b * gr(l, k - 1) + c * gr(l + 1, k - 1);
     endif
#    gr(l, k) = y(l) * exp(-0.585125 * tau * k * (0 + 1i));
  endfor  
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