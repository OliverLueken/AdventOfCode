#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <vector>

typedef std::vector<std::string> strvec;
typedef std::pair<int, int> point;

unsigned long result1 = 0, result2 = 0;

//========================== Tile header start =================================
class tile {
   public:
    strvec data;
    int id;

    tile() : data(), id(){};
    tile(int i, strvec& d) : data(d), id(i){};

    void rotateLeft();
    void flip();
    void print() const;
};
//========================== Tile header end ===================================

void tile::rotateLeft() {
    strvec newdata(data.size());
    for (auto& s : data) {
        for (auto i = 0ul; i < s.size(); i++) {
            newdata[s.size() - i - 1].push_back(s[i]);
        }
    }
    data = newdata;
}

void tile::flip() {
    strvec newdata;
    for (auto it = data.rbegin(); it != data.rend(); it++)
        newdata.push_back(*it);
    data = newdata;
}

void tile::print() const {
    std::cout << id << std::endl;
    for (auto& a : data) std::cout << a << std::endl;
    std::cout << std::endl;
}

//========================== Tile declaration end ==============================

//========================== Picture header start ==============================
class picture {
   public:
    picture(strvec& input);

    long getCornerProd() const;
    void print() const;

    tile picToTile() const;

   private:
    std::map<point, tile> field;
    std::set<point> freepoints;

    int maxx, minx, maxy, miny;
    unsigned long length;
    unsigned long height;
    unsigned long numberOfTiles;
    bool maxBoundsFound;

    bool insertTile(tile& t);
    std::queue<tile> extractTilesFromInput(strvec& input) const;
    bool tryInsert(tile& t);
    std::vector<point> getNeighbors(point p) const;
    bool doesTileFit(tile& t, point& tp, point& np) const;
    void updateFreePoints(point& p);
    void printFreepoints() const;
    void updateBounds(point& p);
    bool outOfBounds(point& p) const;
};
//========================== Picture header end ===============================

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

//========================== Picture Declaration end ===========================

std::vector<std::vector<size_t>> getSnekPos() {
    std::string snek1 = "                  # ";
    std::string snek2 = "#    ##    ##    ###";
    std::string snek3 = " #  #  #  #  #  #   ";

    std::vector<std::vector<size_t>> pos(3);
    size_t p;
    p = snek1.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[0].push_back(p);
        // std::cout << p << std::endl;
        p = snek1.find("#", p + 1);
    }
    p = snek2.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[1].push_back(p);
        // std::cout << p << std::endl;
        p = snek2.find("#", p + 1);
    }
    p = snek3.find("#");
    // std::cout << p << std::endl;
    while (p != std::string::npos) {
        pos[2].push_back(p);
        // std::cout << p << std::endl;
        p = snek3.find("#", p + 1);
    }
    return pos;
}

bool isSnek(auto x, auto y, std::vector<size_t>& snekpos, tile& water) {
    std::string s = water.data[y].substr(x);

    for (size_t i : snekpos) {
        if (s[i] != '#') return false;
    }
    return true;
}

int findSnakes(tile& water) {
    std::vector<std::vector<size_t>> pos = getSnekPos();
    int sneks = 0;
    for (auto y = 0ul; y < water.data.size() - 2; y++) {
        for (auto x = 0ul; x < water.data[y].size() - 19; x++) {
            // std::cout << "findSnakes: " << x << ", " << y << std::endl;
            bool isSnake = isSnek(x, y, pos[0], water) &&
                           isSnek(x, y + 1, pos[1], water) &&
                           isSnek(x, y + 2, pos[2], water);

            if (isSnake) sneks++;
        }
    }
    return sneks;
}

int countRoughWater(tile& water) {
    int count = 0;
    for (auto y = 0ul; y < water.data.size(); y++) {
        std::string s = water.data[y];
        size_t p = s.find("#");
        while (p != std::string::npos) {
            count++;
            p = s.find("#", p + 1);
        }
    }
    return count;
}

void doStuff(strvec& input) {
    picture pic(input);
    result1 = pic.getCornerProd();

    tile water = pic.picToTile();
    water.print();
    int sneks = 0;
    for (int i = 0; i < 4; i++) {
        sneks += findSnakes(water);
        water.rotateLeft();
    }
    water.flip();
    for (int i = 0; i < 4; i++) {
        sneks += findSnakes(water);
        water.rotateLeft();
    }
    result2 = countRoughWater(water) - 15 * sneks;
}

strvec readfile(std::string file) {
    std::string line;
    std::ifstream input(file);
    strvec lines;

    if (input.is_open()) {
        while (getline(input, line)) {
            lines.push_back(line);
        }
        input.close();
    } else {
        std::cout << "Unable to open file\n";
    }
    return lines;
}

int main() {
    strvec input = readfile("input.txt");

    doStuff(input);

    std::cout << result1 << "\n";
    std::cout << result2 << "\n";
}
