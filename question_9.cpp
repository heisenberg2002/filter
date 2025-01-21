#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>

using namespace std;

class ParcelRouter {
public:
    ParcelRouter(pair<int, int> office_a, pair<int, int> office_b, vector<pair<int, int>> collection_points)
        : office_a(office_a), office_b(office_b), collection_points(collection_points), n(collection_points.size()) {}

    int calculate_distance(pair<int, int> point1, pair<int, int> point2) {
        // Calculate Manhattan distance between two points
        return abs(point1.first - point2.first) + abs(point1.second - point2.second);
    }

    int calculate_route_fuel(pair<int, int> office, vector<pair<int, int>> points) {
        // Calculate total fuel consumption for a single truck route
        if (points.empty()) {
            return 0;
        }

        int fuel = 0;
        pair<int, int> current_pos = office;
        int parcels = 0;

        // Calculate fuel for each segment of the route
        for (auto point : points) {
            int distance = calculate_distance(current_pos, point);
            fuel += distance * (1 + parcels);  // Fuel consumption formula
            current_pos = point;
            parcels += 1;  // Add one parcel at each collection point
        }

        // Return trip to office
        int distance = calculate_distance(current_pos, office);
        fuel += distance * (1 + parcels);

        return fuel;
    }

    int find_minimum_fuel_route() {
        // Find the minimum total fuel consumption for both trucks
        int min_total_fuel = INT_MAX;

        // Try all possible combinations of points between the two trucks
        for (int i = 0; i <= n; ++i) {  // Number of points for truck A (including 0)
            vector<int> indices(n);
            fill(indices.begin(), indices.begin() + i, 1);
            do {
                // Points for truck A
                vector<pair<int, int>> points_a, points_b;
                for (int j = 0; j < n; ++j) {
                    if (indices[j]) {
                        points_a.push_back(collection_points[j]);
                    } else {
                        points_b.push_back(collection_points[j]);
                    }
                }

                // Try all possible orderings for truck A's points
                int min_fuel_a = points_a.empty() ? 0 : INT_MAX;
                int min_fuel_b = points_b.empty() ? 0 : INT_MAX;

                // Calculate minimum fuel for truck A's route
                if (!points_a.empty()) {
                    sort(points_a.begin(), points_a.end());
                    do {
                        int fuel = calculate_route_fuel(office_a, points_a);
                        min_fuel_a = min(min_fuel_a, fuel);
                    } while (next_permutation(points_a.begin(), points_a.end()));
                }

                // Calculate minimum fuel for truck B's route
                if (!points_b.empty()) {
                    sort(points_b.begin(), points_b.end());
                    do {
                        int fuel = calculate_route_fuel(office_b, points_b);
                        min_fuel_b = min(min_fuel_b, fuel);
                    } while (next_permutation(points_b.begin(), points_b.end()));
                }

                int total_fuel = min_fuel_a + min_fuel_b;
                min_total_fuel = min(min_total_fuel, total_fuel);

            } while (prev_permutation(indices.begin(), indices.end()));
        }

        return min_total_fuel;
    }

private:
    pair<int, int> office_a;
    pair<int, int> office_b;
    vector<pair<int, int>> collection_points;
    int n;
};

int main() {
    // Read number of collection points
    int N;
    cin >> N;

    // Read office locations
    int x, y;
    cin >> x >> y;
    pair<int, int> office_a = {x, y};
    cin >> x >> y;
    pair<int, int> office_b = {x, y};

    // Read collection points
    vector<pair<int, int>> collection_points(N);
    for (int i = 0; i < N; ++i) {
        cin >> x >> y;
        collection_points[i] = {x, y};
    }

    // Create router and find minimum fuel required
    ParcelRouter router(office_a, office_b, collection_points);
    int min_fuel = router.find_minimum_fuel_route();
    cout << min_fuel << endl;

    return 0;
}
