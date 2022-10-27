void Sequential(double perc){

    //open video and read first frame
    VideoCapture* cap = new VideoCapture(videoname);

    Mat frame; 
    cap->read(frame);

    //print some stats for video
    print_stats(frame);
    
    //convert in grey and do smoothing
    Mat * grey = togrey(frame);
    Mat * background = smoothing(grey);
    
    int total_add=0;
    int diff=0;

    while(true) {
        Mat fra;

        if(!cap->read(fra)) break;

        //if detection 
        if(composition_fun(background,fra,perc)) diff++;
        total_add++;

    }

    cout << "Total frame:" << total_add << endl;
    cout << "Total frame detection: "  << diff << endl;

    delete background;
    
    cap->release();
    delete cap;
 
}

//function to compute time for each operations
void compute_time(){

    VideoCapture* cap = new VideoCapture(videoname);

    int framm = cap->get(CAP_PROP_FRAME_COUNT);
    cout << "frame_num:" << framm << endl;

    long t_read=0,t_grey=0, t_smoothing=0,t_makeall=0,t_average=0;

    Mat frame; 
    Mat * background;
    Mat * grey;
    int total=0;

    //compute time for read each frame
    {
        utimer u(&t_read);
        while(true) {
          if(!cap->read(frame)) break;
          total++;
        }
    }

    cap = new VideoCapture(videoname);
    cap->read(frame);
    {   
        utimer u(&t_grey);
        grey = togrey(frame);
    }
    {   
        utimer u(&t_smoothing);
        background = smoothing(grey);
    }
    {   
        utimer u(&t_makeall);
        composition_fun(background,frame,0.0);
    }
    {
        utimer u(&t_average);
        
        while(true) {
            Mat fra;

            if(!cap->read(fra)) break;

            composition_fun(background,fra,0.0);
        }

    }

    cout << "t_read: " << t_read/total  << " us" << endl;
    cout << "t_grey: " << t_grey  << " us" << endl;
    cout << "t_smoothing: " << t_smoothing  << " us" <<endl;
    cout << "t_makeall: " << t_makeall << " us" << endl;
    cout << "t_average_read_and_compute: " << t_average/total << " us" << endl;

    delete cap;
    delete background;
}


//function to compute time for each operations on average for all frames
void compute_for_each(){
    
    long t_read=0, t_grey=0, t_smoothing=0,t_composition_fun=0;
    long total_r=0, total_grey=0, total_smoothing=0,total_composition_fun=0;

    Mat frame; 
    Mat * background;
    Mat * temp;
    Mat * grey;
    int total=0;
    int detectio=0;

    VideoCapture cap = VideoCapture(videoname);

    cap.read(frame);
    grey = togrey(frame);
    background = smoothing(grey);

   
    while(true) {
        Mat frame;

        {
            utimer u(&t_read);
            if(!cap.read(frame)) break;
        }
        total_r=total_r+t_read;
        
        {
            utimer u(&t_grey);
            grey = togrey(frame);
        }
        total_grey=total_grey+t_grey;
        {
            utimer u(&t_smoothing);
            temp = smoothing(grey);
        }

        total_smoothing=total_smoothing+t_smoothing;
        
        {
            utimer u(&t_composition_fun);
            if( composition_fun(background,frame,0.0)) detectio=detectio+1;
        }
        total_composition_fun = total_composition_fun + t_composition_fun;


        total=total+1;
    }
         
    cout << "total_r: " << total_r/total << endl;
    cout << "total_grey: " << total_grey/total << endl;
    cout << "total_smoothing: " << total_smoothing/total << endl;
    cout << "total_composition_fun: " << total_composition_fun/total << endl;
    cout << "total frame detection: " << detectio << endl;
    cout << "total: " << total << endl;
    cap.release();

    delete background;
    delete temp;
}
