//FIXME When this is too big core dumps WHY?
#define CLOCK_MEMORY 50


class MyClock{
    public:
        float timeSaved[CLOCK_MEMORY];
        int currentIndex;

        MyClock(float optimalFrameRate);

        void add(float time);

        float avg();

        void printTimes();
};