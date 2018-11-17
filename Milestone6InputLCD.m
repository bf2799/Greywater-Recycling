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
BUTTON_PORTS = ['A3', 'A4', 'A5', 'A2'];

% LCD Ports
LCD_PORTS = ['D8', 'D9', 'D10', 'D11', 'D12', 'D13'];

% Port for the LCD Backlight
LCD_BACKLIGHT_PORT = 6;

% Keep track of the number of greywater inputs and outputs
NUM_INPUTS = 4;
NUM_OUTPUTS = 4;

% Names of the greywater inputs
GREYWATER_INPUTS = ['Fresh Water', 'Laundry', 'Shower', 'Toilet'];

% Voltage of the LCD backlight
LCD_BACKLIGHT_VOLTAGE = 2;

% Create arduino that can 
aBoard = arduino('COM3', 'Uno', 'libraries', 'ExampleLCD/LCDAddon', 'ForceBuild', true);

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
        
        % Print the current greywater input
        printLCD(inputLCD, GREYWATER_INPUTS(inputCounter));
        
        % Pause for 1 second so button can't be hit again
        pause(1);
        
        % Set isButtonHit to false
        % isButtonHit keeps track of whether a button has been hit for that
        % 
        isButtonHit = false;
        
        % Keep checking for button press while one isn't 
        while ~isButtonHit
            
            % For each button
            %   If pressed
            %       Register that a button was pressed
            for button = 1:NUM_OUTPUTS
                if readDigitalPin(aBoard, BUTTON_PORTS(button) == 0)
                    isButtonHit = true;
                end
            end
            
        end     
        
    end
    
end
