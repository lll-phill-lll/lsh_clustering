#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "log.h"
#include "prediction.h"


class Intersector {
public:
    Intersector(int votes_percent): votes_percent(votes_percent) {};

    Prediction intersect(const std::vector<Prediction>& predictions, int objects_count) const {
        if (predictions.size() == 0) {
            L(linfo) << "No predictions";
            return {};
        }

        std::unordered_set<int> object_with_cluster;
        Prediction final_prediction;
        L(linfo) << "Intersection started";

        for (int i = 0; i != objects_count; ++i) {
            if (object_with_cluster.count(i)) {
                continue;
            }
            std::unordered_map<int, int> cluster_for_i_votes;
            for (const auto& prediction : predictions) {
                const auto cluster = prediction.get_cluster_for_object(i);
                for (int object : cluster) {
                    cluster_for_i_votes[object]++;
                }
            }
            std::unordered_set<int> cluster_for_i;
            int voters = predictions.size();
            for (const auto& [object, votes] : cluster_for_i_votes) {
                if (static_cast<double>(votes) * 100 / voters >= votes_percent) {
                    cluster_for_i.insert(object);
                    object_with_cluster.insert(object);
                }
            }

            final_prediction.add_cluster(cluster_for_i);
        }

        L(linfo) << "Intersection finished";

        return final_prediction;
    }

private:
    const double votes_percent;
};
