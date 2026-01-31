#pragma once
#include <chrono>

enum class ProfileEntryType
{
    Start,
    Stop
};

struct ProfileEntry
{
    ProfileEntryType type;
    std::chrono::time_point<std::chrono::high_resolution_clock> time;
    const char* name;
};

class Profiler
{
public:
    static Profiler* get();

    void addEntry(ProfileEntryType type,const char* name)
    {
        entries.push_back(ProfileEntry{type,std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::high_resolution_clock::now() - captureOffset),name});
    }

    void startCapture()
    {
        previous = std::move(entries);
        captureOffset = std::chrono::high_resolution_clock::now();
    }

    std::vector<ProfileEntry>& getPreviousTrace()
    {
        return previous;
    }
protected:
    std::vector<ProfileEntry> previous;
    std::vector<ProfileEntry> entries;
    std::chrono::time_point<std::chrono::high_resolution_clock> captureOffset;

    // Singleton Part
protected:
    Profiler() = default;
    Profiler(Profiler &other) = delete;
    void operator=(const Profiler &) = delete;

    static Profiler* instance;
};

inline Profiler* Profiler::get()
{
    if(instance == nullptr)
        instance = new Profiler();
    return instance;
}

class ScopeProfile
{
public:
    ScopeProfile(const char* name) : name(name)
    {
        Profiler::get()->addEntry(ProfileEntryType::Start,name);
    }
    ~ScopeProfile()
    {
        Profiler::get()->addEntry(ProfileEntryType::Stop,name);
    }
protected:
    const char* name;
};
