#ifndef LIGHTSHOW_H
#define LIGHTSHOW_H

#include <vector>
#include <string>

using coordinate = std::pair<size_t, size_t>;
using grid = std::vector<std::vector<bool>>;

class lightShow{

    grid lights;

    unsigned int getNeighborLights(const unsigned int, const unsigned int) const;

public:
    lightShow(std::vector<std::string>);

    void turnCornersOn();
    size_t getOn() const;
    void print() const;
    void update();
};

#endif