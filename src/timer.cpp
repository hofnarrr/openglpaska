#include <chrono>

class Timer {
    std::chrono::steady_clock::time_point t0;
public:
    Timer() { reset(); }
    void reset() { t0 = std::chrono::steady_clock::now(); }
    double get() {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t0).count() / 1000000.0;
    }
};
