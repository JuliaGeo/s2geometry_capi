#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "s2c.h"

int main() {
    printf("Simple S2 Polygon Test\n");
    printf("=====================\n\n");
    
    // Create a triangle polygon around Manhattan
    printf("Creating a triangle around Manhattan...\n");
    
    // Define three vertices of the triangle
    S2CLatLng* v1_ll = s2c_latlng_from_degrees(40.7, -74.1);  // West
    S2CLatLng* v2_ll = s2c_latlng_from_degrees(40.8, -73.9);  // North
    S2CLatLng* v3_ll = s2c_latlng_from_degrees(40.7, -73.9);  // East
    
    // Convert to S2Points
    S2CPoint* v1 = s2c_latlng_to_point(v1_ll);
    S2CPoint* v2 = s2c_latlng_to_point(v2_ll);
    S2CPoint* v3 = s2c_latlng_to_point(v3_ll);
    
    // Create array of vertices
    const S2CPoint* vertices[] = {v1, v2, v3};
    
    // Create loop from vertices
    S2CLoop* loop = s2c_loop_new_from_points(vertices, 3);
    
    // Validate the loop
    S2CError error = {0};
    if (s2c_loop_is_valid(loop, &error)) {
        printf("✓ Loop is valid\n");
    } else {
        printf("✗ Loop is invalid: %s\n", error.text);
        if (error.text) s2c_free_string(error.text);
    }
    
    // Get loop properties
    printf("\nLoop properties:\n");
    printf("  Number of vertices: %d\n", s2c_loop_num_vertices(loop));
    printf("  Area (steradians): %.10f\n", s2c_loop_get_area(loop));
    printf("  Area (km²): %.2f\n", s2c_loop_get_area(loop) * 6371.0 * 6371.0);
    
    // Create polygon from loop
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    // Validate polygon
    if (s2c_polygon_is_valid(polygon, &error)) {
        printf("\n✓ Polygon is valid\n");
    } else {
        printf("\n✗ Polygon is invalid: %s\n", error.text);
        if (error.text) s2c_free_string(error.text);
    }
    
    // Test point containment
    printf("\nTesting point containment:\n");
    
    // Test point inside the triangle (center of Manhattan)
    S2CLatLng* test1_ll = s2c_latlng_from_degrees(40.75, -73.98);
    S2CPoint* test1 = s2c_latlng_to_point(test1_ll);
    printf("  Manhattan center (40.75, -73.98): %s\n", 
           s2c_polygon_contains(polygon, test1) ? "INSIDE" : "OUTSIDE");
    
    // Test point outside the triangle
    S2CLatLng* test2_ll = s2c_latlng_from_degrees(40.6, -74.0);
    S2CPoint* test2 = s2c_latlng_to_point(test2_ll);
    printf("  South of triangle (40.6, -74.0): %s\n", 
           s2c_polygon_contains(polygon, test2) ? "INSIDE" : "OUTSIDE");
    
    // Test vertex point
    printf("  Vertex point: %s\n", 
           s2c_polygon_contains(polygon, v1) ? "INSIDE" : "OUTSIDE");
    
    // Calculate distances from polygon to test points
    printf("\nCalculating distances:\n");
    S1CAngle* angle1 = s1c_angle_from_degrees(40.75);
    S1CAngle* angle2 = s1c_angle_from_degrees(-73.98);
    S2CLatLng* center_ll = s2c_latlng_new(angle1, angle2);
    S2CPoint* center = s2c_latlng_to_point(center_ll);
    
    // Get polygon centroid
    S2CPoint* centroid = s2c_polygon_get_centroid(polygon);
    S2CLatLng* centroid_ll = s2c_latlng_from_point(centroid);
    S1CAngle* centroid_lat = s2c_latlng_lat(centroid_ll);
    S1CAngle* centroid_lng = s2c_latlng_lng(centroid_ll);
    
    printf("  Polygon centroid: (%.4f°, %.4f°)\n", 
           s1c_angle_degrees(centroid_lat), 
           s1c_angle_degrees(centroid_lng));
    
    // Create another polygon (rectangle) that overlaps
    printf("\nCreating overlapping rectangle...\n");
    S2CLatLng* r1_ll = s2c_latlng_from_degrees(40.72, -74.05);
    S2CLatLng* r2_ll = s2c_latlng_from_degrees(40.78, -74.05);
    S2CLatLng* r3_ll = s2c_latlng_from_degrees(40.78, -73.95);
    S2CLatLng* r4_ll = s2c_latlng_from_degrees(40.72, -73.95);
    
    S2CPoint* r1 = s2c_latlng_to_point(r1_ll);
    S2CPoint* r2 = s2c_latlng_to_point(r2_ll);
    S2CPoint* r3 = s2c_latlng_to_point(r3_ll);
    S2CPoint* r4 = s2c_latlng_to_point(r4_ll);
    
    const S2CPoint* rect_vertices[] = {r1, r2, r3, r4};
    S2CLoop* rect_loop = s2c_loop_new_from_points(rect_vertices, 4);
    S2CPolygon* rect_polygon = s2c_polygon_new_from_loop(rect_loop);
    
    printf("  Rectangle area (km²): %.2f\n", 
           s2c_polygon_get_area(rect_polygon) * 6371.0 * 6371.0);
    
    // Test intersection
    printf("  Polygons intersect: %s\n", 
           s2c_polygon_intersects(polygon, rect_polygon) ? "YES" : "NO");
    
    // Cleanup
    s2c_polygon_destroy(polygon);
    s2c_polygon_destroy(rect_polygon);
    s2c_point_destroy(v1);
    s2c_point_destroy(v2);
    s2c_point_destroy(v3);
    s2c_point_destroy(r1);
    s2c_point_destroy(r2);
    s2c_point_destroy(r3);
    s2c_point_destroy(r4);
    s2c_point_destroy(test1);
    s2c_point_destroy(test2);
    s2c_point_destroy(center);
    s2c_point_destroy(centroid);
    s2c_latlng_destroy(v1_ll);
    s2c_latlng_destroy(v2_ll);
    s2c_latlng_destroy(v3_ll);
    s2c_latlng_destroy(r1_ll);
    s2c_latlng_destroy(r2_ll);
    s2c_latlng_destroy(r3_ll);
    s2c_latlng_destroy(r4_ll);
    s2c_latlng_destroy(test1_ll);
    s2c_latlng_destroy(test2_ll);
    s2c_latlng_destroy(center_ll);
    s2c_latlng_destroy(centroid_ll);
    s1c_angle_destroy(angle1);
    s1c_angle_destroy(angle2);
    s1c_angle_destroy(centroid_lat);
    s1c_angle_destroy(centroid_lng);
    
    printf("\nTest completed successfully!\n");
    return 0;
}