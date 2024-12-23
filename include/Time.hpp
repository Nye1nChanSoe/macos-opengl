#ifndef TIME_HPP
#define TIME_HPP

class Time
{
public:
    Time(float time)
        : m_Time(time)
    {
    }

    float inline GetSeconds() const { return m_Time; }
    float inline GetMilliSeconds() const { return m_Time * 1000; }

    operator float() { return m_Time; }

private:
    float m_Time;
};

#endif
