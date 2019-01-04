# Timer

### A single-header C++ class that provides a high-level programming interface 
for timers.

Timer functionalities are usually implemented in large libraries (e.g., the 
[QTimer of Qt](http://doc.qt.io/archives/qt-5.5/qtimer.html)). 
This Timer class provides a single-header implementation.

With Timer, tasks (i.e., calling to functions) can be easily scheduled at either 
constant intervals or after a specified period. Timer supports any types of 
functions with any number of arguments, e.g.,
 - non-member functions;
 - lambda functions;
 - member functions of classes.


#### Example 1: calling to non-member functions
```c++
    // create a timer instance
    Timer t;

    // the function you want to call at a constant interval
    void foo(int value);

    // the function to be called when timeout.
    void timeout();

    const int value = 333;
    // function "foo" will be executed every second.
    t.set_interval(&foo, 1000, value);

    // function "timeout()" will be executed after 3 seconds.
    t.set_timeout(&timeout, 3000);
```

#### Example 2: calling to member functions
```c++
    // a trivial class
    class Car {
    public:
        Car(const std::string& name, float speed);
        void print_speed();
        void stop();
    private:
        std::string name_;
        float       speed_;
    };

    // car 1 reports its speed every 2 seconds and it stops after 10 seconds
    Car car1("BMW", 180);
    t.set_interval_m(&car1, &Car::print_speed, 2000);
    t.set_timeout_m(&car1, &Car::stop, 10000);

    // car 2 reports its speed every 3 seconds and it stops after 20 seconds
    Car car2("Chevrolet", 120);
    t.set_interval_m(&car2, &Car::print_speed, 3000);
    t.set_timeout_m(&car2, &Car::stop, 20000);
```


#### Example 3: calling to lambda functions
```c++
    const float value = 5;
    t.set_interval([&](float, const std::string&) {
        std::cout << "After every 3 sec. value: " << value << ", message: " << msg << std::endl;
    }, 3000, value, msg);

    t.set_timeout([&]() {
        t.stop();
        std::cout << "After 8 sec, the timer is stopped!" << std::endl;
    }, 8000);
```

### Build

No need to build, it is a single hearder file :smile: 
You should be able to use Timer on any platforms with a C++ 11 compatible compiler.

---

Should you have any questions, comments, or suggestions, please contact me at liangliang.nan@gmail.com

**_Liangliang Nan_**

https://3d.bk.tudelft.nl/liangliang/

Jan. 3, 2018

Copyright (C) 2018
