#ifndef HERMITE_H_INCLUDED
#define HERMITE_H_INCLUDED

// Similar to bezier spline, just different basis functions.
// instead of two points and interpolation, two points and slope are used
template<typename T>
T Hermite(float t, T& p1, T& s1, T& p2, T&s2) {
    return p1 * ((1.0f + 2.0f * t) * ((1.0f - t) * (1.0f - t))) +
           s1 * (t * ((1.0f - t) * (1.0f -t))) +
           p2 * ((t * t) * (3.0f - 2.0f * t)) +
           s2 * ((t * t) * (t - 1.0f));
}

#endif // HERMITE_H_INCLUDED

