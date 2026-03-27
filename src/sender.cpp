#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "../include/packet.hpp"

using namespace std;
using namespace cv;

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in client{};
    client.sin_family = AF_INET;
    client.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &client.sin_addr);

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Camera error\n";
        return -1;
    }

    int frame_id = 0;
    vector<int> params = {IMWRITE_JPEG_QUALITY, 50};

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        vector<uchar> buffer;
        imencode(".jpg", frame, buffer, params);

        int total = (buffer.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;

        for (int i = 0; i < total; i++) {
            Packet pkt{};
            pkt.frame_id = frame_id;
            pkt.seq_no = i;
            pkt.total_chunks = total;

            pkt.timestamp = chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now().time_since_epoch()).count();

            int start = i * CHUNK_SIZE;
            int len = min(CHUNK_SIZE, (int)buffer.size() - start);

            memcpy(pkt.data, buffer.data() + start, len);

            sendto(sock, &pkt, sizeof(pkt), 0,
                   (sockaddr*)&client, sizeof(client));
        }

        frame_id++;
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}