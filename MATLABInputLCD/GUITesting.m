fileName = 'ResultScreen33.jpg';
file = imread(fileName);
screenSize = get(0, 'ScreenSize');
imshow(file, 'InitialMagnification', screenSize(3) / 960 * 100);