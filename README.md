# testing_particle

A Particle project named testing_particle

## Welcome to your project!

Every new Particle project is composed of 3 important elements that you'll see have been created in your project directory for testing_particle.

#### ```/src``` folder:  
This is the source folder that contains the firmware files for your project. It should *not* be renamed. 
Anything that is in this folder when you compile your project will be sent to our compile service and compiled into a firmware binary for the Particle device that you have targeted.

If your application contains multiple files, they should all be included in the `src` folder. If your firmware depends on Particle libraries, those dependencies are specified in the `project.properties` file referenced below.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device. It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++. For more information about using the Particle firmware API to create firmware for your Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/) section of the Particle documentation.

#### ```project.properties``` file:  
This is the file that specifies the name and version number of the libraries that your project depends on. Dependencies are added automatically to your `project.properties` file when you add a library to a project using the `particle library add` command in the CLI or add a library in the Desktop IDE.

## Adding additional files to your project

#### Projects with multiple sources
If you would like add additional files to your application, they should be added to the `/src` folder. All files in the `/src` folder will be sent to the Particle Cloud to produce a compiled binary.

#### Projects with external libraries
If your project includes a library that has not been registered in the Particle libraries system, you should create a new folder named `/lib/<libraryname>/src` under `/<project dir>` and add the `.h` and `.cpp` files for your library there. All contents of the `/lib` folder and subfolders will also be sent to the Cloud for compilation.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target selected and run `particle compile <platform>` in the CLI or click the Compile button in the Desktop IDE. The following files in your project folder will be sent to the compile service:
for this you need data on your laptop as it is an online process of compilation
- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`

## Flashing your project

When the compiling is done, it will generate a firmware.bin file. it will be something like this electron_1.2.1_firmware_1566801741246.bin. This will be the file that will flashed into your particle device. There are two ways to flash the firmware into the board.
- Method 1: To use the Over The Air (OTA) - flash button on the desktop IDE. This will inncur heavy data cost. So, This method is not recommeneded
- Method 2: To use the particle cli to flash the firmware.

#Method 2: 
	-Open commandprompt and cd into the directory in which the firmware.bin file is located. For this, the commandproft command would be something like this 
		```cd "<filepath to the working project directory>" ```
	-Connect the particle board to the computer using a microusb to usb cable
	-Once you in the command directory/folder where the firmware.bin file is located. Type the following in the commandprompt.
	
		```particle dfu usb```
	-This will set the device into dfu mode. The LED in the e-series will start to blink Yellow. This is the representation that the E-series in the dfu mode. Only in this mode you can flash your firmware via usb.
	-Once the E-series in dfu mode, it is ready for flashing the firmware
	-In the command prompt, type the following command:
	
		```particle flash --usb <firmware.bin file name>```
	-Replace the firmware.bin file name to the filename of the firmware.bin file that was generated during compilation
	-After flashing the LED on the E-Series should turn glow in cyan color (sky bluish color)
	