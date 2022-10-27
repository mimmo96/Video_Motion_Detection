# Video_Motion_Detection


There are two ways to execute the Project:
 
  • Mode 1: execute the script inside the folder “script” with the command ./test.sh which will execute the program in different modes in order to test the         execution times of all the components (sequential, overhead, native parallel and fastflow), the output result will be shown on the terminal and at the same time stored in an external output file called “executution_result.txt” inside the same folder.

  • Mode 2: Compile the program with the command 'make main' and execute it manually using the following command:

    ./main type nw perc time overhead

    where:
    - type: 0 = Sequential , 1=Native thread, 2=Fastflow nw: from 1 to n
    - perc: 0.0 to 1
    - time: 0 = not show 1 = show time
    - overhead: 0 = compute overhead, 1 = not compute overhead
    
    
NOTE: these commands take into account that the opencv and fastflow libraries are already installed on the system you are using, otherwise need to be installed manually.

To change the video on which the program runs, simply drag the new video into the video folder and change the string “videoname” line 16 in the main.cpp file.
