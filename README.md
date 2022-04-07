# Program-to-monitor-resources


Overview of the program:

The program displays some important information about the system using command line. Specifically, the program first displays some basic information such as system name, machine name, linux version, release, architecture of the computer. After, the program displays the users that are connected to the machine, along with device name tty and the hostname. Furthermore , the program displays
Information about the memory usage(RAM) in GB. The program also displays CPU utilization and how many processors the computer has. Memory information and core information can enhanced with graphic UI to help better understand the data presented.

Functions documentation:

	✓	Void cores(int start, int amount, int g)

The purpose of this function is for displaying core and CPU utilization information. There is optional graphics implementation that can be performed if the tag graphics has been used.

The function sysconf(_SC_NPROCESSORS_ONLN) is used to find number of cores.


CPU utilization is calculated with the formula.:

CPU util = (user +nice +system +irq + softirq +steal) / (user +nice +system+ irq + softirq +steal + idle + iowait)

	This function takes in the parameter start, amount and g. The integer “start” indicates the value or amount of samples that needs to be taken indicated originally by the user. The integer “amount” is the amount of steps or iteration of displaying information, relating mostly to the graphics part. It is mostly for indexing how many spaces the cursor should move down. The integer “g” is the graphics checker, if it is 1 then graphics is on and if it is 0 graphics is off.
 
	✓	Void sess()

The purpose of this function is to obtain information about the users that needs to be documented. It is one time and access the utmp struct to do so.

	✓	Void sessions(int amount, int time, int all, int iteration)
The purpose of the function is to discover if the —user flag is used and to give out the tasks. If —user flag is used the function will loop all N times on its own. If the —user flag is used, then the faction relies on the loop that is in system_info().

	int amount -> the amount of time samples needs to be taken
	int time -> how long between each sample
	int all -> if the —user flag was used or not
	int iteration -> which iteration the system_info() loop is on since there is a special case for first run through (need to erase header)

	✓	Void memory_usage(int amount, int time, int g, int all)
The purpose of the function is to find how much physical memory is being used and how many virtual memory is being used out of the total. The physical memory is just calculated with total Ram and virtual memory is calculated with Ram plus swap memory. Note that during conversion from bytes to GB, (1024*1024*1024) was used in conversion.

	int amount -> the amount of time samples needs to be taken
	int time -> how long between each sample
	int all -> if the —user flag was used or not
	int g -> if the graphics tag has been used or not since it changes how the information is being displayed 

	✓	Void system_info()
The purpose of the function is to display some basic information of the machine beforehand that is important to every information being displayed. Displayed information includes system name, machine name, linux version, release, architecture of the computer.

	✓	Int main(int argc, char **argv)
The main function is where the inputed command line is inspected for flag uses and notify the other functions on how to perform with which flags used. 

How to compile and run:

Make sure to be in file where the program is in first.
In linux terminal, type in “gcc A1.c” first to create an output file called ./a.out

Next, type “./a.out” if you would like to run the program with no additional/specific instructions. Otherwise, you can perform 
specific requests with flags like “./a.out —g 2 3” or “./a.out —system 10 1”. 

Try to have the terminal screen size big enough to fit all the information in without the need of scrolling.
 
Note that the extra flags that can be used are : 
	1.	—system  		(Used to print out Memory information and CPU information only)
	2.	—user			(Used to only print our the Sessions/Users information)
	3.	—graphics / —g	(Used to add graphics onto the informations for memory and cpu)
	4.	—samples=N		(Used to declare N samples to be taken)
	5.	—tdelay=T		(Used to declare the interval of samples to be returned with time T)
	6.	N T				(Used to declare N samples with T time interval) !must be declared in that order!
									


How I solve the problems:
	⁃	Problem: Dealing with escape codes and managing to put multiple things at same time. I decided to not clear the whole screen first and re apply the informations that are stored in struct. The method I used is to index with the cursor to the position that needs to be changed/updated and then move to next location and so on to create the illusion that everything is updating at the exact same time. This involved using ESCape codes that store cursor location, “\n” and ESCape codes to move cursor up and down. (Felt like I was doing CSS)
	⁃	Problem: Obtain information about everything that needs to be collected. This involved reading through all the documentation that was put on Quercus first. After going through each of them, the next step was to organize the information that needs to be displayed in a set of notes so I can keep track which header is required for the functions I need and how to use these functions. After that it was just about putting what I need in the specific places and printing them for the user to see.
	⁃	Problem: Calculating CPU and memory. CPU took me the longest time to research for how the values in /proc/stat can be used to calculate it. In the end I decided to go with what the prof said and to find the ratio between idle and not idle and calculate a percentage from that. That means values that calculates how much time the computer is doing nothing vs the times when the computer is performing something. For memory, I was able to find the information easily, but then it was about converting bytes into GB. At first I used conversion of a number to the power of 10, but then it turned out to be a number a lot higher. After some research, the conversion was a number to the power of 2 (1024*1024*1024).
	⁃	Problem: Finding which flag was used with the command line. This involved understanding argc and argv to be able to do some string manipulation to find all the information I need for that.
	⁃	Problem: Graphics. Graphics was something I dealt with last since it was just an add on to the code I had before. Graphics involved me to analyze how the graphics should be shown and how to put it there with all my other information.

Known Issues:
	•	If the size of the terminal screen is too small for all the information to be presented then there might be over writing due to wrong positioning of ESCape codes in the small screen. (Make sure to expand the window to biggest size possible to display information as correct as possible)
