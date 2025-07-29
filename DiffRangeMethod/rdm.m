clc; clear; close all;

% === Task Parameters ===
c     = 3e8;           % speed of light, m/s
dt12  = 1.47e-6;       % path difference (1–2), s
dt13  = -13.4e-6;      % path difference (1–3), s

% station coordinates (meters)
posts = [5000, 6000;    % station 1
         1000, 1000;    % station 2
         9000, 1000];   % station 3

% distance differences
d12 = c * dt12;
d13 = c * dt13;

% system of equations function
fun = @(xy) [
    sqrt((xy(1)-posts(1,1))^2 + (xy(2)-posts(1,2))^2) - ...
    sqrt((xy(1)-posts(2,1))^2 + (xy(2)-posts(2,2))^2) - d12;
    
    sqrt((xy(1)-posts(1,1))^2 + (xy(2)-posts(1,2))^2) - ...
    sqrt((xy(1)-posts(3,1))^2 + (xy(2)-posts(3,2))^2) - d13
];

% initial guess – center of mass of the stations
x0 = mean(posts(:,1));
y0 = mean(posts(:,2));

% solving with fsolve
opts = optimoptions('fsolve', ...
    'Display','off', ...
    'Algorithm','levenberg-marquardt');
[xy_sol, ~, exitflag] = fsolve(fun, [x0; y0], opts);

if exitflag <= 0
    error('fsolve did not converge. Try a different initial guess.');
end

fprintf('Estimated emitter position: x = %.2f m, y = %.2f m\n', xy_sol(1), xy_sol(2));

% === Visualization ===
figure('Color','w');
hold on; grid on; axis equal;
xlim([0 10000]); ylim([0 7000]);

% – stations
plot(posts(:,1), posts(:,2), 'ro', 'MarkerSize',8, 'LineWidth',1.5);
text(posts(:,1)+150, posts(:,2)+150, {'Station 1','Station 2','Station 3'}, ...
     'FontSize',10, 'Color','r');

% – emitter
plot(xy_sol(1), xy_sol(2), 'b*', 'MarkerSize',12, 'LineWidth',2);
text(xy_sol(1)+150, xy_sol(2)+150, 'Emitter', 'FontSize',11, 'Color','b');

% – field calculations for hyperbolas
[x, y] = meshgrid(linspace(0,10000,600), linspace(0,7000,420));
d1 = hypot(x - posts(1,1), y - posts(1,2));
d2 = hypot(x - posts(2,1), y - posts(2,2));
d3 = hypot(x - posts(3,1), y - posts(3,2));

% – hyperbola contours d1-d2 = d12 and d1-d3 = d13
contour(x, y, d1 - d2, [d12 d12], 'r--', 'LineWidth',1.2);
contour(x, y, d1 - d3, [d13 d13], 'g--', 'LineWidth',1.2);

xlabel('X, m'); 
ylabel('Y, m');
title('Emitter Position Determination by TDOA Method');
legend({'Stations','Emitter','Hyperbola 1-2','Hyperbola 1-3'}, ...
       'Location','bestoutside');
