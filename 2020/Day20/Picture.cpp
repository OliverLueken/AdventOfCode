
#include "Picture.hpp"


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
        const auto tileFits = doesTileFit(tile, p);
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

bool picture::doesTileFit(const tile& t, const Position& p) const {
    Position l = {p.first - 1, p.second    };
    Position r = {p.first + 1, p.second    };
    Position u = {p.first    , p.second + 1};
    Position d = {p.first    , p.second - 1};

    const auto [n,m] = t.size();

    if (field.contains(l)){
        tile neighborTile = field.at(l);
        auto fits = true;
        for(auto i = 0ul; i < n; ++i) {
            if( t(i,0) != neighborTile(i,m-1) ){
                fits = false;
            };
        }
        if(!fits) return false;
    }
    if (field.contains(r)){
        tile neighborTile = field.at(r);
        auto fits = true;
        for (auto i = 0ul; i < n; ++i) {
            if(t(i,m-1) != neighborTile(i,0) ){
                fits = false;
            };
        }
        if(!fits) return false;
    }
    if (field.contains(u)){
        tile neighborTile = field.at(u);
        auto fits = true;
        for (auto i = 0ul; i < m; ++i) {
            if(t(0, i) != neighborTile(n-1,i) ){
                fits = false;
            };
        }
        if(!fits) return false;
    }
    if (field.contains(d)){
        tile neighborTile = field.at(d);
        auto fits = true;
        for (auto i = 0ul; i < m; ++i) {
            if(t(n-1, i) != neighborTile(0,i) ){
                fits = false;
            };
        }
        if(!fits) return false;
    }
    return true;
}


void picture::updateFreePoints(const Position& p) {
    auto isInBounds = [this](const Position& p) {
        if (!this->maxBoundsFound) return true;
        const auto [x, y] = p;
        return Utilities::isBetween(x, this->minx, this->maxx+1)
            && Utilities::isBetween(y, this->miny, this->maxy+1);
    };

    if (!maxBoundsFound && length * height == numberOfTiles) {
        maxBoundsFound = true;
        std::erase_if(freePositions, std::not_fn(isInBounds));
    }

    auto addToFreePositions = [this, &isInBounds](auto&& p){
        if(isInBounds(p)){
            this->freePositions.emplace(std::move(p));
        }
    };

    addToFreePositions( Position{p.first - 1, p.second    } );
    addToFreePositions( Position{p.first + 1, p.second    } );
    addToFreePositions( Position{p.first    , p.second + 1} );
    addToFreePositions( Position{p.first    , p.second - 1} );
}

tile picture::picToTile() const {
    auto [tileHeight, tileLength] = field.begin()->second.size();
    auto water = tile{ (maxy-miny+1)*(tileHeight-2), (maxx-minx+1)*(tileHeight-2) };
    for(auto tiley = maxy; tiley>=miny; --tiley){
        for(auto y = 0ul; y<tileHeight-2; ++y){
            for(auto tilex = minx; tilex<=maxx; ++tilex){
                const auto& tile = field.at({tilex, tiley});
                for(auto x = 0ul; x<tileHeight-2; ++x){
                    water( (maxy-tiley)*(tileHeight-2)+y, (tilex-minx)*(tileHeight-2)+x ) = tile(y+1, x+1);
                }
            }
        }
    }
    return water;
}