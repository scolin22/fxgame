fxgame
======

This is our supposedly working processor and project skeleton in Eclipse.

Go read the [wiki](https://github.com/scolin22/fxgame/wiki) to inform yourself on our git workflow.

Getting Started
======
1. create a new project, make sure it's called fxgame, that's the new name of the high level vhd file
1. DE2 Cyclone II EP2C35F672C6
1. use the ```nios_system.qsys``` file to generate and add ```fxgame.vhd``` to the project.
1. add the pin assignments
1. add the ```nios_system.qip``` file found in the synthesis folder to the project
1. before you compile the project, read the notes below. do them and finally you can compile.
1. create a new project in eclipse using the ```nios_system.sopcinfo``` file
1. commit only changes to ```.c``` or ```.h``` files in the ```software/<eclipse_project_name>``` folder or ```README.md``` or ```.qsys```
1. or add any other docs you think is important, like HAL header files for api reference, just change to file name to add ```DOCS_``` to the front so we know not to include it in the eclipse project

## Note 1: 
I might've fixed this, but if you can't compile then try this step

When you “generate” a NIOS processor containing the SD Card Interface, there are three files that are not copied to your working directory. Before successfully compiling your design in Quartus II, you need to manually copy the following files:

* Altera_UP_SD_Card_Memory_Block.bsf
* Altera_UP_SD_Card_Memory_Block.cmp
* Altera_UP_SD_Card_Memory_Block.vhd

From the IP directory (in your Quartus II installation): ```C:\altera\12.0sp2\ip\University_Program\memory\altera_up_sd_card_ avalon_interface\hdl\```
To your project directory: ```<QsysProjectFolder>\synthesis\submodules\```

##Note 2: 
There are a few bugs in the SD card HAL functions. These functions are located in the following source code file, which is located in yourinstallation directory: ```C:\altera\12.0sp2\ip\University_Program\Memory\altera_up_sd_card_avalon _interface\HAL\src\altera_up_sd_card_avalon_interface.c```

You should replace this file with the ```altera_up_sd_card_avalon_interface.c``` file rovided on the Vista website.

You only need to do this once (per installation).  If you later use someone else’s computer in your project, you should remember to change it in their installation. 
