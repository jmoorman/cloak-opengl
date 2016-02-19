inline void clampf(float &val, const float min, const float max) {
    if(val < min) {
        val = min;
    }
    else if(val > max) {
        val = max;
    }
}