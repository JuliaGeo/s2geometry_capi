#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "s2c.h"

// Helper function to create a simple triangle loop
std::vector<S2CPoint*> CreateTriangle() {
    std::vector<S2CPoint*> points;
    
    // Create an equilateral triangle around (0, 0)
    S2CLatLng* latlng1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* latlng2 = s2c_latlng_from_degrees(0.0, 1.0);
    S2CLatLng* latlng3 = s2c_latlng_from_degrees(1.0, 0.0);
    
    points.push_back(s2c_latlng_to_point(latlng1));
    points.push_back(s2c_latlng_to_point(latlng2));
    points.push_back(s2c_latlng_to_point(latlng3));
    
    s2c_latlng_destroy(latlng1);
    s2c_latlng_destroy(latlng2);
    s2c_latlng_destroy(latlng3);
    
    return points;
}

// Test S2Loop operations
TEST(S2Loop, CreateFromPoints) {
    std::vector<S2CPoint*> points = CreateTriangle();
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    
    ASSERT_NE(loop, nullptr);
    EXPECT_EQ(s2c_loop_num_vertices(loop), 3);
    EXPECT_TRUE(s2c_loop_is_valid(loop));
    
    // Clean up
    s2c_loop_destroy(loop);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

TEST(S2Loop, CreateEmpty) {
    S2CLoop* loop = s2c_loop_new_empty();
    ASSERT_NE(loop, nullptr);
    EXPECT_TRUE(s2c_loop_is_empty(loop));
    EXPECT_EQ(s2c_loop_num_vertices(loop), 0);
    s2c_loop_destroy(loop);
}

TEST(S2Loop, CreateFull) {
    S2CLoop* loop = s2c_loop_new_full();
    ASSERT_NE(loop, nullptr);
    EXPECT_TRUE(s2c_loop_is_full(loop));
    EXPECT_FALSE(s2c_loop_is_empty(loop));
    s2c_loop_destroy(loop);
}

TEST(S2Loop, GetVertex) {
    std::vector<S2CPoint*> points = CreateTriangle();
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    
    for (int i = 0; i < 3; i++) {
        S2CPoint* vertex = s2c_loop_get_vertex(loop, i);
        ASSERT_NE(vertex, nullptr);
        
        double x1, y1, z1, x2, y2, z2;
        s2c_point_get_xyz(points[i], &x1, &y1, &z1);
        s2c_point_get_xyz(vertex, &x2, &y2, &z2);
        
        EXPECT_NEAR(x1, x2, 1e-10);
        EXPECT_NEAR(y1, y2, 1e-10);
        EXPECT_NEAR(z1, z2, 1e-10);
        
        s2c_point_destroy(vertex);
    }
    
    s2c_loop_destroy(loop);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

TEST(S2Loop, ContainsPoint) {
    std::vector<S2CPoint*> points = CreateTriangle();
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    
    // Test center point (should be inside)
    S2CLatLng* center_latlng = s2c_latlng_from_degrees(0.3, 0.3);
    S2CPoint* center = s2c_latlng_to_point(center_latlng);
    EXPECT_TRUE(s2c_loop_contains_point(loop, center));
    
    // Test outside point
    S2CLatLng* outside_latlng = s2c_latlng_from_degrees(10.0, 10.0);
    S2CPoint* outside = s2c_latlng_to_point(outside_latlng);
    EXPECT_FALSE(s2c_loop_contains_point(loop, outside));
    
    s2c_latlng_destroy(center_latlng);
    s2c_latlng_destroy(outside_latlng);
    s2c_point_destroy(center);
    s2c_point_destroy(outside);
    s2c_loop_destroy(loop);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

// Test S2Polyline operations
TEST(S2Polyline, CreateFromPoints) {
    std::vector<S2CPoint*> points;
    
    // Create a simple line
    for (double lat = 0.0; lat <= 10.0; lat += 2.0) {
        S2CLatLng* latlng = s2c_latlng_from_degrees(lat, 0.0);
        points.push_back(s2c_latlng_to_point(latlng));
        s2c_latlng_destroy(latlng);
    }
    
    S2CPolyline* polyline = s2c_polyline_new_from_points(points.data(), points.size());
    ASSERT_NE(polyline, nullptr);
    EXPECT_EQ(s2c_polyline_num_vertices(polyline), 6);
    
    // Test length
    double length = s2c_polyline_get_length(polyline);
    EXPECT_GT(length, 0.0);
    
    // Test interpolation
    S2CPoint* midpoint = s2c_polyline_interpolate(polyline, 0.5);
    ASSERT_NE(midpoint, nullptr);
    s2c_point_destroy(midpoint);
    
    // Test project
    S2CLatLng* test_latlng = s2c_latlng_from_degrees(5.0, 1.0);
    S2CPoint* test_point = s2c_latlng_to_point(test_latlng);
    int next_vertex;
    S2CPoint* projected = s2c_polyline_project(polyline, test_point, &next_vertex);
    ASSERT_NE(projected, nullptr);
    EXPECT_GE(next_vertex, 0);
    EXPECT_LT(next_vertex, 6);
    
    s2c_latlng_destroy(test_latlng);
    s2c_point_destroy(test_point);
    s2c_point_destroy(projected);
    s2c_polyline_destroy(polyline);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

TEST(S2Polyline, Subsample) {
    std::vector<S2CPoint*> points;
    
    // Create a longer line
    for (double lat = 0.0; lat <= 20.0; lat += 1.0) {
        S2CLatLng* latlng = s2c_latlng_from_degrees(lat, 0.0);
        points.push_back(s2c_latlng_to_point(latlng));
        s2c_latlng_destroy(latlng);
    }
    
    S2CPolyline* polyline = s2c_polyline_new_from_points(points.data(), points.size());
    
    // Subsample
    S1CAngle* tolerance = s1c_angle_from_degrees(2.0);
    S2CPolyline* subsampled = s2c_polyline_subsample_vertices(polyline, tolerance);
    ASSERT_NE(subsampled, nullptr);
    EXPECT_LT(s2c_polyline_num_vertices(subsampled), s2c_polyline_num_vertices(polyline));
    
    s1c_angle_destroy(tolerance);
    s2c_polyline_destroy(subsampled);
    s2c_polyline_destroy(polyline);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

// Test S2Polygon operations
TEST(S2Polygon, CreateFromLoop) {
    std::vector<S2CPoint*> points = CreateTriangle();
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    ASSERT_NE(polygon, nullptr);
    EXPECT_FALSE(s2c_polygon_is_empty(polygon));
    EXPECT_EQ(s2c_polygon_num_loops(polygon), 1);
    EXPECT_TRUE(s2c_polygon_is_valid(polygon));
    
    // Test area
    double area = s2c_polygon_get_area(polygon);
    EXPECT_GT(area, 0.0);
    EXPECT_LT(area, 4 * M_PI);  // Less than full sphere
    
    s2c_polygon_destroy(polygon);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

TEST(S2Polygon, CreateEmpty) {
    S2CPolygon* polygon = s2c_polygon_new_empty();
    ASSERT_NE(polygon, nullptr);
    EXPECT_TRUE(s2c_polygon_is_empty(polygon));
    EXPECT_EQ(s2c_polygon_num_loops(polygon), 0);
    EXPECT_DOUBLE_EQ(s2c_polygon_get_area(polygon), 0.0);
    s2c_polygon_destroy(polygon);
}

TEST(S2Polygon, CreateFull) {
    S2CPolygon* polygon = s2c_polygon_new_full();
    ASSERT_NE(polygon, nullptr);
    EXPECT_TRUE(s2c_polygon_is_full(polygon));
    EXPECT_FALSE(s2c_polygon_is_empty(polygon));
    EXPECT_NEAR(s2c_polygon_get_area(polygon), 4 * M_PI, 1e-10);
    s2c_polygon_destroy(polygon);
}

TEST(S2Polygon, ContainsPoint) {
    std::vector<S2CPoint*> points = CreateTriangle();
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    // Test center point (should be inside)
    S2CLatLng* center_latlng = s2c_latlng_from_degrees(0.3, 0.3);
    S2CPoint* center = s2c_latlng_to_point(center_latlng);
    EXPECT_TRUE(s2c_polygon_contains_point(polygon, center));
    
    // Test outside point
    S2CLatLng* outside_latlng = s2c_latlng_from_degrees(10.0, 10.0);
    S2CPoint* outside = s2c_latlng_to_point(outside_latlng);
    EXPECT_FALSE(s2c_polygon_contains_point(polygon, outside));
    
    s2c_latlng_destroy(center_latlng);
    s2c_latlng_destroy(outside_latlng);
    s2c_point_destroy(center);
    s2c_point_destroy(outside);
    s2c_polygon_destroy(polygon);
    for (auto* point : points) {
        s2c_point_destroy(point);
    }
}

TEST(S2Polygon, IntersectsPolygon) {
    // Create two overlapping triangles
    std::vector<S2CPoint*> points1 = CreateTriangle();
    S2CLoop* loop1 = s2c_loop_new_from_points(points1.data(), points1.size());
    S2CPolygon* polygon1 = s2c_polygon_new_from_loop(loop1);
    
    // Create second triangle offset by 0.5 degrees
    std::vector<S2CPoint*> points2;
    S2CLatLng* latlng1 = s2c_latlng_from_degrees(0.5, 0.5);
    S2CLatLng* latlng2 = s2c_latlng_from_degrees(0.5, 1.5);
    S2CLatLng* latlng3 = s2c_latlng_from_degrees(1.5, 0.5);
    
    points2.push_back(s2c_latlng_to_point(latlng1));
    points2.push_back(s2c_latlng_to_point(latlng2));
    points2.push_back(s2c_latlng_to_point(latlng3));
    
    S2CLoop* loop2 = s2c_loop_new_from_points(points2.data(), points2.size());
    S2CPolygon* polygon2 = s2c_polygon_new_from_loop(loop2);
    
    // They should intersect
    EXPECT_TRUE(s2c_polygon_intersects(polygon1, polygon2));
    
    // Create a distant polygon that doesn't intersect
    std::vector<S2CPoint*> points3;
    S2CLatLng* latlng4 = s2c_latlng_from_degrees(50.0, 50.0);
    S2CLatLng* latlng5 = s2c_latlng_from_degrees(50.0, 51.0);
    S2CLatLng* latlng6 = s2c_latlng_from_degrees(51.0, 50.0);
    
    points3.push_back(s2c_latlng_to_point(latlng4));
    points3.push_back(s2c_latlng_to_point(latlng5));
    points3.push_back(s2c_latlng_to_point(latlng6));
    
    S2CLoop* loop3 = s2c_loop_new_from_points(points3.data(), points3.size());
    S2CPolygon* polygon3 = s2c_polygon_new_from_loop(loop3);
    
    EXPECT_FALSE(s2c_polygon_intersects(polygon1, polygon3));
    
    // Clean up
    s2c_latlng_destroy(latlng1);
    s2c_latlng_destroy(latlng2);
    s2c_latlng_destroy(latlng3);
    s2c_latlng_destroy(latlng4);
    s2c_latlng_destroy(latlng5);
    s2c_latlng_destroy(latlng6);
    
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(polygon3);
    
    for (auto* point : points1) s2c_point_destroy(point);
    for (auto* point : points2) s2c_point_destroy(point);
    for (auto* point : points3) s2c_point_destroy(point);
}

TEST(S2Polygon, Union) {
    // Create two adjacent squares
    std::vector<S2CPoint*> square1_points;
    S2CLatLng* sq1_ll1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* sq1_ll2 = s2c_latlng_from_degrees(0.0, 1.0);
    S2CLatLng* sq1_ll3 = s2c_latlng_from_degrees(1.0, 1.0);
    S2CLatLng* sq1_ll4 = s2c_latlng_from_degrees(1.0, 0.0);
    
    square1_points.push_back(s2c_latlng_to_point(sq1_ll1));
    square1_points.push_back(s2c_latlng_to_point(sq1_ll2));
    square1_points.push_back(s2c_latlng_to_point(sq1_ll3));
    square1_points.push_back(s2c_latlng_to_point(sq1_ll4));
    
    S2CLoop* loop1 = s2c_loop_new_from_points(square1_points.data(), square1_points.size());
    S2CPolygon* polygon1 = s2c_polygon_new_from_loop(loop1);
    
    std::vector<S2CPoint*> square2_points;
    S2CLatLng* sq2_ll1 = s2c_latlng_from_degrees(1.0, 0.0);
    S2CLatLng* sq2_ll2 = s2c_latlng_from_degrees(1.0, 1.0);
    S2CLatLng* sq2_ll3 = s2c_latlng_from_degrees(2.0, 1.0);
    S2CLatLng* sq2_ll4 = s2c_latlng_from_degrees(2.0, 0.0);
    
    square2_points.push_back(s2c_latlng_to_point(sq2_ll1));
    square2_points.push_back(s2c_latlng_to_point(sq2_ll2));
    square2_points.push_back(s2c_latlng_to_point(sq2_ll3));
    square2_points.push_back(s2c_latlng_to_point(sq2_ll4));
    
    S2CLoop* loop2 = s2c_loop_new_from_points(square2_points.data(), square2_points.size());
    S2CPolygon* polygon2 = s2c_polygon_new_from_loop(loop2);
    
    S2CPolygon* union_polygon = s2c_polygon_union(polygon1, polygon2);
    ASSERT_NE(union_polygon, nullptr);
    
    // Union should have larger area than either individual polygon
    double area1 = s2c_polygon_get_area(polygon1);
    double area2 = s2c_polygon_get_area(polygon2);
    double union_area = s2c_polygon_get_area(union_polygon);
    
    EXPECT_GT(union_area, area1);
    EXPECT_GT(union_area, area2);
    EXPECT_LE(union_area, area1 + area2);  // Less than or equal due to shared edge
    
    // Clean up
    s2c_latlng_destroy(sq1_ll1);
    s2c_latlng_destroy(sq1_ll2);
    s2c_latlng_destroy(sq1_ll3);
    s2c_latlng_destroy(sq1_ll4);
    s2c_latlng_destroy(sq2_ll1);
    s2c_latlng_destroy(sq2_ll2);
    s2c_latlng_destroy(sq2_ll3);
    s2c_latlng_destroy(sq2_ll4);
    
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(union_polygon);
    
    for (auto* point : square1_points) s2c_point_destroy(point);
    for (auto* point : square2_points) s2c_point_destroy(point);
}

// Test S2LatLngRect operations
TEST(S2LatLngRect, CreateFromCenterSize) {
    S2CLatLng* center = s2c_latlng_from_degrees(40.0, -74.0);
    S2CLatLng* size = s2c_latlng_from_degrees(2.0, 2.0);
    
    S2CLatLngRect* rect = s2c_latlngrect_from_center_size(center, size);
    ASSERT_NE(rect, nullptr);
    
    EXPECT_FALSE(s2c_latlngrect_is_empty(rect));
    EXPECT_FALSE(s2c_latlngrect_is_full(rect));
    EXPECT_TRUE(s2c_latlngrect_is_valid(rect));
    
    // Test contains point
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(rect, center));
    
    // Test corner
    S2CLatLng* corner = s2c_latlng_from_degrees(39.0, -75.0);
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(rect, corner));
    
    // Test outside point
    S2CLatLng* outside = s2c_latlng_from_degrees(50.0, -80.0);
    EXPECT_FALSE(s2c_latlngrect_contains_latlng(rect, outside));
    
    s2c_latlng_destroy(center);
    s2c_latlng_destroy(size);
    s2c_latlng_destroy(corner);
    s2c_latlng_destroy(outside);
    s2c_latlngrect_destroy(rect);
}

TEST(S2LatLngRect, Union) {
    S2CLatLng* p1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* p2 = s2c_latlng_from_degrees(10.0, 10.0);
    S2CLatLng* p3 = s2c_latlng_from_degrees(20.0, 20.0);
    S2CLatLng* p4 = s2c_latlng_from_degrees(30.0, 30.0);
    
    S2CLatLngRect* rect1 = s2c_latlngrect_from_point_pair(p1, p2);
    S2CLatLngRect* rect2 = s2c_latlngrect_from_point_pair(p3, p4);
    
    S2CLatLngRect* union_rect = s2c_latlngrect_union(rect1, rect2);
    ASSERT_NE(union_rect, nullptr);
    
    // Union should contain all points
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(union_rect, p1));
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(union_rect, p2));
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(union_rect, p3));
    EXPECT_TRUE(s2c_latlngrect_contains_latlng(union_rect, p4));
    
    s2c_latlng_destroy(p1);
    s2c_latlng_destroy(p2);
    s2c_latlng_destroy(p3);
    s2c_latlng_destroy(p4);
    s2c_latlngrect_destroy(rect1);
    s2c_latlngrect_destroy(rect2);
    s2c_latlngrect_destroy(union_rect);
}

// Test S2CellUnion operations
TEST(S2CellUnion, CreateFromCellIds) {
    std::vector<S2CCellId*> cell_ids;
    
    // Create a few cell IDs
    for (double lat = 0.0; lat < 10.0; lat += 5.0) {
        for (double lng = 0.0; lng < 10.0; lng += 5.0) {
            S2CLatLng* latlng = s2c_latlng_from_degrees(lat, lng);
            S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
            S2CCellId* parent = s2c_cellid_parent(cell_id, 10);
            cell_ids.push_back(parent);
            s2c_latlng_destroy(latlng);
            s2c_cellid_destroy(cell_id);
        }
    }
    
    S2CCellUnion* cell_union = s2c_cellunion_new(cell_ids.data(), cell_ids.size());
    ASSERT_NE(cell_union, nullptr);
    EXPECT_GT(s2c_cellunion_num_cells(cell_union), 0);
    
    // Test contains
    S2CLatLng* test_latlng = s2c_latlng_from_degrees(2.5, 2.5);
    S2CCellId* test_cell_id = s2c_cellid_from_latlng(test_latlng);
    EXPECT_TRUE(s2c_cellunion_contains_cellid(cell_union, test_cell_id));
    
    s2c_latlng_destroy(test_latlng);
    s2c_cellid_destroy(test_cell_id);
    s2c_cellunion_destroy(cell_union);
    
    for (auto* cell_id : cell_ids) {
        s2c_cellid_destroy(cell_id);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}