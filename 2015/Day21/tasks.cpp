
#include "../../lib/readFile.hpp"
#include "../../lib/utilities.hpp"

#include <iostream>
#include <vector>
#include <climits>
#include <cmath>

struct character{
    unsigned int hp{0u};
    unsigned int damage{0u};
    unsigned int armor{0u};
};

struct item{
    unsigned int cost{0u};
    unsigned int damage{0u};
    unsigned int armor{0u};
};

struct store{
    std::vector<item> weapons{};
    std::vector<item> armor{};
    std::vector<item> rings{};
};

auto parseEnemyInput = [](const auto&& input){
    character enemy{};
    enemy.hp     = std::stoi(Utilities::split(input[0])[2]);
    enemy.damage = std::stoi(Utilities::split(input[1])[1]);
    enemy.armor  = std::stoi(Utilities::split(input[2])[1]);
    return enemy;
};

auto parseStoreInput = [](const auto&& input){
    auto stockItems = [](const auto& input) mutable{
        std::vector<item> items{};
        for(const auto row : input){
            const auto split = Utilities::split(row);
            const auto n = split.size();
            items.emplace_back(std::stoi(split[n-3]), std::stoi(split[n-2]), std::stoi(split[n-1]));
        }
        return items;
    };

    store store{};
    store.weapons = stockItems(input | std::views::drop(1) | std::views::take(5));

    store.armor = stockItems(input | std::views::drop(7) | std::views::take(5));
    store.armor.insert(std::begin(store.armor), item{}); //armor is optional

    store.rings = stockItems(input | std::views::drop(13));
    store.rings.insert(std::begin(store.rings), item{}); //we need two optional rings
    store.rings.insert(std::begin(store.rings), item{});

    return store;
};

auto getStats = [](const auto& weapon, const auto& armor, const auto& ring1, const auto& ring2){
    auto addStats = [](auto& stats, const auto& item){
        stats.first.damage+=item.damage;
        stats.first.armor+=item.armor;
        stats.second+=item.cost;
    };

    auto stats = std::make_pair<character, unsigned int>({100u,0u,0u}, 0u); //{player, gold}
    addStats(stats, weapon);
    addStats(stats, armor);
    addStats(stats, ring1);
    addStats(stats, ring2);
    return stats;
};

auto playerWinsFight = [](const auto& enemy, const auto& player, const auto& cmp){
    auto getDamageOfAgainst = [](const auto& attacker, const auto& defender){
         return attacker.damage>defender.armor ? attacker.damage-defender.armor : 1u;
    };

    auto playerDamage = getDamageOfAgainst(player, enemy);
    auto enemyDamage  = getDamageOfAgainst(enemy, player);
    auto roundsNeededToBeatEnemy  = std::ceil((double)enemy.hp/playerDamage);
    auto roundsNeededToBeatPlayer = std::ceil((double)player.hp/enemyDamage);

    return  cmp(roundsNeededToBeatEnemy, roundsNeededToBeatPlayer);
};

auto leastAmountOfGoldToWin = []<typename comparator = std::less_equal<>>
                        (const auto& enemy, const auto& store, const comparator cmp = std::less_equal<>()){
    auto minGold = cmp(UINT_MAX,0u) ? 0u : UINT_MAX;
    for(auto& weapon : store.weapons){
        for(auto& armor : store.armor){
            for(auto ring1=std::begin(store.rings); ring1!=std::end(store.rings); ring1++){
                for(auto ring2=ring1+1; ring2<std::end(store.rings); ring2++){
                    const auto [player, gold] = getStats(weapon, armor, *ring1, *ring2);
                    if(playerWinsFight(enemy, player, cmp) && cmp(gold,minGold)){
                        minGold = gold;
                    }
                }
            }
        }
    }
    return minGold;
};

int main(){
    const auto enemy = parseEnemyInput(readFile::vectorOfStrings("input1.txt"));
    const auto store = parseStoreInput(readFile::vectorOfStrings("input2.txt"));

    //Task 1
    auto gold = leastAmountOfGoldToWin(enemy, store);
    std::cout << "The player would win the fight with a minimum investment of "<< gold << " gold.\n";

    //Task 2
    gold = leastAmountOfGoldToWin(enemy, store, std::greater<>());
    std::cout << "The player would still lose the fight with a maximum investment of "<< gold << " gold.\n";
}