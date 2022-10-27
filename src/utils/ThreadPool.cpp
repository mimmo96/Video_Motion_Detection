class ThreadPool {       

    private:

        //set some useful variable
        mutex mtx;
        condition_variable cond;
        bool should_terminate = false; 
        queue <function<int(void)>> task;
        vector<thread> threads;
        int nw = 0;
        atomic<double> total_frame= {0};
        Mat * background;
        int totalFrames = 0;
        double perc = 0;

        //worker function
        void body(){
            
            //while new task arrived, compute result and go waiting new task or for termination
            while(true){
                function<int(void)> t;
                {
                    unique_lock<mutex> lock(mtx);
                    cond.wait(lock,
                                [&](){ return(! this->task.empty() || should_terminate);});
                    
                    if(! this->task.empty()){
                        t = this->task.front();
                        this->task.pop();
                    }    
                    
                    else if(should_terminate ) return;
                }
                int result = t();
                total_frame = total_frame + result;
            }
        }


    public:

        ThreadPool(int nw,Mat *back,VideoCapture *ca,double per) {
            this->nw=nw;
            this->background=back;
            this->perc=per;
            this->totalFrames = ca->get(CAP_PROP_FRAME_COUNT);
            
            //add worker thread
            for(int i=0; i<nw; i++){
                threads.emplace_back([this](){this->body(); });
            }

        }
        
        //add new task for worker
        void addJob(function<int(void)> f){
            {
                unique_lock<mutex> lock(mtx);
                task.push(f);
            }
            cond.notify_one();
        }

        //set the shared variable should_terminate = true
        void stop(){
            {
                unique_lock<mutex> lock(mtx);
                should_terminate = true;
            }
            cond.notify_all();
        }

        //execute join operation
        void end(){
            for(int i=0; i<nw; i++){
                threads[i].join();
            }

            cout << "Total frame detected:" << total_frame << endl;
        }

};