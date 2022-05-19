#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

#include "log.h"
#include "data.h"

class Prediction {

public:
    void add(const std::string& hash, int object) {
        // if we already have hash
        if (hash_to_cluster.count(hash)) {
            int cluster_idx = hash_to_cluster[hash];
            object_to_cluster[object] = cluster_idx;
            clusters[cluster_idx].insert(object);
            return;
        }

        std::unordered_set<int> new_cluster = {object};
        clusters.push_back(new_cluster);

        int cluster_idx = clusters.size() - 1;
        object_to_cluster[object] = cluster_idx;
        hash_to_cluster[hash] = cluster_idx;
    }

    std::unordered_set<int> get_cluster_for_object(int object) {
        assert(object_to_cluster.count(object) > 0);

        int cluster_idx = object_to_cluster[object];

        return clusters[cluster_idx];

    }

    std::string get_log() const {
        std::stringstream buffer;

        for (const auto& [hash, cluster_idx] : hash_to_cluster) {
            buffer << "\n\t" << hash;
            for (int object : clusters[cluster_idx]) {
                buffer << "\n\t\t" << object << ": " << data[object];
            }
        }
        return buffer.str();

    }


private:
    std::unordered_map<int, int> object_to_cluster;
    std::vector<std::unordered_set<int>> clusters;

    std::unordered_map<std::string, int> hash_to_cluster;

};
