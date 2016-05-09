#ifndef ANIMATION_H
#define ANIMATION_H

template<typename T>
class Animation
{
public:
    Animation(T startValue, T endValue, double duration)
        : _startValue(startValue), _endValue(endValue), _currentValue(startValue),
          duration(duration) {};

    T CurrentValue() { return _currentValue; };

    void ApplyDelta(double delta);
    bool Finished() const;

    double duration;
    bool   reverses;

private:
    double _beginTime = -1;
    double _currentTime = 0;

    T _startValue;
    T _currentValue;
    T _endValue;
};

#endif
