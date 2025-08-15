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

#include "mini_engine.h"

#include <chrono>
#include <iostream>
#include <thread>

namespace mini {

using Clock   = std::chrono::steady_clock;
using Seconds = std::chrono::duration<double>;

static volatile std::sig_atomic_t g_sigint = 0;
static void on_sigint(int) { g_sigint = 1; }

Engine::Engine(EngineConfig cfg) : cfg_(cfg) {
    dt_ = 1.0 / static_cast<double>(cfg_.target_hz);
}

int Engine::run(IGame& game) {
    std::signal(SIGINT, on_sigint);
    game.on_init();

    auto last = Clock::now();
    double acc = 0.0;
    std::uint64_t ticks = 0;

    // diagnostics
    double sec_accum = 0.0;
    int frames_this_sec = 0;
    std::uint64_t updates_this_sec = 0;

    if (cfg_.print_stats) {
        std::cout << "[mini_engine] target: " << cfg_.target_hz
                  << " Hz (dt=" << dt_ << "s). Press Ctrl+C to quit.\n";
    }

    while (!quit_) {
        if (g_sigint) quit_ = true;

        auto now   = Clock::now();
        double deltaTime = std::chrono::duration_cast<Seconds>(now - last).count();
        last = now;

        // clamp very long frames to avoid spiral-of-death
        if (deltaTime > cfg_.max_frame_seconds) deltaTime = cfg_.max_frame_seconds;
        acc += deltaTime;

        // fixed-step updates
        while (acc >= dt_) {
            game.on_update(dt_);
            acc -= dt_;
            ++ticks;
            ++updates_this_sec;
        }

        // diagnostics once per second (engine has no rendering loop)
        sec_accum += deltaTime;
        ++frames_this_sec;
        if (cfg_.print_stats && sec_accum >= 1.0) {
            std::cout << "[t+" << total_seconds_since_start_()
                      << "s] ticks=" << ticks
                      << ", UPS=" << updates_this_sec
                      << ", LoopHz(est)=" << frames_this_sec << "\n";
            sec_accum -= 1.0;
            frames_this_sec = 0;
            updates_this_sec = 0;
        }

        // Yield a bit to avoid burning 100% CPU when no rendering is present.
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    }

    game.on_shutdown();
    if (cfg_.print_stats) std::cout << "[mini_engine] bye.\n";
    return 0;
}

void Engine::request_quit() { quit_ = true; }

double Engine::total_seconds_since_start_() const {
    static const auto t0 = Clock::now();
    return std::chrono::duration_cast<Seconds>(Clock::now() - t0).count();
}

} // namespace mini