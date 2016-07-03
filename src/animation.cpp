#include "animation.h"

#include "graphics.h"
#include <iostream>

using namespace rgb_matrix;

template<>
void Animation<Color>::ApplyDelta(double delta)
{
    _currentTime = delta;

    if (_beginTime < 0) {
        _beginTime = delta;
    }

    double progress = (_currentTime - _beginTime) / duration;

    _currentValue.r = _startValue.r + ((_endValue.r - _startValue.r) * progress);
    _currentValue.g = _startValue.g + ((_endValue.g - _startValue.g) * progress);
    _currentValue.b = _startValue.b + ((_endValue.b - _startValue.b) * progress);

    if (Finished()) {
        try {
            onCompletion(_isReversing);
        } catch (std::bad_function_call e) {
            // empty is ok.
        }

        if (reverses) {
            _isReversing = !_isReversing;

            Color startValueTmp = _startValue;
            _startValue = _endValue;
            _endValue = startValueTmp;

            _currentValue = _startValue;

            _beginTime = -1;
        }
    }
}

template <typename T>
bool Animation<T>::Finished() const
{
    return ((_currentTime - _beginTime) >= duration);
}
