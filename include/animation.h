#ifndef ANIMATION_H
#define ANIMATION_H

#include <functional>

template<typename T>
class Animation
{
public:
    Animation(T startValue, T endValue, double duration)
        : _startValue(startValue), _endValue(endValue), _currentValue(startValue),
          duration(duration) {};

    T CurrentValue() const { return _currentValue; };
    float CurrentProgress() const { return ((_currentTime - _beginTime) / duration); };

    void ApplyDelta(double delta);
    bool Finished() const;

    void Reset() { _beginTime = -1; _currentTime = 0; };

    double duration;
    bool   reverses;

    std::function<void(bool reversed)> onCompletion;

private:
    double _beginTime = -1;
    double _currentTime = 0;
    bool _isReversing = false;

    T _startValue;
    T _currentValue;
    T _endValue;
};

#endif
