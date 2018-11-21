%Clear console and variables
clear;
clc;

% Make LCD available to rest of Matlab program
% CHANGE PATH DEPEN
addpath('C:\Users\Ben\Documents\MyDocs\Cornerstone\Greywater Recycling\GreywaterRecyclingCode');

% Greywater Input number look-up
% Instead of making each input to an object, each input is given a number to make the code loops easier
%    0 = Fresh Water
%    1 = Laundry
%    2 = Shower
%    3 = Toilet

% Button input ports
% Buttons:       A     B     C     D
BUTTON_PORTS = ["A3", "A4", "A5", "A2"];

% LCD Ports
LCD_PORTS = {'D8', 'D9', 'D10', 'D11', 'D12', 'D13'};

% Port for the LCD Backlight
LCD_BACKLIGHT_PORT = "D6";

% Keep track of the number of greywater inputs and outputs
NUM_INPUTS = 8;
NUM_OUTPUTS = 4;

% Names of the greywater inputs
GREYWATER_INPUTS = {'Fresh Water', 'Laundry', 'Shower', 'Toilet', 'Dishwasher', 'Bathroom Sink', 'Kitchen Sink', 'Gutter'};

% Voltage of the LCD backlight
LCD_BACKLIGHT_VOLTAGE = 2;

% Number of LEDs in a strip (maximum)
NUM_LEDS = 29;

% The number of ms of flow delay
FLOW_DELAY_MS = 50;

% Image with in pixels
IMAGE_WIDTH_PX = 960;

% Time to delay to look at computer screen
SCREEN_DELAY_SEC = 5;

% Create arduino that can 
aBoard = arduino('COM3', 'Uno', 'libraries', 'ExampleLCD/LCDAddon', 'ForceBuild', true);

disp("Successful Build");

% Initialize input LCD with LCD ports
lcdInput = addon(aBoard, 'ExampleLCD/LCDAddon', LCD_PORTS);
initializeLCD(lcdInput);
clearLCD(lcdInput);

% Set the backlight of the LCD
writePWMVoltage(aBoard, LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_VOLTAGE);

% Until the game is exited
while true
    
    % Loop through greywater inputs each time the game runs
    for inputCounter = 1:NUM_INPUTS
        
        % Clear the LCD screen of previous output
        clearLCD(lcdInput);
        
        % Print 
        printLCD(lcdInput, GREYWATER_INPUTS{inputCounter});
        
        % Set isButtonHit to false
        % isButtonHit keeps track of whether a button has been hit for that
        % 
        isButtonHit = false;
        
        % Initialize which button is pressed
        buttonPressed = 0;
        
        % Keep checking for button press while one isn't 
        while ~isButtonHit
            
            % For each button
            %   If pressed
            %       Register that a button was pressed
            for button = 1:NUM_OUTPUTS
                if readDigitalPin(aBoard, BUTTON_PORTS(button)) == 0
                    isButtonHit = true;
                    buttonPressed = button;
                end
            end
            
        end
        
        % Pause for flow of LED time so button can't be hit again
        % 1/1000 turns ms into sec
        pause(NUM_LEDS * FLOW_DELAY_MS / 1000);
        
        % Create file name defined as:
        % ResultScreen followed by input number and button pressed number
        fileName = strcat('ResultScreen', num2str(inputCounter), num2str(buttonPressed),'.jpg');
        
        % Create file from file name
        file = imread(fileName);
        
        % Get screen size of computer
        screenSize = get(0, 'ScreenSize');
        
        % Make image about full screen
        % screenSize(3) gets width of screen
        % * 100 puts magnification in percent instead of decimals
        imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_SIZE_PX * 100);
        
        % Enlarge figure to full screen.
        set(gcf, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]);
        % Get rid of tool bar and pulldown menus that are along top of figure.
        set(gcf, 'Toolbar', 'none', 'Menu', 'none');
        
        % Turn off top LCD screen so user attention goes elsewhere
        % (preferably computer screen
        clearLCD(lcdInput);
        
        % Pause the program for some time so user looks at computer screen
        pause(SCREEN_DELAY_SEC);
        
    end

    % Create file of play again screen
    file = imread('PlayAgain.jpg');

    % Get screen size of computer
    screenSize = get(0, 'ScreenSize');

    % Make image about full screen
    % screenSize(3) gets width of screen
    % * 100 puts magnification in percent instead of decimals
    imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_SIZE_PX * 100);

    % Enlarge figure to full screen.
    set(gcf, 'Units', 'Normalized', 'OuterPosition', [0 0 1 1]);
    % Get rid of tool bar and pulldown menus that are along top of figure.
    set(gcf, 'Toolbar', 'none', 'Menu', 'none');
    
    % Wait while button A isn't pressed
    while readDigitalPin(aBoard, BUTTON_PORTS(1)) == 1
        %Do nothing
    end
    
end
