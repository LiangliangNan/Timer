/*
*	Copyright (C) 2015 by Liangliang Nan (liangliang.nan@gmail.com)
*	https://3d.bk.tudelft.nl/liangliang/
*
*	Timer is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License Version 3
*	as published by the Free Software Foundation.
*
*	Timer is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

#include <thread>
#include <chrono>

/* Code example:
 *
    void fun1() {
        std::cout << "simple test" << std::endl;
    }

    void fun2(int v, const std::string& msg) {
        std::cout << v << ", " << msg << std::endl;
    }

    void timeout() {
        std::cout << "timeout" << std::endl;
    }


    class Car {
    public:
        void print(int year) {
            std::cout << "BMW of year " << year << std::endl;
        }

        void stop() {
            std::cout << "car stoped" << std::endl;
        }
    };


    int main()
    {
        Timer t;

        // -------- test set_interval() ----------

        // call to non-member function without arguments
        t.set_interval(&fun1, 1000);

        // call to non-member function with arguments
        const int v = 333;
        const std::string msg = "abc";
        t.set_interval(&fun2, 2000, v, msg);

        // call to lambda function without arguments
        t.set_interval([&]() {
            std::cout << "After each 2 sec." << std::endl;
        }, 2000);

        // call to lambda function with arguments
        const float value = 5;
        t.set_interval([&](float, const std::string&) {
            std::cout << "After each 3 sec. value: " << value << ", msg: " << msg << std::endl;
        }, 3000, value, msg);


        // -------- test set_timeout() ----------

        t.set_timeout(&timeout, 3000);

        t.set_timeout([&]() {
            std::cout << "Hey.. After 10 sec. But I will stop the timer!" << std::endl;
            t.stop();
        }, 10000);

        // -------- test for member functions ----------

        Car car;
        const int year = 1981;
        t.set_interval_m(&car, &Car::print, 1000, year);

        t.set_timeout_m(&car, &Car::stop, 5000);


        std::cout << "I am Timer" <<std::endl;
        while(true); // Keep main thread active
    }
 */


class Timer {
public:
    Timer() : stopped_(false) {}

    // execute function 'func' after 'delay' milliseconds.
    // func:  the pointer to the function.
    // delay: the time to be delayed, in milliseconds.
    /* NOTE: arguments must be "const", e.g.,
     * Example:
     *      const int taskID = 3;
     *      const std::string msg = "Finished";
     *      timer.set_timeout(&reset, 5000, taskID, msg);
     * so this won't work
     *      timer.set_timeout(&reset, 5000, 3, "Finished");
     */
    template <class Function, class... Args >
    void set_timeout(Function&& func, int delay, Args&&... args);

    // execute member function 'func' of class 'owner' after 'delay' milliseconds.
    // owner: the pointer to 'Class' instance, e.g., '&a' for 'Class a' or 'this' within Class.
    // func:  the pointer to the member function of 'Class', e.g., '&Class::foo()'.
    // delay: the time to be delayed, in milliseconds.
    /* NOTE: arguments must be "const", e.g.,
     * Example:
     *      const int taskID = 3;
     *      const std::string msg = "Finished";
     *      timer.set_timeout_m(viewer, &Viewer::reset, 5000, taskID, msg);
     * so this won't work
     *      timer.set_timeout_m(viewer, &Viewer::reset, 5000, 3, "Finished");
     */
    template < class Class, class Function, class... Args >
    void set_timeout_m(Class&& owner, Function&& func, int delay, Args&&... args);

    // execute function ‘func' for every 'interval' milliseconds.
    // func:  the pointer to the function.
    // interval: the interval, in milliseconds.
    /* NOTE: arguments must be "const", e.g.,
     * Example:
     *      const std::string speed = "Speed: ";
     *      timer.set_interval_m(&print_speed, 10000, speed);
     * so this won't work
     *      timer.set_interval_m(&print_speed, 10000, "Speed: ");
     */
    template <class Function, class... Args >
    void set_interval(Function&& func, int interval, Args&&... args);

    // execute member function 'func' of class 'owner' for every 'interval' milliseconds.
    // owner: the pointer to 'Class' instance, e.g., '&a' for 'Class a' or 'this' within Class.
    // func:  the pointer to the member function of 'Class', e.g., '&Class::foo()'.
    // interval: the interval, in milliseconds.
    /* NOTE: arguments must be "const", e.g.,
     * Example:
     *      const std::string speed = "Speed: ";
     *      timer.set_interval_m(car, &Car::print_speed, 10000, speed);
     * so this won't work
     *      timer.set_interval_m(car, &Car::print_speed, 10000, "Speed: ");
     */
    template < class Class, class Function, class... Args >
    void set_interval_m(Class&& owner, Function&& func, int interval, Args&&... args);

    // stop the timer
    void stop() { stopped_ = true; }

private:
   bool stopped_;
};



//-------------------------- IMPLEMENTATION ---------------------------


template <class Function, class... Args >
void Timer::set_timeout(Function&& func, int delay, Args&&... args) {
    stopped_ = false;
    std::thread t([=]() {
        if(stopped_) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(stopped_) return;
        func(std::forward<Args>(args)...);
    });
    t.detach();
}


template < class Class, class Function, class... Args >
void Timer::set_timeout_m(Class&& owner, Function&& func, int delay, Args&&... args) {
    stopped_ = false;
    std::thread t([=]() {
        if(stopped_) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(stopped_) return;
        (owner->*func)(std::forward<Args>(args)...);
    });
    t.detach();
}


template <class Function, class... Args >
void Timer::set_interval(Function&& func, int interval, Args&&... args) {
    stopped_ = false;
    std::thread t([=]() {
        while(true) {
            if(stopped_) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(stopped_) return;
            func(std::forward<Args>(args)...);
        }
    });
    t.detach();
}


template < class Class, class Function, class... Args >
void Timer::set_interval_m(Class&& owner, Function&& func, int interval, Args&&... args) {
    stopped_ = false;
    std::thread t([=]() {
        while(true) {
            if(stopped_) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(stopped_) return;
            (owner->*func)(std::forward<Args>(args)...);
        }
    });
    t.detach();
}


#endif  // UTIL_TIMER_H
