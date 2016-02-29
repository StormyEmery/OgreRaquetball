CS354R 
Group 10: Sihai Tang, Rafik Rizik, Stormy Emery 
EID: st24899, ror279, sre478 

Execution:
To make a clean build: buildit
To compile: make
To run executable: ./assignment2


Controls:
	Esc			Pause and bring up Menu
    F12         Exit Game
    Q        	Play impact Sound
    Reset Ball  Button Resets Ball, Re-renders the Screen with the ball reset
    Exit        Exits Game
    W,A,S,D    	Default Camera Controls
    L-Shift     Faster Camera Movement
    Mouse       Controls Camera When Not on Menu Option


Progress:
	
	We have physics and sound integrated.

	We have made the playing field, a room made up of 6 different planes. Within this room, we have a ball that starts out with a random force applied to it. As of right now, the ball is affected by gravity and will eventually slow to a stop. 

	We have a background track playing and whenever you tap on 'Q', you can hear the sound effect that we will be using for when the ball collides with an object. 

	We also have also implemented a pause mechanic which also features our basic menu and can be accessed by pressing 'Ecp'. Our menu has two buttons on it. One is a reset ball button which just resets the ball back to where it started at and applies a new random force to it. The other button is an exit button which exits the game, just like pressing 'F12' would do. Pressing 'Ecs' again will resume the game.

	The scoring mechanic isn't implemented yet, but we do have a score board in the top left corner of the screen. It has some extra debugging info on it for now, but the score is incremented by pressing the reset ball button in the menu. Again, this is just temporary and is just for debugging purposes.

	We still need to implement our player (which will be a paddle) and the way it interacts with the ball. I feel we are on track to finish by next Friday.  