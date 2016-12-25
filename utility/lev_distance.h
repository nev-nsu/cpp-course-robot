//
// Created by nev on 12/25/16.
//

#ifndef ROBOT_LEV_DISTANCE_H
#define ROBOT_LEV_DISTANCE_H


#include <vector>

// copy-pasted from Wikipedia
template<class P>
int LevenshteinDistance(std::vector<P> s, std::vector<P> t) {
    // degenerate cases
    if (s == t) return 0;
    if (s.size() == 0) return (int) t.size();
    if (t.size() == 0) return (int) s.size();

    // create two work vectors of integer distances
    std::vector<int> v0(t.size() + 1);
    std::vector<int> v1(t.size() + 1);

    // initialize v0 (the previous row of distances)
    // this row is A[0][i]: edit distance for an empty s
    // the distance is just the number of characters to delete from t
    for (int i = 0; i < v0.size(); i++)
        v0[i] = i;

    for (int i = 0; i < s.size(); i++) {
        // calculate v1 (current row distances) from the previous row v0

        // first element of v1 is A[i+1][0]
        //   edit distance is delete (i+1) chars from s to match empty t
        v1[0] = i + 1;

        // use formula to fill in the rest of the row
        for (int j = 0; j < t.size(); j++) {
            int cost = (s[i] == t[j]) ? 0 : 1;
            v1[j + 1] = std::min(v1[j] + 1, std::min(v0[j + 1] + 1, v0[j] + cost));
        }

        // copy v1 (current row) to v0 (previous row) for next iteration
        for (int j = 0; j < v0.size(); j++)
            v0[j] = v1[j];
    }

    return v1[t.size()];
}


#endif //ROBOT_LEV_DISTANCE_H
