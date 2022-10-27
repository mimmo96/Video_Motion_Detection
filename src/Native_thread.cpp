//read new frame and push compute function into queue of threads
void read_and_push(VideoCapture *cap, Mat * background ,ThreadPool *tp,double perc,int end){

    int start = 0; 

    //declare function that return 1 if frame was detected
    auto f = [&](Mat frame) {
        if(composition_fun(background,frame,perc)) return 1;
        else return 0;

    };

    while(start < end-1 ){

        Mat fra;

        if(!cap->read(fra)){
            break;
        } 

        auto fx = (bind(f,fra));

        //put the task in queue
        tp->addJob(fx);

        start = start +1;
    }

    cout << "Total frame read: " << start << endl; 
    tp->stop();

}

void Thread(int nw,double perc){

    VideoCapture* cap = new VideoCapture(videoname);

    int total_frames = cap->get(CAP_PROP_FRAME_COUNT) ;

    Mat frame; 

    //read first frame as background
    cap->read(frame);

    print_stats(frame);
    
    //convert in grey and do smoothing
    Mat * grey = togrey(frame);
    Mat * background = smoothing(grey);

    //create threadpool and read all remaning frames
    ThreadPool *tp = new ThreadPool(nw,background,cap,perc);
    read_and_push(cap,background,tp,perc,total_frames);

    tp->end();

    delete tp;
    delete background;
    
    cap->release();
    delete cap;
 
}

//function for adding 720 task on threadpool queue
void compute_something(ThreadPool *tp){
     auto f = [&]() {
            return 1;
        };

        int k= 0;
        while(k<720){
            tp->addJob(f);
            k++;
        }

}


void compute_overhead(int nw ){

    VideoCapture* cap = new VideoCapture(videoname);

    //read first frame as background
    Mat frame; 
    cap->read(frame);

    print_stats(frame);
    
    //convert in grey
    Mat * grey = togrey(frame);

    Mat * background = smoothing(grey);

    long overhead=0;

    { 
        utimer u(&overhead);
        ThreadPool *tp = new ThreadPool(nw,background,cap,0.0);

        //compute_something(tp);

        tp->stop();
        tp->end();

        delete tp;
    }

    delete background; 

    cout << "nw:" << nw <<" Overhead: " << overhead << " us" << endl;

    cap->release();
    delete cap;

}
