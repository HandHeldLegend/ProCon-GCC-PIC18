**What is this?**

This is version 2 firmware designed for PIC18LF26K83 microcontrollers which emulates the function of a GameCube controller as faithfully as possible. Other PIC18 microcontrollers are compatible, though you will likely need to utilize the migration tool in MPLAB code configurator.

**Features:**

- GUI-less configuration
- Stick calibration to real GCC range (-100 to +100)
- 4 different trigger modes to handle analog and digital presses in a variety of manners
- 1 bonus input mode specifically designed for Smash Ultimate
- Toggle between Xbox or Nintendo button layouts.
- Rumble disable/enable
- Software adjustable snapback and deadzone

**Background**

I had originally set out to utilize a GC+2.0 board from Aurelio and 4layertech for a custom Pro Controller build. The build worked well enough utilizing a custom PCB but there were a few items I wanted to resolve (namely support for a much faster polling rate and stick range) and changes I wanted to make to support configuring the controller without any external tools. I don't often boot up my Wii and wanted to be able to adjust my controller on the fly.

The schematic and pin layout is incompatible with the original GC+2.0 board as of board revision 1b of my PCB design.
Schematics and boardview are TBA.

This project 100% would not exist without the work of Aurelio, it inspired me to push this project as far as I could go and meet my project goals. There was a ton that was learned from his codebase; I do take a slightly different approach with much of my firmware.

**GUI-less configuration:**

**While booting...**

- Hold ZL/L : Leftmost trigger mode
- Hold ZR/R : Rightmost trigger mode
- Hold L/R : Default trigger mode
- Hold ZL/ZR : Full trigger mode
- Hold - (minus) : Stick calibration mode
- Hold + and - : Reset to defaults
- Hold + (plus) : Enable/disable rumble
- Hold B : Enable Smash Ultimate mode
- Hold Y : Enable/Disable Xbox button layout

**Software Snapback**

Built in to firmware version 2.0 is an adjustable snapback reduction algorithm.
This is configurable for the left stick only on both the X and Y axis.

There are 16 levels of adjustment, the default being completely turned off.

To adjust the X axis, hold Minus and press L or R trigger. R increases the reduction intensity, L decreases.
In other words, if you're experiencing snapback, you need to hold Minus and press R. Adjust one value at a time
and test to ensure you're getting the expected results.

To adjust the Y axis, hold Minus and press ZL or ZR. Same details apply as the X axis.

**Software Deadzone**

You can adjust the stick deadzone on the fly by holding Minus and pressing Dpad Up or Dpad Down. This will increase
or decrease the deadzone respectively. The default value is 4, with 16 values of adjustment. I recommend reducing this 
setting as much as you can without the stick becoming jittery.

**Trigger mode explanations:**

- Leftmost trigger mode: ZL is full L trigger press, L is light trigger press.
- Rightmost trigger mode: ZR is full R trigger press, R is light trigger press.
- Default trigger mode: ZL/ZR act as full trigger press, L/R are mapped as 'Z'
- Full trigger mode: ZL/ZR act as full trigger press, L/R act as light analog press

**Smash Ultimate Mode:**

- L is mapped to left dpad so it can be re-bound to something else in Smash Ultimate from R being mapped to Z
- ZL/ZR are mapped as shield presses. It performs both the digital/analog press simultaneously

**Stick calibration mode:**

In stick calibration mode, both sticks can be swirled around 5-6 times. Press + (plus) to save the configuration. Controller will connect immediately after saving the settings.
