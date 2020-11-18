#include <iostream>

// The only function is to keep a counter
// that can be incremented or decremented,
// it is not supposed to launch a rocket
struct Counter
{
    int count{0};

    Counter(int init) :  count(init) {}

    void increment()
    {
        count++;
    }

    void decrement()
    {
        count--;
    }

    bool launch_rockets() const
    {
        std::cout << "Hey man, it's not my responsability. I'm just a counter trying to live my life." << std::endl;
        return false;
    }

};

int main()
{
    std::cout << "Single Responsability Principle" << std::endl;

    Counter counter(10);
    std::cout << "Counter init : " << counter.count << std::endl;

    counter.increment();

    std::cout << "Counter incremented : " << counter.count << std::endl;

    counter.decrement();

    std::cout << "Counter decremented : " << counter.count << std::endl;

    std::cout << "Counter launch rockets!!!!!"  << std::endl;
    counter.launch_rockets();

    return 0;
}
