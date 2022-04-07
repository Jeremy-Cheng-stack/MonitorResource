#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <utmp.h>

void cores(int start, int amount, int g) //Function to display the number of processors the computer has
                                         //along with cpu utilization (RAM) overall percentage of the computer
                                         //Has optional graphics design implementation if graphics flag is on
{
    printf("---------------------------------------------------\n");
    long number_of_processors = sysconf(_SC_NPROCESSORS_ONLN);
    //Use the unistd.h header to obtain number of processors online
    printf("Number of cores: %ld\n", number_of_processors);

    FILE *stat;
    stat = fopen("/proc/stat", "r"); // Opening file /proc/stat for reading

    long unsigned int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    //Columns definitions for /proc/stat

    fscanf(stat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);
    //Accessing the information of each value from the first line of /proc/stat and storing in individual variables
    fclose(stat);

    unsigned long long int not_idle = (user + nice + system + irq + softirq + steal);
    //Calculation to find cpu use that are not idle
    unsigned long long int idled = (idle + iowait);
    //Calculation to find cpu use that are idle
    unsigned long long int total = idled + not_idle;
    //Calculation to find total cpu use

    double cpu_util = ((double)not_idle / (double)total) * 100;
    //Calculation to find cpu utilization obtaining the ratio among the first line of /proc/stat
    printf("\033[2k"); //Erase current row for new printing
    printf("Total CPU use = %.2f %%\n", cpu_util);
    if (g == 1) //print grpahics for UI here if toggled with 1
    {
        for (int i = 0; i < amount; i++)
        {
            printf("\n"); //Indexing the spot
        }
        printf("       |||");
        for (int j = 0; j < (int)cpu_util; j++)
        {
            printf("|"); //Applying extra "|"" based on the value cpu utilization
        }
        printf("%.2f", cpu_util);
    }

    printf("\033[u"); //Return back to saved cursor postion in memory_usage
}

void sess() // Function to obtain user/session information and print out one time
{
    printf("### Sessions/Users ###\n");
    struct utmp *ut; // Create a struct for utmp

    setutent(); //point to the beginning of the utmp file to ensure we are at the start

    ut = getutent(); // get the intial pointer to point to the first struct of utmp with information

    while (ut != NULL) // While loop to make sure the pointer is still pointing to a struct and exit when NULL pointer
    {
        if (ut->ut_type == USER_PROCESS) //Check to make sure type of record is a user process
        {
            printf("\033[2k");                  // Clears line to put new values in
            printf("%s         ", ut->ut_user); // Prints out the user login name
            printf("\033[2k");
            printf("%s ", ut->ut_line); // Prints out the device name
            printf("\033[2k");
            printf("(%s)\n", ut->ut_host); // Prints out the hostname for remote login
        }

        ut = getutent(); // Move pointer to next utmp struct
    }
    printf("---------------------------------------------------\n");
}

void sessions(int amount, int time, int all, int iteration) // Function used to print out information about the users logged in, device name tty, hostname
{
    if (all == 1 && iteration == 0) // if statement to check if we are on the first iteration of display
    {
        printf("\033[2k"); //Erases the row for header
        sess();
    }
    else if (all == 1) // else if statment to check if all the inforamtion must be displayed
    {
        sess();
        printf("\033[u"); //Put cursor back to the end the most recent memory display
    }
    else // If we are not printing everything (--user flag) then this will run
    {
        printf("### Sessions/Users ###\n");
        for (int j = 0; j < amount; j++) //print the amount of time the update for inforamtion is requested
        {
            printf("\033[s"); //Using ESCape code to save cursor location

            struct utmp *ut; // Create a struct pointer for utmp
            setutent();      //point to the beginning of the utmp file to ensure we are at the start
            ut = getutent(); // get the intial pointer to point to the first struct of utmp with information

            while (ut != NULL) // While loop to make sure the pointer is still pointing to something and exit when NULL pointer
            {
                if (ut->ut_type == USER_PROCESS) //Check to make sure type of record is a user process
                {
                    printf("%s         ", ut->ut_user); // Prints out the user login name
                    printf("%s ", ut->ut_line);         // Prints out the device name
                    printf("(%s)\n", ut->ut_host);      // Prints out the hostname for remote login
                }

                ut = getutent(); // Move pointer to next utmp struct
            }

            printf("---------------------------------------------------\n");
            printf("\033[u"); // Go back to the originally saved cursor location

            sleep(time); //Pause for duration as requested by user
        }
    }
}
void memory_usage(int amount, int time, int g, int all) // function to check memory usage providing physical memory usage and virtual memory usage
{
    long convert = 1024 * 1024 * 1024;                              //Constant to convert from bytes to GB
    struct sysinfo sys_info;                                        //Creating the sysinfo struct through the header sys / sysinfo
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n"); //Print header UI for information of display
    float prev_physmem;
    for (int i = 0; i < amount; i++) // loop through the amount of times memory needs to be displayed
    {
        sysinfo(&sys_info); //accessing information about the system and storing it into the struct

        float phys_memory_total = (float)sys_info.totalram / convert;
        // Total Ram
        float phys_memory_used = phys_memory_total - (float)sys_info.freeram / convert;
        // Ram used = Total Ram - Free Ram
        float virt_memory_total = phys_memory_total + (float)sys_info.totalswap / convert;
        // Total ram + Toal swap memory
        float virt_memory_used = phys_memory_used + (float)sys_info.totalswap / convert - (float)sys_info.freeswap / convert;
        //Ram + swap memory used
        if (g == 0)
        {
            printf(" %.2f GB / %.2f GB  --  %.2f GB / %.2f GB\n", phys_memory_used, phys_memory_total, virt_memory_used, virt_memory_total); //print desired information for memeory
        }
        else // Applying graphics UI to memory section
        {
            if (i == 0) //Check if we are on the inital write through as inital value of prev_physmen doesn't exist
            {
                prev_physmem = virt_memory_used;
            }
            printf(" %.2f GB / %.2f GB  --  %.2f GB / %.2f GB   |", phys_memory_used, phys_memory_total, virt_memory_used, virt_memory_total); //print desired information for memeory

            int diff = ((int)(virt_memory_used * 1000 - prev_physmem * 1000)) / 10; //Finding difference between the previous memory used and current memory used
            float change = (float)diff / 100;

            if (diff >= 1) //Check if the difference between previous mem and current mem is positive
            {
                for (int j = 0; j < diff; j++)
                {
                    printf("#");
                }
                printf("*");
            }
            else if (diff <= -1) //Check if the difference is negative
            {
                diff = diff * (-1);
                for (int j = 0; j < diff; j++)
                {
                    printf(":");
                }
                printf("*");
            }
            if ((int)(diff * 100) < 1 && (int)(diff * 100) > -1) // If the difference is "0"
            {
                if ((int)(diff * 100) < 1)
                {
                    printf("@*");
                    diff = diff * (-1);
                }
                else
                {
                    printf("o*");
                }
            }
            printf(" %.2f (%.2f) \n", change, virt_memory_used);
        } //END of graphics design for memory

        //print format for UI
        printf("\033[s");                    //Save for current position to allow for printing inforamtion of CPU and Memory at same time
        for (int l = 0; l < amount - i; l++) //Moving a number of spaces down to CPU display section
        {
            printf("\n");
        }
        cores(amount, i, g); // Update CPU information

        if (all == 1) //Update User connected inforamtion next if all inforamtion is required to be shown
        {
            printf("\033[H"); //Move cursor to home position and move a constant 9 spaces down to reach user inoformation
            printf("\033[9B");
            sessions(amount, time, all, 1);
        }
        prev_physmem = phys_memory_used;
        if (i + 1 == amount) //Final iteration must set up for finishing the program
        {
            if (g == 1)
            {
                for (int l = 0; l < amount * 2; l++)
                {
                    printf("\n");
                }
            }
            else
            {
                printf("\n");
                printf("\n");
                printf("\n");
                printf("\n");
            }
        }
        else
        {
            sleep(time);
        }
    }
    //Create a delay to desired time
    printf("---------------------------------------------------\n");
}

void system_info() // Function to print out some basic information of the system
{
    printf("### System Information ###\n");
    struct utsname uts_name; //Creating the struct for utsname
    uname(&uts_name);        //Getting the info of system and storing the info into the struct uts_na

    printf(" System Name:  %s\n", uts_name.sysname);
    printf(" Machine Name: %s\n", uts_name.nodename);
    printf(" Version:  %s\n", uts_name.version);
    printf(" Release:  %s\n", uts_name.release);
    printf(" Architecture:  %s\n", uts_name.machine);
    //Printing the information desired from the struct uts_name
    printf("---------------------------------------------------\n");
}

int main(int argc, char **argv)
{
    printf("\033[2J"); //Clears screen
    printf("\033[H");  //Put cursor top left

    int check[4]; //checks for samples, tdelay, and graphic, all
    check[0] = 0; // Check for special samples call
    check[1] = 0; // check for special tdelay call
    check[2] = 0; //Graphics off
    check[3] = 1; //No tags -> check if all the inforamtion must be displayed

    int amount = 10; // samples
    int time = 1;    // tdelay

    for (int i = 1; i < argc; i++) // Obtain information on which flags are used
    {
        int checker = 0;
        char *grab_string;
        char *hold;

        grab_string = strtok(argv[i], "-="); //Splitting string into tokens
        hold = grab_string;                  //Get the flag value of samples or tdelay
        grab_string = strtok(NULL, " ,.-");  //one more time to get int value

        if (strcmp(argv[i], "--system") == 0) //Checking which flags are used
        {
            check[0] = 1;
            check[3] = 0;
        }
        else if (strcmp(argv[i], "--user") == 0)
        {
            check[1] = 1;
            check[3] = 0;
        }
        else if (strcmp(argv[i], "--graphics") == 0 || strcmp(argv[i], "--g") == 0)
        {
            check[2] = 1;
        }
        else if (strcmp(hold, "samples") == 0)
        {
            amount = atoi(grab_string);
        }
        else if (strcmp(hold, "tdelay") == 0)
        {
            time = atoi(grab_string);
        }
        else
        {
            amount = atoi(argv[i]);
            i++;
            time = atoi(argv[i]);
        }
    }
    printf("Number of samples: %d -- every %d sec\n", amount, time);
    printf("---------------------------------------------------\n");

    if (check[0] == 1 && check[1] == 0) // Check if  only --system flag used
    {
        system_info();
        memory_usage(amount, time, check[2], check[3]);
    }
    if (check[1] == 1 && check[0] == 0) //Check if only  --user flag used
    {
        system_info();
        sessions(amount, time, check[3], 0);
    }
    if (check[0] == 0 && check[1] == 0 || check[1] == 1 && check[0] == 1) //Check if all flags used or none
    {
        system_info();
        sessions(amount, time, check[3], 0);
        memory_usage(amount, time, check[2], check[3]);
    }
}