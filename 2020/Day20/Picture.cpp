
#include "Picture.hpp"

void picture::printFreePositions() const {
    for (auto& p : freePositions) {
        std::cout << p.first << ", " << p.second << std::endl;
    }
    std::cout << std::endl;
}

picture::picture(std::queue<tile>&& tiles) {
    numberOfTiles = tiles.size();
    maxBoundsFound = false;

    while (!tiles.empty()) {
        auto tile = tiles.front();
        tiles.pop();
        const auto inserted = insertTile(tile);
        if (!inserted) {
            tiles.push(std::move(tile));
        }
    }
}

long picture::getCornerProd() const {
    long sum = 1;
    sum *= field.at({minx, miny}).id;
    sum *= field.at({maxx, miny}).id;
    sum *= field.at({minx, maxy}).id;
    sum *= field.at({maxx, maxy}).id;
    return sum;
}

void picture::print() const {
    std::cout << "Printing picture\n";
    for (auto& [p, t] : field) {
        std::cout << p.first << ", " << p.second << ": " << t.id << std::endl;
        t.print();
    }
    std::cout << "Printing picture done\n\n";
}

bool picture::insertTile(tile& tile) {
    if (field.empty()) {
        const auto origin = Position{0, 0};
        field[origin] = tile;
        updateFreePoints(origin);
        return true;
    }
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(tile);
        if (inserted) return true;
        tile.rotateLeft();
    }
    tile.flip();
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(tile);
        if (inserted) return true;
        tile.rotateLeft();
    }
    return false;
}

bool picture::tryInsert(tile& tile) {
    for (const auto& p : freePositions) {
        // Position p = *it;
        // std::cout << p.first << ", " << p.second << std::endl;
        const auto neighbors = getNeighbors(p);
        auto tileFits = true;
        for (const auto& neighbor : neighbors) {
            tileFits = doesTileFit(tile, p, neighbor);
            if (!tileFits) break;
        }

        if (tileFits) {
            field[p] = std::move(tile);
            updateBounds(p);
            updateFreePoints(p);
            freePositions.erase(p);
            return true;
        }
    }
    return false;
}

void picture::updateBounds(const Position& p) {
    int x = p.first, y = p.second;
    minx = std::min(minx, x);
    maxx = std::max(maxx, x);
    miny = std::min(miny, y);
    maxy = std::max(maxy, y);
    length = maxx - minx + 1;
    height = maxy - miny + 1;
}

std::vector<Position> picture::getNeighbors(const Position& p) const {
    Position l = {p.first - 1, p.second};
    Position r = {p.first + 1, p.second};
    Position u = {p.first, p.second + 1};
    Position d = {p.first, p.second - 1};

    std::vector<Position> neighbors;
    if (field.find(l) != field.end()) neighbors.push_back(l);
    if (field.find(r) != field.end()) neighbors.push_back(r);
    if (field.find(u) != field.end()) neighbors.push_back(u);
    if (field.find(d) != field.end()) neighbors.push_back(d);

    return neighbors;
}

bool picture::doesTileFit(const tile& t, const Position& tp, const Position& np) const {
    tile n = field.at(np);
    if (tp.first < np.first) {  // left
        // std::cout << "left\n";
        for (auto i = 0ul; i < t.data.size(); i++) {
            if (t.data[i].back() != n.data[i].front()) return false;
        }
    } else {
        if (tp.first > np.first) {  // right
            // std::cout << "right\n";
            for (auto i = 0ul; i < t.data.size(); i++) {
                if (n.data[i].back() != t.data[i].front()) return false;
            }
        } else {
            if (tp.second > np.second) {  // up
                // std::cout << "up\n";
                return t.data.back() == n.data.front();
            } else {  // down
                // std::cout << "down\n";
                return t.data.front() == n.data.back();
            }
        }
    }

    return true;
}

bool picture::outOfBounds(Position& p) const {
    if (!maxBoundsFound) return false;
    int x = p.first;
    int y = p.second;
    return x < minx || maxx < x || y < miny || maxy < y;
}

void picture::updateFreePoints(const Position& p) {
    if (!maxBoundsFound) {
        if (length * height == numberOfTiles) {
            maxBoundsFound = true;

            for (auto it = freePositions.begin(); it != freePositions.end();) {
                Position p_ = *it;
                if (outOfBounds(p_)) {
                    it = freePositions.erase(it);
                } else {
                    it++;
                }
            }
        }
    }

    Position l = {p.first - 1, p.second};
    Position r = {p.first + 1, p.second};
    Position u = {p.first, p.second + 1};
    Position d = {p.first, p.second - 1};

    if (!outOfBounds(l) && field.find(l) == field.end()) freePositions.insert(l);
    if (!outOfBounds(r) && field.find(r) == field.end()) freePositions.insert(r);
    if (!outOfBounds(u) && field.find(u) == field.end()) freePositions.insert(u);
    if (!outOfBounds(d) && field.find(d) == field.end()) freePositions.insert(d);
}

tile picture::picToTile() const {
    tile water;
    // auto tileLength = field.at({0, 0}).data.at(0).size();
    auto tileHeight = field.at({0, 0}).data.size();
    for (int y = maxy; y >= miny; y--) {
        for (auto tiley = 1ul; tiley < tileHeight - 1; tiley++) {
            std::string s;

            for (int x = minx; x <= maxx; x++) {
                std::string tilestr = field.at({x, y}).data.at(tiley);
                s.append(tilestr.substr(1, tilestr.size() - 2));
            }
            water.data.push_back(s);
        }
    }
    return water;
}