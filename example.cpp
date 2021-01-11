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
    // -------- for non member functions ----------

    {   // non-member function without argument
        easy3d::Timer<> t;
        t.set_timeout(3000, &timeout);
    }

    {   // non-member function with arguments
        easy3d::Timer<int, const std::string &> t;
        const int v = 333;
        const std::string msg = "some random string";
        t.set_interval(2000, &foo, v, msg);
    }

    {   // lambda function without arguments
        easy3d::Timer<> t;
        t.set_interval(2050, [&]() {
            std::lock_guard<std::mutex> guard(mutex);
            std::cout << "After each 2 sec." << std::endl;
        });
    }

    {   // lambda function with arguments
        easy3d::Timer<int, const std::string &> t;
        t.set_interval(3030, [&](float value, const std::string& msg) {
            std::lock_guard<std::mutex> guard(mutex);
            std::cout << "After each 3 sec. value: " << value << ", msg: " << msg << std::endl;
        }, 5, "blabla...");
    }

    // -------- for member functions ----------

    { // member function without argument
        easy3d::Timer<> t;
        Car car1("BMW", 180);
        t.set_interval(1000, &car1, &Car::print_speed);
        t.set_timeout(6000, &car1, &Car::stop);
    }

    { // member function without argument
        easy3d::Timer<> t;
        Car car2("Chevrolet", 120);
        t.set_interval(1000, &car2, &Car::print_speed);
        t.set_timeout(4000, &car2, &Car::stop);
    }

    std::cout << "Timer started..." <<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return EXIT_SUCCESS;
}
