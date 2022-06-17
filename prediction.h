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

    void add_cluster(const std::unordered_set<int>& cluster) {
        clusters.push_back(cluster);
        int cluster_idx = clusters.size() - 1;
        for (int object : cluster) {
            object_to_cluster[object] = cluster_idx;
        }
    }

    std::unordered_set<int> get_cluster_for_object(int object) const {
        assert(object_to_cluster.count(object) > 0);

        const auto cluster_idx = object_to_cluster.at(object);

        return clusters[cluster_idx];
    }

    std::string get_log() const {
        std::stringstream buffer;

        if (!hash_to_cluster.empty()) {
            for (const auto& [hash, cluster_idx] : hash_to_cluster) {
                buffer << "\n\t" << hash;
                for (int object : clusters[cluster_idx]) {
                    buffer << "\n\t\t" << object << ": " << (*data)[object];
                }
            }
        } else {
            int i = 0;
            for (const auto& cluster : clusters) {
                // buffer << "\ncluster " << i << ":";
                if (i != 0) {
                    buffer << "\n";
                }

                int j = 0;
                for (int object : cluster) {
                    if (j != 0) {
                        buffer << ",";
                    }
                    ++j;
                    buffer << object+1; //  << ": " << (*data)[object];
                }
                ++i;
            }

        }
        return buffer.str();
    }

    int get_objects_count() const {
        return object_to_cluster.size();
    }

    int get_clusters_num() const {
        return clusters.size();
    }

    std::unordered_set<int> get_next_clusters() const {
        if (next_cluster < clusters.size()) {
            return clusters[next_cluster++];
        }
        return {};
    }

    void reset_next_cluster() {
        next_cluster = 0;
    }


private:
    mutable int next_cluster = 0;
    std::unordered_map<int, int> object_to_cluster;
    std::vector<std::unordered_set<int>> clusters;

    std::unordered_map<std::string, int> hash_to_cluster;
};
