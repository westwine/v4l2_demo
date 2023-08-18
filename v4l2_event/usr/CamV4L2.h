#ifndef Cam_V4L2_H_
#define Cam_V4L2_H_

/*
* Camera device using Video4Linux2
*/

#include <string>

class CamV4L2 {
    public:
        explicit CamV4L2(std::string device);
        void run();

    private:

        // image buffer variable    
        struct Buffer
        {
            unsigned index;
            unsigned char * start;
            size_t length;
        };

        Buffer buffer_;

        int fd_;

        // functions
        int open_camera(std::string device);
        int print_caps();
        int get_thermal();
        void close_camera();

};


#endif //Cam_V4L2_HPP_