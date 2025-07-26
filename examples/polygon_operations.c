#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "s2c.h"

// Helper function to create a regular polygon around a center point
S2CPolygon* create_regular_polygon(double center_lat, double center_lng, double radius_degrees, int num_vertices) {
    // Create vertices for a regular polygon
    S2CPoint** vertices = (S2CPoint**)malloc(num_vertices * sizeof(S2CPoint*));
    
    // Center point
    S2CLatLng* center = s2c_latlng_from_degrees(center_lat, center_lng);
    S2CPoint* center_point = s2c_latlng_to_point(center);
    
    for (int i = 0; i < num_vertices; i++) {
        double angle = 2.0 * M_PI * i / num_vertices;
        double lat = center_lat + radius_degrees * sin(angle);
        double lng = center_lng + radius_degrees * cos(angle) / cos(center_lat * M_PI / 180.0);
        
        S2CLatLng* vertex_latlng = s2c_latlng_from_degrees(lat, lng);
        vertices[i] = s2c_latlng_to_point(vertex_latlng);
        s2c_latlng_destroy(vertex_latlng);
    }
    
    // Create loop from vertices
    S2CLoop* loop = s2c_loop_new_from_points((const S2CPoint**)vertices, num_vertices);
    
    // Check if loop is valid
    S2CError error = {0};
    if (!s2c_loop_is_valid(loop, &error)) {
        printf("Loop validation failed: %s\n", error.text);
        if (error.text) s2c_free_string(error.text);
        s2c_loop_destroy(loop);
        for (int i = 0; i < num_vertices; i++) {
            s2c_point_destroy(vertices[i]);
        }
        free(vertices);
        s2c_point_destroy(center_point);
        s2c_latlng_destroy(center);
        return NULL;
    }
    
    // Create polygon from loop
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    // Cleanup
    for (int i = 0; i < num_vertices; i++) {
        s2c_point_destroy(vertices[i]);
    }
    free(vertices);
    s2c_point_destroy(center_point);
    s2c_latlng_destroy(center);
    
    return polygon;
}

void print_polygon_info(const char* name, S2CPolygon* polygon) {
    printf("\n%s:\n", name);
    printf("  Number of loops: %d\n", s2c_polygon_num_loops(polygon));
    printf("  Is empty: %s\n", s2c_polygon_is_empty(polygon) ? "yes" : "no");
    printf("  Area (steradians): %.10f\n", s2c_polygon_get_area(polygon));
    printf("  Area (km²): %.2f\n", s2c_polygon_get_area(polygon) * 6371.0 * 6371.0);
}

int main() {
    printf("S2 Polygon Operations Example\n");
    printf("=============================\n");
    
    // Create two overlapping regular polygons
    printf("\nCreating two overlapping hexagons...\n");
    
    // First polygon: hexagon centered at (40.0, -74.0) with 1 degree radius
    S2CPolygon* polygon1 = create_regular_polygon(40.0, -74.0, 1.0, 6);
    if (!polygon1) {
        printf("Failed to create polygon1\n");
        return 1;
    }
    print_polygon_info("Polygon 1", polygon1);
    
    // Second polygon: hexagon centered at (40.5, -73.5) with 1 degree radius
    S2CPolygon* polygon2 = create_regular_polygon(40.5, -73.5, 1.0, 6);
    if (!polygon2) {
        printf("Failed to create polygon2\n");
        s2c_polygon_destroy(polygon1);
        return 1;
    }
    print_polygon_info("Polygon 2", polygon2);
    
    // Test intersection
    printf("\nPolygons intersect: %s\n", 
           s2c_polygon_intersects(polygon1, polygon2) ? "yes" : "no");
    
    // Test point containment
    S2CLatLng* test_point_ll = s2c_latlng_from_degrees(40.0, -74.0);
    S2CPoint* test_point = s2c_latlng_to_point(test_point_ll);
    printf("\nPolygon 1 contains center of polygon 1: %s\n",
           s2c_polygon_contains(polygon1, test_point) ? "yes" : "no");
    printf("Polygon 2 contains center of polygon 1: %s\n",
           s2c_polygon_contains(polygon2, test_point) ? "yes" : "no");
    
    // Perform boolean operations
    printf("\nPerforming boolean operations...\n");
    
    // Create result polygon for union operation
    S2CPolygon* result = s2c_polygon_new();
    S2CPolygonLayer* result_layer = s2c_polygon_layer_new(result);
    S2CBuilderLayer* builder_layer = s2c_polygon_layer_as_builder_layer(result_layer);
    
    // Create boolean operation for union
    S2CBooleanOperation* union_op = s2c_boolean_operation_new(S2C_BOOLEAN_OP_UNION, builder_layer);
    
    // Add both polygons to shape index
    S2CMutableShapeIndex* index1 = s2c_mutable_shape_index_new();
    s2c_mutable_shape_index_add_polygon(index1, polygon1);
    
    S2CMutableShapeIndex* index2 = s2c_mutable_shape_index_new();
    s2c_mutable_shape_index_add_polygon(index2, polygon2);
    
    // Note: The actual boolean operation execution would require additional
    // wrapper functions to add shape indexes to the operation, which are not
    // yet implemented in our basic C API
    
    printf("\nNote: Full boolean operation execution requires additional wrapper functions\n");
    printf("that are not yet implemented in this basic example.\n");
    
    // Calculate individual areas for comparison
    double area1 = s2c_polygon_get_area(polygon1);
    double area2 = s2c_polygon_get_area(polygon2);
    printf("\nArea comparison:\n");
    printf("  Polygon 1: %.10f steradians\n", area1);
    printf("  Polygon 2: %.10f steradians\n", area2);
    printf("  Sum of areas: %.10f steradians\n", area1 + area2);
    
    // Cleanup
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(result);
    s2c_point_destroy(test_point);
    s2c_latlng_destroy(test_point_ll);
    s2c_boolean_operation_destroy(union_op);
    s2c_mutable_shape_index_destroy(index1);
    s2c_mutable_shape_index_destroy(index2);
    
    printf("\nExample completed successfully!\n");
    return 0;
}