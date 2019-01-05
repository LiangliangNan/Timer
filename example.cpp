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
    Timer t;

#if 0
    // -------- test for non-member functions ----------

    // call to non-member function
    const int v = 333;
    const std::string msg = "some random string";
    t.set_interval(&foo, 2000, v, msg);
    t.set_timeout(&timeout, 3000);

    // call to lambda function without arguments
    t.set_interval([&]() {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "After each 2 sec." << std::endl;
    }, 2050);

    // call to lambda function with arguments
    const float value = 5;
    t.set_interval([&](float, const std::string&) {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "After each 3 sec. value: " << value << ", msg: " << msg << std::endl;
    }, 3030, value, msg);

#else
    // -------- test for member functions ----------

    Car car1("BMW", 180);
    t.set_interval_m(&car1, &Car::print_speed, 1000);
    t.set_timeout_m(&car1, &Car::stop, 6000);

    Car car2("Chevrolet", 120);
    t.set_interval_m(&car2, &Car::print_speed, 1000);
    t.set_timeout_m(&car2, &Car::stop, 4000);

#endif

    t.set_timeout([&]() {
        std::lock_guard<std::mutex> guard(mutex);
        t.stop();
        std::cout << "After 8 sec, the timer is stopped!" << std::endl;
        exit(0);
    }, 8000);

    std::cout << "Timer started..." <<std::endl;

    while(true); // Keep main thread active
}
