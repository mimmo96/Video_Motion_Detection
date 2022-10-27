#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

using namespace ff;

Mat *background;
double perc=0.00;
VideoCapture cap = VideoCapture(videoname);

//emitter 
struct emitter: ff_node_t<int,Mat> {
    emitter(const bool overhead):overhead(overhead) {}
    Mat* svc(int *) {
        
        //if i must compute overhead send an empty matrix 720 times
        if(overhead){
            int k=0;
            Mat* empty = new Mat(0,3,CV_8UC3);
            while(k<720){
                ff_send_out(empty);
                k++;  total_add++;
            }
        }
        
        //normal execution
        else{

            //read all frames and send them to each to worker threads
            while(true) {
                Mat *frame = new Mat(background->rows, background->cols, CV_8UC3);

                cap >> *frame;

                if(frame->empty())  break;

                ff_send_out(frame);

                total_add++;

            }
        }
        
        //send EOS for termination
        return EOS;
    }

    void svc_end() { std::cout << "total frame read = " << total_add << "\n"; }
    
    int total_add = 0;
    bool overhead = false;
};

//worker
struct Worker: ff_node_t<Mat,int> {
    int* svc(Mat * frame) { 
        Mat &temp = *frame;

        //if i receive an empty return 0 as frame detected (used for measures overhead)
        if(temp.empty()){
            return new int(0);
        } 

        //if i receive a normal Matrix, compute a computation and send result to collector thread
        else if(composition_fun(background,temp,perc)){
            delete frame;
            return new int(1);      //if frame are detected
        }
        else{
            delete frame;
            return new int(0);      //if frame are not detected
        } 
    }
};

//collector
struct collector: ff_node_t<int> {

    //receive sum all result received from worker thread and delete task
    int* svc(int * task) { 
        int &t = *task;
        sum += t; 
        delete task;
        return GO_ON; 
    }

    void svc_end() { std::cout << "total frame detection: "  << sum << "\n"; }
    int sum = 0;
};

void fastflow(int nw,double perce, bool overhead,bool time) {    

    //read first frame
    Mat frame; 
    cap.read(frame);

    perc=perce;

    print_stats(frame);

    //convert in grey and do smoothing
    Mat * grey = togrey(frame);
    background = smoothing(grey);

    emitter  emitter(overhead);
    collector  Collector;

    //create a vector of worker
    vector<unique_ptr<ff_node> > W;
    for(size_t i=0;i<nw;++i) W.push_back(make_unique<Worker>());

    ff_Farm<> farm(move(W));        
    farm.add_emitter(emitter);
    farm.add_collector(Collector);
    
    farm.set_scheduling_ondemand();

    ffTime(START_TIME);
    if (farm.run_and_wait_end()<0) {
        error("running farm");
    }

    ffTime(STOP_TIME);

    //show time for overhead
    if(time)cout <<"nw: " << nw << " Time: " << ffTime(GET_TIME)*1000 << " us \n";

}
