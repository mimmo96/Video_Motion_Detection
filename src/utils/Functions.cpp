/* 
    checks if the parameters entered by the user are correct 
*/
bool checkpar(int type, int nw, double perc, int time, int overhead){
    bool is_all_ok = true;

    if(type > 2 || type < 0){
        cout << "type correct value: 0 1 2" << endl;
        is_all_ok = false;
    } 
    if(nw <= 0){
        cout << "nw correct value: >0" << endl;
        is_all_ok = false;
    }
    if(perc < 0 || perc > 1){
        cout << "perc correct value: 0 to 1" << endl;
        is_all_ok = false;
    }
    if(time < 0 || time > 1){
        cout << "time correct value: 0 or 1" << endl;
        is_all_ok = false;
    }
    if(overhead < 0 || overhead > 1){
        cout << "overhead correct value: 0 or 1" << endl;
        is_all_ok = false;
    }

    return is_all_ok;
}

/*
    print useful statistics of the image
*/
void print_stats(Mat input_frame){
    cout << "number of channels: " << input_frame.channels() << endl;

    // print total element present into matrix
    cout << "number of elements: " << input_frame.rows*input_frame.cols << endl;

    //print number of rows
    cout << "number of rows: " << input_frame.rows << endl;
    cout <<"number of columns: " <<  input_frame.cols << endl;

    cout << "size: " << input_frame.size() << endl;

    cout << "type: " << input_frame.type() << endl;

}

/*
    convert RGB pixel into a single channel and store it in grey
*/
Mat* togrey(Mat input_frame){

    int rows = input_frame.rows;
    int cols = input_frame.cols;

    //create new Matrix for store grey values
    Mat *to_grey=new Mat(rows,cols,CV_8UC1,Scalar(0));

    //average R+G+B / 3
    for (int i = 0; i <rows; i++) {
        for (int j = 0; j < cols; j++) {
            to_grey->at<uchar>(i, j)= ( input_frame.at<Vec3b>(i, j)[0] + input_frame.at<Vec3b>(i, j)[1] + input_frame.at<Vec3b>(i, j)[2])/3;
        }   
    }

   return to_grey;
}

/*
    smoothing of input image
*/
Mat* smoothing(Mat* input_frame){

    int rows = input_frame->rows;
    int cols = input_frame->cols;
    int offset = 1;
    int acc = 0, num_el = 0;
    int sx_l=offset, sx_r=offset, dx_l=offset, dx_r=offset;
    
    //create new Matrix for store smoothing values
    Mat* to_smoothing = new Mat(rows,cols,CV_8UC1,Scalar(0));
    
    for (int i = 0; i <rows; i++) {
        for (int j = 0; j < cols; j++) {

            sx_l = i > 0 ? -offset : 0;
            sx_r = i < rows-1 ? offset : 0;
            dx_l = j > 0 ? -offset : 0;
            dx_r = j < cols-1 ? offset : 0;

            acc = 0;
            num_el = 0;
            
            for(int l = sx_l; l<= sx_r; l++){
                for(int k = dx_l; k<= dx_r; k++){
                   acc += input_frame->at<uchar>(i+l, j+k);
                    num_el++;
                }
            }

            to_smoothing->at<uchar>(i, j) = acc/num_el;
           
        }   
    } 

    delete input_frame;

    return to_smoothing;

}


/*
    transform to grey, do the smoothing and check the different pixels
*/
bool composition_fun(Mat* background, Mat input_frame, double k){
    
    int rows = input_frame.rows;
    int cols = input_frame.cols;

    //create new Matrix for store grey values
    Mat *grey=new Mat(rows,cols,CV_8UC1,Scalar(0));

    //compute grey value
    for (int i = 0; i <rows; i++) {
        for (int j = 0; j < cols; j++) {
            grey->at<uchar>(i, j)= ( input_frame.at<Vec3b>(i, j)[0] + input_frame.at<Vec3b>(i, j)[1] + input_frame.at<Vec3b>(i, j)[2])/3;
        }   
    }

    int difference=0;
    int offset = 1;
    int num_el = 0;
    int acc = 0, diff=0;
    int num_pixels=rows*cols;
    int sx_l=offset, sx_r=offset, dx_l=offset, dx_r=offset;
    
    //compute smoothing operation
    for (int i = 0; i <rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            // change values to handle image borders
            sx_l = i > 0 ? -offset : 0;
            sx_r = i < rows-1 ? offset : 0;
            dx_l = j > 0 ? -offset : 0;
            dx_r = j < cols-1 ? offset : 0;

            acc = 0;
            num_el = 0;
            
            for(int l = sx_l; l<= sx_r; l++){
                for(int k = dx_l; k<= dx_r; k++){
                    acc += grey->at<uchar>(i+l, j+k);
                    num_el++;
                }
            }
            
            diff = acc/num_el;

            if(background->at<uchar>(i, j) != diff){
                difference ++;
            } 
    
        }   
    } 

    double per_diff = (double)difference/num_pixels;
    delete grey;

    if(per_diff>k ) return true;

    return false;

}

