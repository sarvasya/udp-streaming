#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "../include/packet.hpp"
#include "../include/jitter_buffer.hpp"

using namespace std;
using namespace cv;

// Frame storage
struct FrameBuffer {
    vector<vector<char>> chunks;
    int received = 0;
    uint64_t timestamp;
};

unordered_map<int, FrameBuffer> frames;
JitterBuffer jb;

// ---------- Thread 1: Receive ----------
void receive_thread(int sock) {
    while (true) {
        Packet pkt{};
        recv(sock, &pkt, sizeof(pkt), 0);

        auto &fb = frames[pkt.frame_id];

        if (fb.chunks.empty()) {
            fb.chunks.resize(pkt.total_chunks);
            fb.timestamp = pkt.timestamp;
        }

        if (fb.chunks[pkt.seq_no].empty()) {
            fb.chunks[pkt.seq_no] =
                vector<char>(pkt.data, pkt.data + CHUNK_SIZE);
            fb.received++;
        }

        if (fb.received == pkt.total_chunks) {
            vector<uchar> full;

            for (auto &c : fb.chunks)
                full.insert(full.end(), c.begin(), c.end());

            Mat frame = imdecode(full, IMREAD_COLOR);

            if (!frame.empty())
                jb.push(fb.timestamp, frame);

            frames.erase(pkt.frame_id);
        }
    }
}

// ---------- Thread 2: Playback ----------
void playback_thread() {
    const int DELAY = 100;

    while (true) {
        auto [ts, frame] = jb.pop();

        this_thread::sleep_for(chrono::milliseconds(DELAY));

        imshow("Stream", frame);
        waitKey(1);
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (sockaddr*)&server, sizeof(server));

    thread t1(receive_thread, sock);
    thread t2(playback_thread);

    t1.join();
    t2.join();
}