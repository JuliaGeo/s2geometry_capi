#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "s2c.h"

// Simple test framework
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define RUN_TEST(test) do { \
    printf("Running %s... ", #test); \
    fflush(stdout); \
    tests_run++; \
    if (test()) { \
        printf("PASSED\n"); \
        tests_passed++; \
    } else { \
        printf("FAILED\n"); \
        tests_failed++; \
    } \
} while(0)

#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("\n  Assertion failed: %s\n", #condition); \
        return 0; \
    } \
} while(0)

#define ASSERT_DOUBLE_EQ(a, b) do { \
    if (fabs((a) - (b)) > 1e-10) { \
        printf("\n  Values not equal: %f != %f\n", (double)(a), (double)(b)); \
        return 0; \
    } \
} while(0)

#define ASSERT_NEAR(a, b, tol) do { \
    if (fabs((a) - (b)) > (tol)) { \
        printf("\n  Values not near: %f !~ %f (tolerance: %f)\n", (double)(a), (double)(b), (double)(tol)); \
        return 0; \
    } \
} while(0)

// Basic type tests
int test_s2point_create() {
    S2CPoint* point = s2c_point_new(1.0, 0.0, 0.0);
    ASSERT(point != NULL);
    
    double x, y, z;
    s2c_point_get_coords(point, &x, &y, &z);
    ASSERT_DOUBLE_EQ(x, 1.0);
    ASSERT_DOUBLE_EQ(y, 0.0);
    ASSERT_DOUBLE_EQ(z, 0.0);
    
    s2c_point_destroy(point);
    return 1;
}

int test_s2point_normalize() {
    S2CPoint* point = s2c_point_new(3.0, 4.0, 0.0);
    S2CPoint* normalized = s2c_point_normalize(point);
    
    double x, y, z;
    s2c_point_get_coords(normalized, &x, &y, &z);
    double norm = sqrt(x*x + y*y + z*z);
    ASSERT_NEAR(norm, 1.0, 1e-10);
    
    s2c_point_destroy(point);
    s2c_point_destroy(normalized);
    return 1;
}

int test_s2latlng_degrees() {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    ASSERT(latlng != NULL);
    
    double lat_deg = s2c_latlng_lat_degrees(latlng);
    double lng_deg = s2c_latlng_lng_degrees(latlng);
    
    ASSERT_DOUBLE_EQ(lat_deg, 40.7128);
    ASSERT_DOUBLE_EQ(lng_deg, -74.0060);
    
    s2c_latlng_destroy(latlng);
    return 1;
}

int test_s1angle_degrees() {
    S1CAngle* angle = s1c_angle_from_degrees(90.0);
    ASSERT(angle != NULL);
    
    double degrees = s1c_angle_degrees(angle);
    double radians = s1c_angle_radians(angle);
    
    ASSERT_DOUBLE_EQ(degrees, 90.0);
    ASSERT_NEAR(radians, M_PI / 2.0, 1e-10);
    
    s1c_angle_destroy(angle);
    return 1;
}

int test_s2cellid_from_latlng() {
    S2CLatLng* latlng = s2c_latlng_from_degrees(40.7128, -74.0060);
    S2CCellId* cell_id = s2c_cellid_from_latlng(latlng);
    
    ASSERT(cell_id != NULL);
    ASSERT(s2c_cellid_is_valid(cell_id));
    
    int level = s2c_cellid_level(cell_id);
    ASSERT(level == 30);  // Maximum level
    
    s2c_latlng_destroy(latlng);
    s2c_cellid_destroy(cell_id);
    return 1;
}

int test_r1interval_basic() {
    R1CInterval* interval = r1c_interval_new_from_bounds(1.0, 3.0);
    
    ASSERT_DOUBLE_EQ(r1c_interval_lo(interval), 1.0);
    ASSERT_DOUBLE_EQ(r1c_interval_hi(interval), 3.0);
    ASSERT_DOUBLE_EQ(r1c_interval_get_center(interval), 2.0);
    ASSERT_DOUBLE_EQ(r1c_interval_get_length(interval), 2.0);
    
    ASSERT(r1c_interval_contains(interval, 2.0));
    ASSERT(!r1c_interval_contains(interval, 4.0));
    
    r1c_interval_destroy(interval);
    return 1;
}

int test_s2cap_center_height() {
    S2CPoint* center = s2c_point_new(1.0, 0.0, 0.0);
    S2CCap* cap = s2c_cap_from_center_height(center, 0.5);
    
    ASSERT(s2c_cap_contains(cap, center));
    ASSERT(!s2c_cap_is_empty(cap));
    ASSERT(!s2c_cap_is_full(cap));
    
    double area = s2c_cap_area(cap);
    ASSERT(area > 0.0);
    ASSERT(area < 4 * M_PI);  // Less than full sphere
    
    s2c_point_destroy(center);
    s2c_cap_destroy(cap);
    return 1;
}

// Geometry tests
int test_s2loop_create() {
    // Create triangle
    S2CLatLng* latlng1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* latlng2 = s2c_latlng_from_degrees(0.0, 1.0);
    S2CLatLng* latlng3 = s2c_latlng_from_degrees(1.0, 0.0);
    
    S2CPoint* points[3];
    points[0] = s2c_latlng_to_point(latlng1);
    points[1] = s2c_latlng_to_point(latlng2);
    points[2] = s2c_latlng_to_point(latlng3);
    
    S2CLoop* loop = s2c_loop_new_from_points(points, 3);
    
    ASSERT(loop != NULL);
    ASSERT(s2c_loop_num_vertices(loop) == 3);
    ASSERT(s2c_loop_is_valid(loop, NULL));
    
    s2c_loop_destroy(loop);
    s2c_latlng_destroy(latlng1);
    s2c_latlng_destroy(latlng2);
    s2c_latlng_destroy(latlng3);
    s2c_point_destroy(points[0]);
    s2c_point_destroy(points[1]);
    s2c_point_destroy(points[2]);
    return 1;
}

int test_s2polyline_create() {
    S2CPoint* points[6];
    
    // Create a simple line
    for (int i = 0; i < 6; i++) {
        S2CLatLng* latlng = s2c_latlng_from_degrees(i * 2.0, 0.0);
        points[i] = s2c_latlng_to_point(latlng);
        s2c_latlng_destroy(latlng);
    }
    
    S2CPolyline* polyline = s2c_polyline_new_from_points(points, 6);
    ASSERT(polyline != NULL);
    ASSERT(s2c_polyline_num_vertices(polyline) == 6);
    
    S1CAngle* length = s2c_polyline_get_length(polyline);
    ASSERT(s1c_angle_radians(length) > 0.0);
    s1c_angle_destroy(length);
    
    s2c_polyline_destroy(polyline);
    for (int i = 0; i < 6; i++) {
        s2c_point_destroy(points[i]);
    }
    return 1;
}

int test_s2polygon_create() {
    // Create triangle
    S2CLatLng* latlng1 = s2c_latlng_from_degrees(0.0, 0.0);
    S2CLatLng* latlng2 = s2c_latlng_from_degrees(0.0, 1.0);
    S2CLatLng* latlng3 = s2c_latlng_from_degrees(1.0, 0.0);
    
    S2CPoint* points[3];
    points[0] = s2c_latlng_to_point(latlng1);
    points[1] = s2c_latlng_to_point(latlng2);
    points[2] = s2c_latlng_to_point(latlng3);
    
    S2CLoop* loop = s2c_loop_new_from_points(points, 3);
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    ASSERT(polygon != NULL);
    ASSERT(!s2c_polygon_is_empty(polygon));
    ASSERT(s2c_polygon_num_loops(polygon) == 1);
    ASSERT(s2c_polygon_is_valid(polygon, NULL));
    
    double area = s2c_polygon_get_area(polygon);
    ASSERT(area > 0.0);
    ASSERT(area < 4 * M_PI);  // Less than full sphere
    
    s2c_polygon_destroy(polygon);
    s2c_latlng_destroy(latlng1);
    s2c_latlng_destroy(latlng2);
    s2c_latlng_destroy(latlng3);
    s2c_point_destroy(points[0]);
    s2c_point_destroy(points[1]);
    s2c_point_destroy(points[2]);
    return 1;
}

// Utility tests
int test_s2earth_conversions() {
    double km = 100.0;
    double radians = s2c_earth_km_to_radians(km);
    double km_back = s2c_earth_radians_to_km(radians);
    ASSERT_NEAR(km, km_back, 1e-10);
    
    double meters = 1000.0;
    radians = s2c_earth_meters_to_radians(meters);
    double meters_back = s2c_earth_radians_to_meters(radians);
    ASSERT_NEAR(meters, meters_back, 1e-10);
    
    return 1;
}

int test_s2earth_distance() {
    // New York to London (approximate)
    S2CLatLng* ny = s2c_latlng_from_degrees(40.7128, -74.0060);
    S2CLatLng* london = s2c_latlng_from_degrees(51.5074, -0.1278);
    
    double km_dist = s2c_earth_get_distance_km_latlng(ny, london);
    
    // Approximate distance should be around 5570 km
    ASSERT(km_dist > 5000.0);
    ASSERT(km_dist < 6000.0);
    
    s2c_latlng_destroy(ny);
    s2c_latlng_destroy(london);
    return 1;
}

int test_error_handling() {
    S2CError* error = s2c_error_new();
    ASSERT(error != NULL);
    
    // Initially should be OK
    ASSERT(s2c_error_is_ok(error));
    ASSERT(s2c_error_code(error) == 0);
    
    // Set an error
    s2c_error_set(error, 1, "Test error message");
    ASSERT(!s2c_error_is_ok(error));
    ASSERT(s2c_error_code(error) == 1);
    
    const char* message = s2c_error_message(error);
    ASSERT(strcmp(message, "Test error message") == 0);
    
    // Clear error
    s2c_error_clear(error);
    ASSERT(s2c_error_is_ok(error));
    ASSERT(s2c_error_code(error) == 0);
    
    s2c_error_destroy(error);
    return 1;
}

int main() {
    printf("Running S2C API Tests\n");
    printf("====================\n\n");
    
    // Basic type tests
    printf("Basic Types:\n");
    RUN_TEST(test_s2point_create);
    RUN_TEST(test_s2point_normalize);
    RUN_TEST(test_s2latlng_degrees);
    RUN_TEST(test_s1angle_degrees);
    RUN_TEST(test_s2cellid_from_latlng);
    RUN_TEST(test_r1interval_basic);
    RUN_TEST(test_s2cap_center_height);
    
    // Geometry tests
    printf("\nGeometry Types:\n");
    RUN_TEST(test_s2loop_create);
    RUN_TEST(test_s2polyline_create);
    RUN_TEST(test_s2polygon_create);
    
    // Utility tests
    printf("\nUtilities:\n");
    RUN_TEST(test_s2earth_conversions);
    RUN_TEST(test_s2earth_distance);
    RUN_TEST(test_error_handling);
    
    // Summary
    printf("\n====================\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    
    return tests_failed > 0 ? 1 : 0;
}