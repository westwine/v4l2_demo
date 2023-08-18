#include "CamV4L2.h"

int main(int argc, char** argv)
{
    int videoNumber = 0;
    int shift=0;
    if(argc >= 2)
        videoNumber = *argv[1];
    
    CamV4L2 cam(std::string("/dev/video"+std::string(std::to_string(videoNumber))).c_str());

    cam.run();

    return 0;
}