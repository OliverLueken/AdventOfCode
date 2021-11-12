#ifndef LIGHTSHOW_H
#define LIGHTSHOW_H

#include <vector>
#include <string>

using coordinate = std::pair<size_t, size_t>;
using grid = std::vector<std::vector<bool>>;

class lightShow{

    grid lights;
    
    std::vector<coordinate> getNeighbors(const unsigned int, const unsigned int) const;
    size_t getOn(const std::vector<coordinate>) const;

public:
    lightShow(std::vector<std::string>);

    void turnCornersOn();
    size_t getOn() const;
    void print() const;
    void update();
};

#endif