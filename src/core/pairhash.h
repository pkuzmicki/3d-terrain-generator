#pragma once

#ifndef PAIR_H
#define PAIR_H

#include <utility>
#include <unordered_map>

struct PairHash {
    size_t operator()(const std::pair<int,int>& p) const {
        //return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 32);
        size_t h1 = std::hash<int>()(p.first);
        size_t h2 = std::hash<int>()(p.second);
        return h1 ^ (h2 * 2654435761u + 0x9e3779b9u + (h1 << 6) + (h1 >> 2));
    }
};

#endif