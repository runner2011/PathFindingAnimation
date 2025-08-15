#ifndef MINI_ENGINE_H
#define MINI_ENGINE_H

// Minimal C++ fixed-timestep "engine" (header)
// Single-thread, no external libs. C++17.
// Provides: IGame interface, EngineConfig, Engine (main loop).
// Usage: implement IGame and call Engine::run(game).

#include <csignal>
#include <cstdint>

namespace mini {

struct IGame {
    virtual ~IGame() = default;
    virtual void on_init() {}
    virtual void on_update(double /*dt*/) {}
    virtual void on_shutdown() {}
};

struct EngineConfig {
    int    target_hz = 60;           // fixed timestep rate
    double max_frame_seconds = 0.25; // clamp long frames to avoid spiral
    bool   print_stats = true;       // print once-per-second diagnostics
};

class Engine {
public:
    explicit Engine(EngineConfig cfg);
    int run(IGame& game);
    void request_quit();

private:
    double total_seconds_since_start_() const;

    // non-copyable
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    EngineConfig cfg_;
    double dt_{};
    bool quit_{false};
};

} // namespace mini

#endif // MINI_ENGINE_H