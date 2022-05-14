#include "log.h"

struct Runner {
    CGK<ACGT> cgk;
    int word_len;
    int z, m;
    int id;
    std::vector<F> fs;
    std::vector<std::unordered_map<std::string, std::unordered_set<int>>> clusters;

    Runner(int _word_len, int _z, int _m) {
        z = _z;
        m = _m;
        word_len = _word_len;
        id = std::rand() % 1000;

        cgk = CGK<ACGT>(word_len, id);
        LN(linfo, id) << "Init runner:\n\tz: " << z << ", m: " << m << ", word_size: " << word_len;

        // initialize hash functions
        for (int f_num = 0; f_num != z; ++f_num) {
            std::vector<H> hs;
            for (int h_num = 0; h_num != m; ++h_num) {
                hs.push_back(H(rand() % (word_len * 3)));
            }
            fs.push_back(hs);
        }
        clusters.resize(z);
    }

    void run(int index) {
        const std::string emb = cgk.compute(data[index]);
        for (int i = 0; i != z; ++i) {
            const std::string hash_comp = fs[i].get_hash_composition(emb);

            if (clusters[i].count(hash_comp)) {
                clusters[i][hash_comp].insert(index);
            } else {
                clusters[i][hash_comp] = {index};
            }
        }

    }

    int compare_clusters(const std::unordered_set<int>& l, const std::unordered_set<int>& r) const {
        int count = 0;
        for (int elem : l) {
            count += r.count(elem);
        }

        return count;
    }

    std::unordered_set<int> intersect_clusters(const std::unordered_set<int>& l, const std::unordered_set<int>& r) const {

        std::unordered_set<int> intersection;

        for (int elem : l) {
            if (r.count(elem)) {
                intersection.insert(elem);
            }
        }

        return intersection;
    }

    std::vector<std::unordered_set<int>> get_clusters() const {

        std::vector<std::unordered_set<int>> result_clusters;

        int allowed_cluster_distance = 2;
        for (int i = 0; i != clusters.size(); ++i) {
            for (const auto& cluster : clusters[i]) {

                std::unordered_set<int> current_cluster = cluster.second;

                for (int j = 0; j != clusters.size(); ++j) {
                    if (i == j) continue;

                    std::unordered_set<int> closest_cluster;
                    int min_distance = -1;

                    for (const auto& inner_cluster : clusters[j]) {
                        int distance = compare_clusters(current_cluster, inner_cluster.second);
                        if (min_distance == -1) {
                            closest_cluster = inner_cluster.second;
                            min_distance = distance;
                        } else if (distance < min_distance) {
                            closest_cluster = inner_cluster.second;
                            min_distance = distance;
                        }
                    }

                    if (min_distance != -1 && min_distance < allowed_cluster_distance) {
                        current_cluster = intersect_clusters(current_cluster, closest_cluster);
                    }

                }

                result_clusters.push_back(current_cluster);

            }

        }


        return result_clusters;
    }


};
