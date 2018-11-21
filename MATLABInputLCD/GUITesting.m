fileName = 'ResultScreen33.jpg';
file = imread(fileName);
screenSize = get(0, 'ScreenSize');
imshow(file, 'InitialMagnification', screenSize(3) / 960 * 100);
% Enlarge figure to full screen.
set(gcf, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]);
% Get rid of tool bar and pulldown menus that are along top of figure.
set(gcf, 'Toolbar', 'none', 'Menu', 'none');