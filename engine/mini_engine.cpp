// mini_engine.cpp
// Minimal single-file C++ "game engine" with only the main tick (fixed timestep).
//
// Build:
//   Linux/macOS:  g++ -std=c++17 -O2 -Wall -Wextra -o mini_engine mini_engine.cpp
//   Windows(MSVC): cl /std:c++17 /O2 /W4 mini_engine.cpp
//
// Run:
//   ./mini_engine            // default 60 Hz
//   ./mini_engine 120        // target 120 Hz
//
// Exit with Ctrl+C.

#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <cmath>

using Clock = std::chrono::steady_clock;
using Seconds = std::chrono::duration<double>;

#define MilliSecs_CAST std::chrono::duration_cast<std::chrono::milliseconds>


static volatile std::sig_atomic_t g_sigint = 0;
static void on_sigint(int) { g_sigint = 1; }

// ---------------- Engine-side interfaces ----------------

struct IGame {
    virtual ~IGame() = default;
    virtual void on_init() {}
    virtual void on_update(double /*dt*/) {}
    virtual void on_shutdown() {}
};

struct EngineConfig {
    int    target_hz = 60;
    double max_frame_seconds = 0.25; // clamp long frames (e.g., after breakpoint)
    bool   print_stats = true;
};

class Engine {
public:
    explicit Engine(EngineConfig cfg) : cfg_(cfg) {
        dt_ = 1.0 / static_cast<double>(cfg_.target_hz);
    }

    int run(IGame& game) {
        std::signal(SIGINT, on_sigint);
        game.on_init();

        auto last = Clock::now();
        double acc = 0.0;
        uint64_t ticks = 0;

        // diagnostics
        double sec_accum = 0.0;
        int frames_this_sec = 0;
        uint64_t updates_this_sec = 0;

        if (cfg_.print_stats) {
            std::cout << "[mini_engine] target: " << cfg_.target_hz
                      << " Hz (dt=" << dt_ << "s). Press Ctrl+C to quit.\n";
        }

        while (!quit_) {
            if (g_sigint) quit_ = true;

            auto now = Clock::now();
            double delta_time = std::chrono::duration<double>(now - last).count();
            //auto frame = MilliSecs_CAST(now - last).count();
            last = now;

            if (delta_time > cfg_.max_frame_seconds) delta_time = cfg_.max_frame_seconds;
            acc += delta_time;

            // fixed-step updates
            while (acc >= dt_) {
                game.on_update(dt_);
                acc -= dt_;
                ++ticks;
                ++updates_this_sec;
            }

            // diagnostics once per second (engine has no rendering loop)
            sec_accum += delta_time;
            ++frames_this_sec;
            if (cfg_.print_stats && sec_accum >= 1.0) {
                std::cout << "[t+" << total_seconds_since_start_()
                          << "s] ticks=" << ticks
                          << ", UPS=" << updates_this_sec
                          << " deltaTime = " << delta_time
                          << ", FPS(est)=" << frames_this_sec << "\n";
                sec_accum -= 1.0;
                frames_this_sec = 0;
                updates_this_sec = 0;
            }

            // be a good citizen when no rendering: yield a tiny bit
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        game.on_shutdown();
        if (cfg_.print_stats) std::cout << "[mini_engine] bye.\n";
        return 0;
    }

    void request_quit() { quit_ = true; }

private:
    double total_seconds_since_start_() const {
        static const auto t0 = Clock::now();
        return std::chrono::duration_cast<Seconds>(Clock::now() - t0).count();
    }

    EngineConfig cfg_;
    double dt_{};
    bool quit_{false};
};

// ---------------- Demo game (optional) -------------------
// 你可以删除这一节，只保留 Engine/IGame，然后在 main 里换成你自己的游戏类。

struct DemoGame : IGame {
    // simple physics: y position & velocity with damped bounce
    float y = 10.0f;
    float v = 0.0f;
    float gravity = -9.8f;
    float floor_y = 0.0f;

    // print once per second
    double t_acc = 0.0;

    void on_init() override {
        std::cout << "[DemoGame] init: y=" << y << " v=" << v << "\n";
    }

    void on_update(double dt) override {
        // v += static_cast<float>(gravity * dt);
        // y += static_cast<float>(v * dt);

        // if (y < floor_y) {
        //     y = floor_y;
        //     v = -v * 0.6f;            // damp
        //     if (std::fabs(v) < 0.5f) {// settle
        //         v = 0.0f;
        //         y = floor_y;
        //     }
        // }

        // t_acc += dt;
        // if (t_acc >= 1.0) {
        //     std::cout << "[DemoGame] y=" << y << " v=" << v << "\n";
        //     t_acc -= 1.0;
        // }
    }

    void on_shutdown() override {
        std::cout << "[DemoGame] shutdown.\n";
    }
};

// ------------------------------ main ---------------------

int main(int argc, char** argv) {
    int hz = 60;
    if (argc >= 2) {
        int parsed = std::atoi(argv[1]);
        if (parsed > 0 && parsed <= 1000) hz = parsed;
    }

    EngineConfig cfg;
    cfg.target_hz = hz;

    Engine engine{cfg};
    DemoGame game;          // ⚠️ 想要纯框架？用你自己的 IGame 实现替换它即可。
    return engine.run(game);
}
