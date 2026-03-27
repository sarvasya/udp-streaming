# 🚀 UDP Low-Latency Video Streaming System (C++)

## 📌 Overview

A **production-style UDP-based real-time video streaming system** built in C++.
Designed with concepts inspired by **RTP/WebRTC**, focusing on **low latency, concurrency, and network resilience**.

---

## ⚡ Key Features

* 📡 UDP-based transport (low latency)
* 🎥 Real-time webcam streaming (OpenCV)
* 📦 Custom RTP-like packet structure
* 🔀 Frame chunking & reassembly
* 🧵 Multithreaded architecture
* ⏱️ Jitter buffer for smooth playback
* 📉 Packet loss tolerance (frame drop strategy)

---

## 🏗️ Architecture

```
Capture → Encode → Packetize → UDP → Reassemble → Jitter Buffer → Decode → Display
```

---

## 🧠 System Design Highlights

* Producer-Consumer model using threads
* MTU-aware packet chunking (1400 bytes)
* Timestamp-based playback smoothing
* Lock-based synchronization with condition variables

---

## 🔧 Tech Stack

* C++
* OpenCV
* POSIX Sockets
* Multithreading (std::thread, mutex, condition_variable)

---

## 🚀 Build & Run

### Install Dependencies

```bash
sudo apt install libopencv-dev
```

### Build

```bash
mkdir build && cd build
cmake ..
make
```

### Run

```bash
./receiver
./sender
```

---

## 📊 Performance

* Latency: ~50–150 ms
* FPS: ~20–30
* Packet Size: 1400 bytes (MTU optimized)

---

## ⚠️ Limitations

* No retransmission (UDP)
* No congestion control
* JPEG compression (can upgrade to H.264)

---

## 🔥 Future Improvements

* Adaptive bitrate (ABR)
* H.264 encoding (FFmpeg)
* Multi-client streaming server
* NAT traversal (STUN)
* QUIC-style reliability layer

---

## 💡 Learning Outcomes

* Real-time systems design
* UDP networking & packetization
* Concurrency in C++
* Handling jitter & packet loss

---

## 🧠 Interview Talking Points

* Built a custom RTP-like protocol over UDP
* Implemented chunking, sequencing, and reassembly
* Designed a multithreaded pipeline with jitter buffer
* Optimized for low latency streaming

---


