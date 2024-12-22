#ifndef TIME_HPP
#define TIME_HPP

class Time
{
public:
    Time();

    double GetDeltaTime() const;
    double GetFPS() const;

    // Call this method every frame to update the timer
    void Update(double currentTime);

private:
    double previousTime;
    double deltaTime;
    double fps;
};

#endif
