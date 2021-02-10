# Line-tracking-robot

Initial design goals include required the robot to manually navigate to the starting point of the line through Bluetooth interaction while avoiding obstacles on the path. The robot was then tasked with navigating the line through an automated line-tracking system and find the object, informing the operator that the object has been located as it does so through Bluetooth communication. Finally, the robot was required to return to the starting position using its memory of getting to the ball initially.

## Line Tracking Flow Chart

![Line_Tracking_flowchart](https://user-images.githubusercontent.com/78772945/107479752-aaa0cf80-6bcf-11eb-91c7-8e74d0781df9.png)

## BlueTooth Flow Chart

![Bluetooth_control](https://user-images.githubusercontent.com/78772945/107480309-77ab0b80-6bd0-11eb-97fa-df7d89721882.png)

## Slave Pseudocode


	Connect the slave board to the master board using bluetooth;
	Include <Servo.h>
	Servo servorRight;
	Servo servoLeft,

	Initialise if_returing to be false;
	Initialise if_line_tracking to be true;

	Void setup() 
	
	{
	
	Setup serial;
	Setup INPUT and OUTPUT pin;

	Check whether Master and Slave are already connected by polling the ConnStatus pin (This prevents running the full connection setup routine if not 		necessary.)

	Setup the local (slave) Bluetooth module

	Wait one second 
	Flush the serial buffers

	Attach left signal to pin 13
	Attach right signal to pin 13

	Setup INPUT and OUTPUT pin
	
	} 

	Void loop()
	
	{
		while(1)
		
		{

### Receiving command from master board

		Check if there’s any data sent from master board

		If the command is “w” {
		Move forward
		Store “s” in a reversed command array (everytime receiving a command from the master board, store its reversed command in a reversed 				command array for future use)
		Wait for 0.1 second
		}

		If the command is “q” {
		Make a slight left turn 
		Store “e” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “a” {
		Make a left turn 
		Store “d” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “e” {
		Make a slight right turn 
		Store “q” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “d” {
		Make a right turn 
		Store “a” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “s” {
		Move backwards
		Store “w” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “z” {
		Move left backwards
		Store “x” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “x” {
		Move right backwards
		Store “z” in a reversed command array 
		Wait for 0.1 second
		}

		If the command is “n” {
		Stop
		Store “n” in a reversed command array  (Do not reverse “stop” command)
		Wait for 0.1 second
		}

		If the command is “b” {
		Set the boolean for line tracking to be true, start line tracking
		}

		If the command is “t” {
		Turn around (when the robot has found the table tennis ball)
		Wait for 0.1 second
		}

		If the command is “r” {
		Set the boolean for if returning to be true. 
		The robot begins to return.
		}
	
		Check if there's any data sent from the local serial terminal
		
### Line tracking 
			
			Initial irLeft to be irDetect(9,10,38000);
			Initial irLeft to be irDetect(2,3,38000);
			
			Attach left servo to pin 13;
			Attach right servo to pin 12;
			
			//while if_line_tracking is True start line tracking
			
			While (if_line_tracking is true)
				{
				If (irLeft is 1 and irRight is 1)
					{
					moving forward				
					}

				Else If (irLeft is 1 and ir Right is 0)
				{
				  turning left
				}

				Else if (irLeft is 0 and irRight is 1)
				{
				  turning right
				}

				Else if (irLeft is 0 and irRight is 0)
					{
					  stop
					}
				}

			}
	}



