# Timer <img src="timer.png" width="40">

### A single-header C++ class that provides a high-level programming interface for timers.

Timer functionalities are usually implemented in large libraries (e.g., the 
[QTimer of Qt](http://doc.qt.io/archives/qt-5.5/qtimer.html)). 
This Timer class provides a single-header implementation.

With Timer, tasks (i.e., calling to functions) can be easily scheduled at either 
constant intervals or after a specified period. Timer supports any types of 
functions (e.g., function pointers, function objects, Lambda functions, class member
functions) with any number of arguments.


#### Example 1: calling to non-member functions
```c++
    void foo(int value, const std::string& msg) {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "foo: " << value << ", " << msg << std::endl;
    }
    
    void timeout() {
        std::lock_guard<std::mutex> guard(mutex);
        std::cout << "timeout" << std::endl;
    }
    
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
```

#### Example 2: calling to member functions
```c++
    // a trivial class
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
```


#### Example 3: calling to lambda functions
```c++
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
```

### Build

No need to build, it is a single hearder file :smile:
 
You should be able to use Timer on any platforms with a C++ 11 compatible compiler.

---

Should you have any questions, comments, or suggestions, please contact me at liangliang.nan@gmail.com

**_Liangliang Nan_**

https://3d.bk.tudelft.nl/liangliang/

Jan. 3, 2019

Copyright (C) 2019
