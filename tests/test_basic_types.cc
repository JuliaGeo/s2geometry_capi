#include <gtest/gtest.h>
#include <cmath>
#include "s2c.h"

// Test S2Point operations
TEST(S2Point, CreateAndDestroy) {
    S2CPoint* point = s2c_point_new(1.0, 0.0, 0.0);
    ASSERT_NE(point, nullptr);
    
    double x, y, z;
    s2c_point_get_xyz(point, &x, &y, &z);
    EXPECT_DOUBLE_EQ(x, 1.0);
    EXPECT_DOUBLE_EQ(y, 0.0);
    EXPECT_DOUBLE_EQ(z, 0.0);
    
    s2c_point_destroy(point);
}

TEST(S2Point, Normalize) {
    S2CPoint* point = s2c_point_new(3.0, 4.0, 0.0);
    S2CPoint* normalized = s2c_point_normalize(point);
    
    double x, y, z;
    s2c_point_get_xyz(normalized, &x, &y, &z);
    double norm = sqrt(x*x + y*y + z*z);
    EXPECT_NEAR(norm, 1.0, 1e-10);
    
    s2c_point_destroy(point);
    s2c_point_destroy(normalized);
}

TEST(S2Point, CrossProduct) {
    S2CPoint* a = s2c_point_new(1.0, 0.0, 0.0);
    S2CPoint* b = s2c_point_new(0.0, 1.0, 0.0);
    S2CPoint* cross = s2c_point_cross_prod(a, b);
    
    double x, y, z;
    s2c_point_get_xyz(cross, &x, &y, &z);
    EXPECT_NEAR(x, 0.0, 1e-10);
    EXPECT_NEAR(y, 0.0, 1e-10);
    EXPECT_NEAR(z, 1.0, 1e-10);
    
    s2c_point_destroy(a);
    s2c_point_destroy(b);
    s2c_point_destroy(cross);
}

// Test S2LatLng operations
TEST(S2LatLng, CreateFromDegrees) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    ASSERT_NE(latlng, nullptr);
    
    double lat_deg = s2c_latlng_lat_degrees(latlng);
    double lng_deg = s2c_latlng_lng_degrees(latlng);
    
    EXPECT_DOUBLE_EQ(lat_deg, 40.7128);
    EXPECT_DOUBLE_EQ(lng_deg, -74.0060);
    
    s2c_latlng_destroy(latlng);
}

TEST(S2LatLng, CreateFromRadians) {
    double lat_rad = M_PI / 4.0;  // 45 degrees
    double lng_rad = -M_PI / 2.0;  // -90 degrees
    
    S2CLatLng* latlng = s2c_latlng_from_radians(lat_rad, lng_rad);
    
    double lat_deg = s2c_latlng_lat_degrees(latlng);
    double lng_deg = s2c_latlng_lng_degrees(latlng);
    
    EXPECT_NEAR(lat_deg, 45.0, 1e-10);
    EXPECT_NEAR(lng_deg, -90.0, 1e-10);
    
    s2c_latlng_destroy(latlng);
}

TEST(S2LatLng, ToPoint) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(0.0, 0.0);
    S2CPoint* point = s2c_latlng_to_point(latlng);
    
    double x, y, z;
    s2c_point_get_xyz(point, &x, &y, &z);
    
    // At (0,0), the point should be (1,0,0)
    EXPECT_NEAR(x, 1.0, 1e-10);
    EXPECT_NEAR(y, 0.0, 1e-10);
    EXPECT_NEAR(z, 0.0, 1e-10);
    
    s2c_latlng_destroy(latlng);
    s2c_point_destroy(point);
}

// Test S1Angle operations
TEST(S1Angle, CreateFromDegrees) {
    S1CAngle* angle = s1c_angle_from_degrees(90.0);
    ASSERT_NE(angle, nullptr);
    
    double degrees = s1c_angle_degrees(angle);
    double radians = s1c_angle_radians(angle);
    
    EXPECT_DOUBLE_EQ(degrees, 90.0);
    EXPECT_NEAR(radians, M_PI / 2.0, 1e-10);
    
    s1c_angle_destroy(angle);
}

TEST(S1Angle, Normalize) {
    S1CAngle* angle = s1c_angle_from_degrees(370.0);
    S1CAngle* normalized = s1c_angle_normalized(angle);
    
    double degrees = s1c_angle_degrees(normalized);
    EXPECT_NEAR(degrees, 10.0, 1e-10);
    
    s1c_angle_destroy(angle);
    s1c_angle_destroy(normalized);
}

TEST(S1Angle, Arithmetic) {
    S1CAngle* angle1 = s1c_angle_from_degrees(30.0);
    S1CAngle* angle2 = s1c_angle_from_degrees(60.0);
    
    S1CAngle* sum = s1c_angle_add(angle1, angle2);
    EXPECT_DOUBLE_EQ(s1c_angle_degrees(sum), 90.0);
    
    S1CAngle* diff = s1c_angle_sub(angle1, angle2);
    EXPECT_DOUBLE_EQ(s1c_angle_degrees(diff), -30.0);
    
    S1CAngle* scaled = s1c_angle_mul(angle1, 2.0);
    EXPECT_DOUBLE_EQ(s1c_angle_degrees(scaled), 60.0);
    
    s1c_angle_destroy(angle1);
    s1c_angle_destroy(angle2);
    s1c_angle_destroy(sum);
    s1c_angle_destroy(diff);
    s1c_angle_destroy(scaled);
}

// Test S1ChordAngle operations
TEST(S1ChordAngle, FromAngle) {
    S1CAngle* angle = s1c_angle_from_degrees(60.0);
    S1CChordAngle* chord_angle = s1c_chord_angle_from_s1angle(angle);
    
    S1CAngle* back = s1c_chord_angle_to_s1angle(chord_angle);
    EXPECT_NEAR(s1c_angle_degrees(back), 60.0, 1e-10);
    
    s1c_angle_destroy(angle);
    s1c_angle_destroy(back);
    s1c_chord_angle_destroy(chord_angle);
}

// Test S2CellId operations
TEST(S2CellId, CreateFromLatLng) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
    
    ASSERT_NE(cell_id, nullptr);
    EXPECT_TRUE(s2c_cellid_is_valid(cell_id));
    
    int level = s2c_cellid_level(cell_id);
    EXPECT_EQ(level, 30);  // Maximum level
    
    s2c_latlng_destroy(latlng);
    s2c_cellid_destroy(cell_id);
}

TEST(S2CellId, ParentChild) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(0.0, 0.0);
    S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
    
    // Get parent at level 10
    S2CCellId* parent = s2c_cellid_parent(cell_id, 10);
    EXPECT_EQ(s2c_cellid_level(parent), 10);
    EXPECT_TRUE(s2c_cellid_contains(parent, cell_id));
    
    // Get children
    S2CCellId* children[4];
    s2c_cellid_get_all_neighbors(parent, children);
    
    for (int i = 0; i < 4; i++) {
        s2c_cellid_destroy(children[i]);
    }
    
    s2c_latlng_destroy(latlng);
    s2c_cellid_destroy(cell_id);
    s2c_cellid_destroy(parent);
}

// Test interval types
TEST(R1Interval, Basic) {
    R1CInterval* interval = r1c_interval_new_from_bounds(1.0, 3.0);
    
    EXPECT_DOUBLE_EQ(r1c_interval_lo(interval), 1.0);
    EXPECT_DOUBLE_EQ(r1c_interval_hi(interval), 3.0);
    EXPECT_DOUBLE_EQ(r1c_interval_get_center(interval), 2.0);
    EXPECT_DOUBLE_EQ(r1c_interval_get_length(interval), 2.0);
    
    EXPECT_TRUE(r1c_interval_contains(interval, 2.0));
    EXPECT_FALSE(r1c_interval_contains(interval, 4.0));
    
    r1c_interval_destroy(interval);
}

TEST(S1Interval, Basic) {
    S1CInterval* interval = s1c_interval_new_from_bounds(0.0, M_PI / 2.0);
    
    EXPECT_DOUBLE_EQ(s1c_interval_lo(interval), 0.0);
    EXPECT_DOUBLE_EQ(s1c_interval_hi(interval), M_PI / 2.0);
    EXPECT_DOUBLE_EQ(s1c_interval_get_length(interval), M_PI / 2.0);
    
    EXPECT_TRUE(s1c_interval_contains(interval, M_PI / 4.0));
    EXPECT_FALSE(s1c_interval_contains(interval, M_PI));
    
    s1c_interval_destroy(interval);
}

// Test S2Cap operations
TEST(S2Cap, CreateFromCenterHeight) {
    S2CPoint* center = s2c_point_new(1.0, 0.0, 0.0);
    S2CCap* cap = s2c_cap_from_center_height(center, 0.5);
    
    EXPECT_TRUE(s2c_cap_contains_point(cap, center));
    EXPECT_FALSE(s2c_cap_is_empty(cap));
    EXPECT_FALSE(s2c_cap_is_full(cap));
    
    double area = s2c_cap_area(cap);
    EXPECT_GT(area, 0.0);
    EXPECT_LT(area, 4 * M_PI);  // Less than full sphere
    
    s2c_point_destroy(center);
    s2c_cap_destroy(cap);
}

TEST(S2Cap, CreateFromCenterArea) {
    S2CPoint* center = s2c_point_new(0.0, 0.0, 1.0);
    double area = M_PI;  // Quarter of sphere
    S2CCap* cap = s2c_cap_from_center_area(center, area);
    
    EXPECT_NEAR(s2c_cap_area(cap), area, 1e-10);
    
    s2c_point_destroy(center);
    s2c_cap_destroy(cap);
}

// Test S2Cell operations
TEST(S2Cell, CreateFromCellId) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.0, -74.0);
    S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
    S2CCellId* parent_id = s2c_cellid_parent(cell_id, 10);
    
    S2CCell* cell = s2c_cell_new(parent_id);
    ASSERT_NE(cell, nullptr);
    
    // Get cell center
    S2CPoint* center = s2c_cell_get_center(cell);
    ASSERT_NE(center, nullptr);
    
    // Get vertices
    S2CPoint* vertices[4];
    for (int i = 0; i < 4; i++) {
        vertices[i] = s2c_cell_get_vertex(cell, i);
        ASSERT_NE(vertices[i], nullptr);
    }
    
    // Clean up
    for (int i = 0; i < 4; i++) {
        s2c_point_destroy(vertices[i]);
    }
    s2c_point_destroy(center);
    s2c_cell_destroy(cell);
    s2c_cellid_destroy(cell_id);
    s2c_cellid_destroy(parent_id);
    s2c_latlng_destroy(latlng);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}