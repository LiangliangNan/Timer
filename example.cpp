#include <string>
#include <iostream>
#include "timer.h"

std::mutex mutex;

void foo(int value, const std::string& msg) {
    std::lock_guard<std::mutex> guard(mutex);
    std::cout << "foo: " << value << ", " << msg << std::endl;
}

void timeout() {
    std::lock_guard<std::mutex> guard(mutex);
    std::cout << "timeout" << std::endl;
}


class Car {
public:
    Car(const std::string& name, float speed) : name_(name), speed_(speed) {}

    void print_speed() {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "speed of car "<< name_<< ": " << speed_ << std::endl;
    }

    void stop() {
        std::lock_guard<std::mutex> guard(mutex);
        speed_ = 0;
        std::cout << "car " << name_ << " stoped" << std::endl;
    }

private:
    std::string name_;
    float       speed_;
};


int main()
{
    easy3d::Timer t;

#if 0
    // -------- test for non-member functions ----------

    // call to non-member function
    const int v = 333;
    const std::string msg = "some random string";
    t.set_interval(2000, &foo, v, msg);
    t.set_timeout(3000, &timeout);

    // call to lambda function without arguments
    t.set_interval(2050, [&]() {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "After each 2 sec." << std::endl;
    });

    // call to lambda function with arguments
    const float value = 5;
    t.set_interval(3030, [&](float, const std::string&) {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "After each 3 sec. value: " << value << ", msg: " << msg << std::endl;
    }, value, msg);

#else
    // -------- test for member functions ----------

    Car car1("BMW", 180);
    t.set_interval(1000, &Car::print_speed, &car1);
    t.set_timeout(6000, &Car::stop, &car1);

    Car car2("Chevrolet", 120);
    t.set_interval(1000, &Car::print_speed, &car2);
    t.set_timeout(4000, &Car::stop, &car2);

#endif

    t.set_timeout(8000, [&]() {
        std::lock_guard<std::mutex> guard(mutex);
        t.stop();
        std::cout << "After 8 sec, the timer is stopped!" << std::endl;
        exit(0);
    });

    std::cout << "Timer started..." <<std::endl;

    while(true); // Keep main thread active
}
