#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class JitterBuffer {
private:
    queue<pair<uint64_t, Mat>> buffer;
    mutex mtx;
    condition_variable cv;
    const int max_size = 10;

public:
    void push(uint64_t ts, Mat frame) {
        unique_lock<mutex> lock(mtx);

        if (buffer.size() >= max_size) {
            buffer.pop(); // drop old frame
        }

        buffer.push({ts, frame});
        cv.notify_one();
    }

    pair<uint64_t, Mat> pop() {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&] { return !buffer.empty(); });

        auto val = buffer.front();
        buffer.pop();
        return val;
    }
};