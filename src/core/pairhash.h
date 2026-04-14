#pragma once

#ifndef PAIR_H
#define PAIR_H

#include <utility>
#include <unordered_map>

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

#endif