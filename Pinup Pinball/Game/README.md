# Pinup-Pinball
Tribute game of [Pinup Pinball](http://www.pin-ball.org/file.php?f=1248) developed by [Carles Homs](https://github.com/ch0m5) and [Dídac Romero](https://github.com/didacromero) using the SDL and Box 2D libraries.

## Installation
No previous installations is required to play the game, just download and execute the .exe file *Pinup Pinball* from releases.

## Usage
### General Controls
- ESC: Quit the game
- Spacebar: Restart Game

### Player Controls
- Keyboard:
	- Flippers: LEFT ARROW & RIGHT ARROW
	- Kicker: DOWN ARROW

### Map Features
- Turning all top middle lights adds +1 to your score multiplier.
- Turning on all top left lights will unlock ramp 3, if ramp 3 is unlocked it adds +1 to your score multiplier.
- Going through ramp 3 deactivates, but rewards you with A LOT of points!
- Turning on both side lights start the "Ramps Event".
- Making your ball go across ramp 1 and 2 during "Ramps Event" gives an extra ball to use!
- Lights next to bumpers will only turn after getting out of a ramp. Turning one of them OFF recovers all wasted bumpers and gives score for each of them.
- Tunnel with catch your ball and launch it from the other end 2 seconds later.
- Small grey "pegs" save your ball from falling! They are wasted however, but can be recovered by turning off a down light or losing a ball.

### Differences From The Original
- No multiball, different rewards instead.
- Peg recovery is for all pegs, does not depend on light's side.
- No score shown on place of impact.
- No bumper animations.
- Alternative, original fonts static on top instead of game fonts.
- Static camera instead of moving up and down.
- Slight audio variations (same sounds for different events and ball collisions always produces sound).

### Debug Mode
- F1: Enable/Disable Debug Mode
- 1: Create Ball on mouse
- Mouse Click on Ball: Drag ball

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-newfeature`
3. Commit your changes: `git commit -am 'Add some
feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request and we might look into it. :D

## The Team: [DocDonkeys](https://github.com/DocDonkeys)
- Carles Homs 
  - Role: Code
  - Github: [ch0m5](https://github.com/ch0m5)

- Dídac Romero
  - Role: Code
  - Github: [DidacRomero](https://github.com/DidacRomero)

- Repository: https://github.com/DocDonkeys/Pinup-Pinball

![Team Photo](https://raw.githubusercontent.com/DocDonkeys/Pinup-Pinball/master/Wiki%20Material/Logo_%26_Team/Team_Photo.jpeg)
*Dídac Romero & Carles Homs*
 ---
## License & Copyright 

© DocDonkeys

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.