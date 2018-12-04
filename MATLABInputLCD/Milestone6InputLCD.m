%Clear console and variables
clear;
clc;

% Make LCD available to rest of Matlab program
% CHANGE PATH DEPEN
addpath('C:\Users\Ben\Documents\MyDocs\Northeastern\Cornerstone\Greywater Recycling\GreywaterRecyclingCode');

% Greywater Input number look-up
% Instead of making each input to an object, each input is given a number to make the code loops easier
%    0 = Fresh Water
%    1 = Laundry
%    2 = Shower
%    3 = Toilet
%    4 = Dishwasher
%    5 = Bathroom Sink

% Button input ports
% Buttons:       A     B     C     D
BUTTON_PORTS = ["A3", "A0", "A2", "A1"];

% LCD Ports
LCD_PORTS = {'D8', 'D9', 'D10', 'D11', 'D12', 'D13'};

% Keep track of the number of greywater inputs and outputs
NUM_INPUTS = 6;
NUM_OUTPUTS = 4;

% Names of the greywater inputs
GREYWATER_INPUTS = {'Fresh Water', 'Laundry', 'Shower', 'Toilet', 'Dishwasher', 'Kitchen Sink'};

% Number of LEDs in a strip (maximum)
NUM_LEDS = 27;

% The number of ms of flow delay
FLOW_DELAY_MS = 50;

% Image with in pixels
IMAGE_WIDTH_PX = 960;

% Time to delay to look at computer screen
SCREEN_DELAY_SEC = 5;

% Time the start button needs to be held in order to trigger the game
% starting
START_BUTTON_HOLD_TIME_MS = 1000;

% Time to hold A and B to reset game
RESET_HOLD_TIME_MS = 2000;

% Time it takes LEDs to flash 3 times
LED_FLASH_TIME_SEC = 1.5;

% Create arduino that can 
aBoard = arduino('COM3', 'Uno', 'libraries', 'ExampleLCD/LCDAddon', 'ForceBuild', true);

disp("Successful Build");

% Initialize input LCD with LCD ports
lcdInput = addon(aBoard, 'ExampleLCD/LCDAddon', LCD_PORTS);
initializeLCD(lcdInput);
clearLCD(lcdInput);

% Until the game is exited
while true
    
    % Create file of play again screen
    file = imread('PlayAgain.jpg');

    % Get screen size of computer
    screenSize = get(0, 'ScreenSize');

    % Make image about full screen
    % screenSize(3) gets width of screen
    % * 100 puts magnification in percent instead of decimals
    imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_WIDTH_PX * 100);
    
    % Initialized whether the game has started as false
    isStarted = false;
    
    % Keep running code while button A hasn't been held to start the game
    while ~isStarted
        
        % Wait while button A isn't pressed
        while readDigitalPin(aBoard, BUTTON_PORTS(1)) == 1
            %Do nothing
        end
        
        % Wait for time button should be held down to start
        pause(START_BUTTON_HOLD_TIME_MS / 1000);
        
        % If button A is still pressed after delay
        %   Set that the game has started to true to exit the loop
        if readDigitalPin(aBoard, BUTTON_PORTS(1)) == 0
            isStarted = true;
        end
        
    end
    
    % Create file of "Started" screen
    file = imread('Started.jpg');

    % Get screen size of computer
    screenSize = get(0, 'ScreenSize');

    % Make image about full screen
    % screenSize(3) gets width of screen
    % * 100 puts magnification in percent instead of decimals
    imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_WIDTH_PX * 100);
    
    % Pause to give user time to release button
    pause(2 * START_BUTTON_HOLD_TIME_MS / 1000);
    
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
        
        % Create file of greywater recycling graphic
        file = imread('GreywaterRecycling.jpg');

        % Get screen size of computer
        screenSize = get(0, 'ScreenSize');

        % Make image about full screen
        % screenSize(3) gets width of screen
        % * 100 puts magnification in percent instead of decimals
        imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_WIDTH_PX * 100);

        % Pause for  n7 m50 ms to allow user time to press both A and B buttons
        pause(0.05);
        
        % Start internal timer
        tic;
        
        % Wait while buttons A and B are still pressed
        while readDigitalPin(aBoard, BUTTON_PORTS(1)) == 0 && readDigitalPin(aBoard, BUTTON_PORTS(2)) == 0 && toc <= RESET_HOLD_TIME_MS / 1000
            % Do nothing. Wait.
        end
        
        % If more than the reset time has passed
        %   Exit out of the input loop to end the game
        if toc > RESET_HOLD_TIME_MS / 1000
            clearLCD(lcdInput);
            break;
        end
        
        % Pause for flow of LED time so button can't be hit again
        % 1/1000 turns ms into sec
        pause(NUM_LEDS * FLOW_DELAY_MS / 1000 + LED_FLASH_TIME_SEC);
        
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
        imshow(file, 'InitialMagnification', screenSize(3) / IMAGE_WIDTH_PX * 100);
                
        % Turn off top LCD screen so user attention goes elsewhere
        % (preferably computer screen
        clearLCD(lcdInput);
        
        % Pause the program for some time so user looks at computer screen
        pause(SCREEN_DELAY_SEC);
        
    end
    
end
