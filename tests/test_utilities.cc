#include <gtest/gtest.h>
#include <cmath>
#include "s2c.h"

// Test S2Earth utility functions
TEST(S2Earth, RadiusConstants) {
    double radius_km = s2c_earth_radius_km();
    double radius_m = s2c_earth_radius_meters();
    
    EXPECT_NEAR(radius_km, 6371.01, 0.01);
    EXPECT_NEAR(radius_m, 6371010.0, 1.0);
    EXPECT_DOUBLE_EQ(radius_m, radius_km * 1000.0);
}

TEST(S2Earth, DistanceConversions) {
    // Test km to radians and back
    double km = 100.0;
    double radians = s2c_earth_km_to_radians(km);
    double km_back = s2c_earth_radians_to_km(radians);
    EXPECT_NEAR(km, km_back, 1e-10);
    
    // Test meters to radians and back
    double meters = 1000.0;
    radians = s2c_earth_meters_to_radians(meters);
    double meters_back = s2c_earth_radians_to_meters(radians);
    EXPECT_NEAR(meters, meters_back, 1e-10);
}

TEST(S2Earth, AreaConversions) {
    // Test square km to steradians and back
    double km2 = 1000.0;
    double steradians = s2c_earth_square_km_to_steradians(km2);
    double km2_back = s2c_earth_steradians_to_square_km(steradians);
    EXPECT_NEAR(km2, km2_back, 1e-10);
    
    // Test square meters to steradians and back
    double m2 = 1000000.0;  // 1 square km
    steradians = s2c_earth_square_meters_to_steradians(m2);
    double m2_back = s2c_earth_steradians_to_square_meters(steradians);
    EXPECT_NEAR(m2, m2_back, 1e-10);
}

TEST(S2Earth, DistanceBetweenPoints) {
    // New York to London (approximate)
    S2CLatLng* ny = s2c_latlng_from_degrees(40.7128, -74.0060);
    S2CLatLng* london = s2c_latlng_from_degrees(51.5074, -0.1278);
    
    S1CAngle* angle_dist = s2c_earth_get_distance_latlng(ny, london);
    double km_dist = s2c_earth_get_distance_km_latlng(ny, london);
    double m_dist = s2c_earth_get_distance_meters_latlng(ny, london);
    
    // Approximate distance should be around 5570 km
    EXPECT_GT(km_dist, 5000.0);
    EXPECT_LT(km_dist, 6000.0);
    EXPECT_DOUBLE_EQ(m_dist, km_dist * 1000.0);
    
    // Test with points
    S2CPoint* ny_point = s2c_latlng_to_point(ny);
    S2CPoint* london_point = s2c_latlng_to_point(london);
    
    double km_dist_points = s2c_earth_get_distance_km_point(ny_point, london_point);
    EXPECT_NEAR(km_dist, km_dist_points, 1e-10);
    
    s1c_angle_destroy(angle_dist);
    s2c_latlng_destroy(ny);
    s2c_latlng_destroy(london);
    s2c_point_destroy(ny_point);
    s2c_point_destroy(london_point);
}

TEST(S2Earth, InitialBearing) {
    // Test bearing from equator to north
    S2CLatLng* start = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* north = s2c_latlng_from_degrees(10.0, 0.0);
    
    S1CAngle* bearing = s2c_earth_get_initial_bearing(start, north);
    double bearing_deg = s1c_angle_degrees(bearing);
    
    // Should be approximately 0 degrees (north)
    EXPECT_NEAR(bearing_deg, 0.0, 1.0);
    
    // Test bearing east
    S2CLatLng* east = s2c_latlng_from_degrees(0.0, 10.0);
    S1CAngle* bearing_east = s2c_earth_get_initial_bearing(start, east);
    double bearing_east_deg = s1c_angle_degrees(bearing_east);
    
    // Should be approximately 90 degrees (east)
    EXPECT_NEAR(bearing_east_deg, 90.0, 1.0);
    
    s2c_latlng_destroy(start);
    s2c_latlng_destroy(north);
    s2c_latlng_destroy(east);
    s1c_angle_destroy(bearing);
    s1c_angle_destroy(bearing_east);
}

// Test predicates
TEST(Predicates, CrossingSign) {
    // Create four points forming two crossing line segments
    S2CLatLng* a0_ll = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* a1_ll = s2c_latlng_from_degrees(2.0, 2.0);
    S2CLatLng* b0_ll = s2c_latlng_from_degrees(0.0, 2.0);
    S2CLatLng* b1_ll = s2c_latlng_from_degrees(2.0, 0.0);
    
    S2CPoint* a0 = s2c_latlng_to_point(a0_ll);
    S2CPoint* a1 = s2c_latlng_to_point(a1_ll);
    S2CPoint* b0 = s2c_latlng_to_point(b0_ll);
    S2CPoint* b1 = s2c_latlng_to_point(b1_ll);
    
    int sign = s2c_crossing_sign(a0, a1, b0, b1);
    EXPECT_GT(sign, 0);  // Should have positive crossing
    
    // Test non-crossing segments
    S2CLatLng* c0_ll = s2c_latlng_from_degrees(10.0, 10.0);
    S2CLatLng* c1_ll = s2c_latlng_from_degrees(12.0, 12.0);
    S2CPoint* c0 = s2c_latlng_to_point(c0_ll);
    S2CPoint* c1 = s2c_latlng_to_point(c1_ll);
    
    sign = s2c_crossing_sign(a0, a1, c0, c1);
    EXPECT_EQ(sign, 0);  // Should not cross
    
    s2c_latlng_destroy(a0_ll);
    s2c_latlng_destroy(a1_ll);
    s2c_latlng_destroy(b0_ll);
    s2c_latlng_destroy(b1_ll);
    s2c_latlng_destroy(c0_ll);
    s2c_latlng_destroy(c1_ll);
    s2c_point_destroy(a0);
    s2c_point_destroy(a1);
    s2c_point_destroy(b0);
    s2c_point_destroy(b1);
    s2c_point_destroy(c0);
    s2c_point_destroy(c1);
}

TEST(Predicates, GetIntersection) {
    // Create two crossing line segments
    S2CLatLng* a0_ll = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* a1_ll = s2c_latlng_from_degrees(2.0, 2.0);
    S2CLatLng* b0_ll = s2c_latlng_from_degrees(0.0, 2.0);
    S2CLatLng* b1_ll = s2c_latlng_from_degrees(2.0, 0.0);
    
    S2CPoint* a0 = s2c_latlng_to_point(a0_ll);
    S2CPoint* a1 = s2c_latlng_to_point(a1_ll);
    S2CPoint* b0 = s2c_latlng_to_point(b0_ll);
    S2CPoint* b1 = s2c_latlng_to_point(b1_ll);
    
    S2CPoint* intersection = nullptr;
    bool result = s2c_get_intersection(a0, a1, b0, b1, &intersection);
    
    EXPECT_TRUE(result);
    ASSERT_NE(intersection, nullptr);
    
    // The intersection should be approximately at (1, 1)
    S2CLatLng* intersection_ll = s2c_latlng_from_point(intersection);
    double lat = s2c_latlng_lat_degrees(intersection_ll);
    double lng = s2c_latlng_lng_degrees(intersection_ll);
    
    EXPECT_NEAR(lat, 1.0, 0.1);
    EXPECT_NEAR(lng, 1.0, 0.1);
    
    s2c_latlng_destroy(a0_ll);
    s2c_latlng_destroy(a1_ll);
    s2c_latlng_destroy(b0_ll);
    s2c_latlng_destroy(b1_ll);
    s2c_latlng_destroy(intersection_ll);
    s2c_point_destroy(a0);
    s2c_point_destroy(a1);
    s2c_point_destroy(b0);
    s2c_point_destroy(b1);
    s2c_point_destroy(intersection);
}

TEST(Predicates, Interpolate) {
    S2CLatLng* a_ll = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* b_ll = s2c_latlng_from_degrees(10.0, 0.0);
    
    S2CPoint* a = s2c_latlng_to_point(a_ll);
    S2CPoint* b = s2c_latlng_to_point(b_ll);
    
    // Interpolate halfway
    S1CAngle* angle = s2c_interpolate(0.5, a, b);
    double angle_deg = s1c_angle_degrees(angle);
    
    // The angular distance between the points
    S1CAngle* total_angle = s2c_earth_get_distance_point(a, b);
    double total_deg = s1c_angle_degrees(total_angle);
    
    // Halfway should be approximately half the total angle
    EXPECT_NEAR(angle_deg, total_deg / 2.0, 0.01);
    
    s2c_latlng_destroy(a_ll);
    s2c_latlng_destroy(b_ll);
    s2c_point_destroy(a);
    s2c_point_destroy(b);
    s1c_angle_destroy(angle);
    s1c_angle_destroy(total_angle);
}

// Test S2RegionCoverer
TEST(S2RegionCoverer, CoverPolygon) {
    // Create a small polygon
    std::vector<S2CPoint*> points;
    S2CLatLng* ll1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* ll2 = s2c_latlng_from_degrees(0.0, 1.0);
    S2CLatLng* ll3 = s2c_latlng_from_degrees(1.0, 1.0);
    S2CLatLng* ll4 = s2c_latlng_from_degrees(1.0, 0.0);
    
    points.push_back(s2c_latlng_to_point(ll1));
    points.push_back(s2c_latlng_to_point(ll2));
    points.push_back(s2c_latlng_to_point(ll3));
    points.push_back(s2c_latlng_to_point(ll4));
    
    S2CLoop* loop = s2c_loop_new_from_points(points.data(), points.size());
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    // Create coverer with specific parameters
    S2CRegionCoverer* coverer = s2c_region_coverer_new();
    s2c_region_coverer_set_max_cells(coverer, 20);
    s2c_region_coverer_set_min_level(coverer, 0);
    s2c_region_coverer_set_max_level(coverer, 10);
    
    // Get covering (note: this function is not yet implemented in the C API)
    // S2CCellUnion* covering = s2c_region_coverer_get_covering_polygon(coverer, polygon);
    // ASSERT_NE(covering, nullptr);
    // EXPECT_GT(s2c_cellunion_num_cells(covering), 0);
    // EXPECT_LE(s2c_cellunion_num_cells(covering), 20);
    
    // Clean up
    s2c_latlng_destroy(ll1);
    s2c_latlng_destroy(ll2);
    s2c_latlng_destroy(ll3);
    s2c_latlng_destroy(ll4);
    s2c_region_coverer_destroy(coverer);
    s2c_polygon_destroy(polygon);
    for (auto* point : points) s2c_point_destroy(point);
}

// Test error handling
TEST(Error, CreateAndCheck) {
    S2CError* error = s2c_error_new();
    ASSERT_NE(error, nullptr);
    
    // Initially should be OK
    EXPECT_TRUE(s2c_error_is_ok(error));
    EXPECT_EQ(s2c_error_code(error), 0);
    
    // Set an error
    s2c_error_set(error, 1, "Test error message");
    EXPECT_FALSE(s2c_error_is_ok(error));
    EXPECT_EQ(s2c_error_code(error), 1);
    
    const char* message = s2c_error_message(error);
    EXPECT_STREQ(message, "Test error message");
    
    // Clear error
    s2c_error_clear(error);
    EXPECT_TRUE(s2c_error_is_ok(error));
    EXPECT_EQ(s2c_error_code(error), 0);
    
    s2c_error_destroy(error);
}

// Test string conversions
TEST(StringConversions, CellIdToString) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
    
    char* str = s2c_cellid_to_string(cell_id);
    ASSERT_NE(str, nullptr);
    EXPECT_GT(strlen(str), 0);
    
    // Should be a hex string
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        bool is_hex = (c >= '0' && c <= '9') || 
                     (c >= 'a' && c <= 'f') || 
                     (c >= 'A' && c <= 'F');
        EXPECT_TRUE(is_hex) << "Character '" << c << "' is not hexadecimal";
    }
    
    s2c_free_string(str);
    s2c_latlng_destroy(latlng);
    s2c_cellid_destroy(cell_id);
}

TEST(StringConversions, LatLngToString) {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    
    char* str = s2c_latlng_to_string(latlng);
    ASSERT_NE(str, nullptr);
    EXPECT_GT(strlen(str), 0);
    
    // Should contain the coordinates
    EXPECT_NE(strstr(str, "40.7128"), nullptr);
    EXPECT_NE(strstr(str, "-74.006"), nullptr);
    
    s2c_free_string(str);
    s2c_latlng_destroy(latlng);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}