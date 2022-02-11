
#include "Picture.hpp"

void picture::printFreepoints() const {
    for (auto& p : freepoints) {
        std::cout << p.first << ", " << p.second << std::endl;
    }
    std::cout << std::endl;
}

std::queue<tile> picture::extractTilesFromInput(strvec& input) const {
    std::queue<tile> tiles;
    strvec tilepart;
    int id{};
    for (auto& str : input) {
        if (str.find("Tile") != std::string::npos) {
            tilepart.clear();
            id = stoi(str.substr(str.find(" ") + 1));
            continue;
        }
        if (str.empty()) {
            tile t(id, tilepart);
            // t.print();
            tiles.push(t);
            continue;
        }
        tilepart.push_back(str);
    }
    tile t(id, tilepart);
    // t.print();
    tiles.push(t);

    return tiles;
}

picture::picture(strvec& input) {
    std::queue<tile> tiles;
    tiles = extractTilesFromInput(input);
    numberOfTiles = tiles.size();
    maxBoundsFound = false;

    while (!tiles.empty()) {
        tile t = tiles.front();
        tiles.pop();
        // t.print();
        bool inserted = insertTile(t);
        // rint();
        // printFreepoints();
        if (!inserted) {
            tiles.push(t);
        }
        // std::cin.get();
    }
    // print();
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

bool picture::insertTile(tile& t) {
    if (field.empty()) {
        std::pair<int, int> origin = {0, 0};
        field[origin] = t;
        maxx = 0;
        minx = 0;
        miny = 0;
        maxy = 0;
        length = 1;
        height = 1;
        updateFreePoints(origin);
        return true;
    }
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(t);
        if (inserted) return true;
        t.rotateLeft();
    }
    t.flip();
    for (int i = 0; i < 4; i++) {
        bool inserted = tryInsert(t);
        if (inserted) return true;
        t.rotateLeft();
    }
    return false;
}

bool picture::tryInsert(tile& t) {
    for (auto it = freepoints.begin(); it != freepoints.end(); it++) {
        point p = *it;
        // std::cout << p.first << ", " << p.second << std::endl;
        std::vector<point> neighbors = getNeighbors(p);
        bool tileFits = true;
        for (auto neighbor : neighbors) {
            tileFits = doesTileFit(t, p, neighbor);
            if (!tileFits) break;
        }

        if (tileFits) {
            field[p] = t;
            freepoints.erase(it);
            updateBounds(p);
            updateFreePoints(p);
            return true;
        }
    }
    return false;
}

void picture::updateBounds(point& p) {
    int x = p.first, y = p.second;
    minx = std::min(minx, x);
    maxx = std::max(maxx, x);
    miny = std::min(miny, y);
    maxy = std::max(maxy, y);
    length = maxx - minx + 1;
    height = maxy - miny + 1;
}

std::vector<point> picture::getNeighbors(point p) const {
    point l = {p.first - 1, p.second};
    point r = {p.first + 1, p.second};
    point u = {p.first, p.second + 1};
    point d = {p.first, p.second - 1};

    std::vector<point> neighbors;
    if (field.find(l) != field.end()) neighbors.push_back(l);
    if (field.find(r) != field.end()) neighbors.push_back(r);
    if (field.find(u) != field.end()) neighbors.push_back(u);
    if (field.find(d) != field.end()) neighbors.push_back(d);

    return neighbors;
}

bool picture::doesTileFit(tile& t, point& tp, point& np) const {
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

bool picture::outOfBounds(point& p) const {
    if (!maxBoundsFound) return false;
    int x = p.first;
    int y = p.second;
    return x < minx || maxx < x || y < miny || maxy < y;
}

void picture::updateFreePoints(point& p) {
    if (!maxBoundsFound) {
        if (length * height == numberOfTiles) {
            maxBoundsFound = true;

            for (auto it = freepoints.begin(); it != freepoints.end();) {
                point p_ = *it;
                if (outOfBounds(p_)) {
                    it = freepoints.erase(it);
                } else {
                    it++;
                }
            }
        }
    }

    point l = {p.first - 1, p.second};
    point r = {p.first + 1, p.second};
    point u = {p.first, p.second + 1};
    point d = {p.first, p.second - 1};

    if (!outOfBounds(l) && field.find(l) == field.end()) freepoints.insert(l);
    if (!outOfBounds(r) && field.find(r) == field.end()) freepoints.insert(r);
    if (!outOfBounds(u) && field.find(u) == field.end()) freepoints.insert(u);
    if (!outOfBounds(d) && field.find(d) == field.end()) freepoints.insert(d);
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