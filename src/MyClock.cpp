#include "MyClock.hpp"
//#include <iostream>

MyClock::MyClock(float optimalFrameRate){
    currentIndex = 0;
    for(int i = 0; i < CLOCK_MEMORY;++i)timeSaved[i] = 1.0f / optimalFrameRate;

}

void MyClock:: add(float time){
    currentIndex = (currentIndex + 1)%CLOCK_MEMORY;

    timeSaved[currentIndex] = time;
}

float MyClock::avg(){
    float sum = 0;
    for(int i = 0; i < CLOCK_MEMORY;++i)sum+=timeSaved[i];
    return sum/CLOCK_MEMORY;
}

// void MyClock::printTimes(){
//     for(int i = 0; i < CLOCK_MEMORY; ++i){
//         std::cout << timeSaved[i] << " | ";
//     }
//     std::cout<< "    " << avg() <<std::endl;
// }