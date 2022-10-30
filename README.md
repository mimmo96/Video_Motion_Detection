# Video Motion Detection

## Description:
Simple motion detection may be implemented by subtracting video frames from some background images. The aim is implement a parallel video motion detector processing video such that:

1. The first frame of the video is taken as “background picture”
2. Each frame is turned to greyscale, smoothed, and subtracted to the greyscale smoothed background picture
3. Motion detect flag will be true in case more the k% of the pixels differ
4. The output of the program is represented by the number of frames with motion detected

OpenCV are used to open video file and read frames only (greyscale and smoothing should be programmed explicitly). Converting an image to greyscale are achieved by substituting each (R,G,B) pixel with a grey pixel with a grey value which is the average of the R, G and B values. Smoothing requires pixel is obtained “averaging” its value with the value of the surrounding pixels. This is achieved by multiplying the 3x3 pixel neighborhood by one of the matrixes and subsequently taking as the new value of the pixel the central value in the resulting matrix computing the average.


## How execute:
 
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
