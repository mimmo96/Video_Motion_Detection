#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>

using namespace std;
using namespace cv;

//#define videoname "video/motion.mp4"
#define videoname "video/4k.mp4"

#include "src/utils/Functions.cpp"
#include "src/utils/utimer.cpp"
#include "src/utils/ThreadPool.cpp"
#include "src/Sequential.cpp"
#include "src/Native_thread.cpp"
#include "src/Fastflow.cpp"

int main(int argc, char * argv[]) {

    long dur=0;

    if(argc < 6 ){
        cout << "Usage ./main type nw perc time overhead " << endl;
        return 0;
    }

    //get parameter
    int type = atoi(argv[1]);
    int nw = atoi(argv[2]);
    double perc = atof (argv[3]);
    int time = atoi(argv[4]);    
    int overhead = atoi(argv[5]);    

    //check if parameter is valid else stop
    if(!checkpar(type,nw,perc,time,overhead)) return 0;

    //sequential execution
    if(type==0){
        {
            utimer u(&dur);
            
            //in this case overhead = just compute time of each function is not real overhead!!
            if(overhead) compute_for_each();
            else Sequential(perc);
        }
    }

    //native thread
    else if(type==1){
        {
            utimer u(&dur);
            if(overhead) compute_overhead(nw);
            else Thread(nw,perc);
        }
    }

    //fastflow
    else{
        {
            utimer u(&dur);
            if(overhead){
                fastflow(nw,perc,true,time);
            } 
            else fastflow(nw,perc,false,0);
        }
    }

    if(time)  cout << "nw:" << nw << " dur: " << (double) dur/1000000 << "s" << endl;

    return 0;

}   

