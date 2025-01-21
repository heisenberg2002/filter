from itertools import combinations, permutations
import sys
from typing import List, Tuple

class ParcelRouter:
    def __init__(self, office_a: Tuple[int, int], office_b: Tuple[int, int], collection_points: List[Tuple[int, int]]):
        self.office_a = office_a
        self.office_b = office_b
        self.collection_points = collection_points
        self.n = len(collection_points)

    def calculate_distance(self, point1: Tuple[int, int], point2: Tuple[int, int]) -> int:
        """Calculate Manhattan distance between two points"""
        return abs(point1[0] - point2[0]) + abs(point1[1] - point2[1])

    def calculate_route_fuel(self, office: Tuple[int, int], points: List[Tuple[int, int]]) -> int:
        """Calculate total fuel consumption for a single truck route"""
        if not points:
            return 0

        fuel = 0
        current_pos = office
        parcels = 0

        # Calculate fuel for each segment of the route
        for point in points:
            distance = self.calculate_distance(current_pos, point)
            fuel += distance * (1 + parcels)  # Fuel consumption formula
            current_pos = point
            parcels += 1  # Add one parcel at each collection point

        # Return trip to office
        distance = self.calculate_distance(current_pos, office)
        fuel += distance * (1 + parcels)

        return fuel

    def find_minimum_fuel_route(self) -> int:
        """Find the minimum total fuel consumption for both trucks"""
        min_total_fuel = sys.maxsize

        # Try all possible combinations of points between the two trucks
        for i in range(self.n + 1):  # Number of points for truck A (including 0)
            for combo in combinations(range(self.n), i):
                # Points for truck A
                points_a = [self.collection_points[idx] for idx in combo]
                # Remaining points go to truck B
                points_b = [self.collection_points[idx] for idx in range(self.n) if idx not in combo]

                # Try all possible orderings for truck A's points
                min_fuel_a = sys.maxsize if points_a else 0
                min_fuel_b = sys.maxsize if points_b else 0

                # Calculate minimum fuel for truck A's route
                if points_a:
                    for route in permutations(points_a):
                        fuel = self.calculate_route_fuel(self.office_a, route)
                        min_fuel_a = min(min_fuel_a, fuel)

                # Calculate minimum fuel for truck B's route
                if points_b:
                    for route in permutations(points_b):
                        fuel = self.calculate_route_fuel(self.office_b, route)
                        min_fuel_b = min(min_fuel_b, fuel)

                total_fuel = min_fuel_a + min_fuel_b
                min_total_fuel = min(min_total_fuel, total_fuel)

        return min_total_fuel

def main():
    # Read number of collection points
    N = int(input())

    # Read office locations
    x, y = map(int, input().split())
    office_a = (x, y)
    x, y = map(int, input().split())
    office_b = (x, y)

    # Read collection points
    collection_points = []
    for _ in range(N):
        x, y = map(int, input().split())
        collection_points.append((x, y))

    # Create router and find minimum fuel required
    router = ParcelRouter(office_a, office_b, collection_points)
    min_fuel = router.find_minimum_fuel_route()
    print(min_fuel)

if __name__ == "__main__":
    main()
