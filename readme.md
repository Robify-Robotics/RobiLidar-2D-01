# USB-LIDAR DESCRIPTION#
[**RobiLidar-2D-01**](https://github.com/Robify-Robotics/RobiLidar-2D-01)

#HOW TO BUILD AND USE#

## LINUX ##
Prerequisite: g++ and gcc must be installed

	cmake CMakeList.txt
	make
	make install
	./demo config/xxxx.txt  

## WINDOWS ##
windows powershell

    use cmake_gui  build ,open project and compile
    need mfc support
	Detailed operation reference  [SDK Development Manual](docs/SDK_Development_Manual.pdf)
open project by  visual stdio 2013  or greater than this version 
Generate a solution

**Special instructions: Please refer to the corresponding product's documentation to select the specified configuration file, and ensure that the software can be called normally only after the hardware works normally.**

## config file from windows to linux ##
	vim xxxx.txt
	set ff=unix





