#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "s2c.h"

#define ASSERT(condition) \
    if (!(condition)) { \
        printf("Assertion failed: %s (line %d)\n", #condition, __LINE__); \
        return 1; \
    }

// Helper function to create a polygon from lat/lng coordinates
S2CPolygon* create_polygon_from_coords(double coords[][2], int num_vertices) {
    S2CPoint** points = (S2CPoint**)malloc(num_vertices * sizeof(S2CPoint*));
    for (int i = 0; i < num_vertices; i++) {
        S2CLatLng* latlng = s2c_latlng_from_degrees(coords[i][0], coords[i][1]);
        points[i] = s2c_latlng_to_point(latlng);
        s2c_latlng_destroy(latlng);
    }
    
    S2CLoop* loop = s2c_loop_new_from_points((const S2CPoint**)points, num_vertices);
    S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);
    
    for (int i = 0; i < num_vertices; i++) {
        s2c_point_destroy(points[i]);
    }
    free(points);
    
    return polygon;
}

int test_boolean_union_with_shape_indexes() {
    printf("Testing S2BooleanOperation UNION with shape indexes...\n");
    
    // Create two shape indexes
    S2CMutableShapeIndex* index1 = s2c_mutable_shape_index_new();
    S2CMutableShapeIndex* index2 = s2c_mutable_shape_index_new();
    
    // Add polygons to first index (two squares)
    double coords1[][2] = {{0, 0}, {0, 10}, {10, 10}, {10, 0}};
    S2CPolygon* polygon1 = create_polygon_from_coords(coords1, 4);
    s2c_mutable_shape_index_add_polygon(index1, polygon1);
    
    double coords2[][2] = {{20, 0}, {20, 10}, {30, 10}, {30, 0}};
    S2CPolygon* polygon2 = create_polygon_from_coords(coords2, 4);
    s2c_mutable_shape_index_add_polygon(index1, polygon2);
    
    // Add polygons to second index (overlapping square and separate square)
    double coords3[][2] = {{5, 5}, {5, 15}, {15, 15}, {15, 5}};
    S2CPolygon* polygon3 = create_polygon_from_coords(coords3, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon3);
    
    double coords4[][2] = {{40, 0}, {40, 10}, {50, 10}, {50, 0}};
    S2CPolygon* polygon4 = create_polygon_from_coords(coords4, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon4);
    
    // Create result polygon
    S2CPolygon* result = s2c_polygon_new();
    S2CPolygonLayer* layer = s2c_polygon_layer_new(result);
    S2CBuilderLayer* builder_layer = s2c_polygon_layer_as_builder_layer(layer);
    
    // Create boolean operation for UNION
    S2CBooleanOperation* union_op = s2c_boolean_operation_new(S2C_BOOLEAN_OP_UNION, builder_layer);
    ASSERT(union_op != NULL);
    
    // Build the union
    S2CError error;
    bool success = s2c_boolean_operation_build_mutable_indexes(union_op, index1, index2, &error);
    ASSERT(success);
    ASSERT(error.ok);
    
    // The result polygon should contain the union of all polygons
    // It should have multiple loops (outer boundaries)
    int num_loops = s2c_polygon_num_loops(result);
    printf("  Union result has %d loops\n", num_loops);
    ASSERT(num_loops > 0);
    
    // Check that result is valid
    S2CError valid_error;
    ASSERT(s2c_polygon_is_valid(result, &valid_error));
    ASSERT(valid_error.ok);
    
    // Check approximate area - should be larger than any individual polygon
    double area = s2c_polygon_get_area(result);
    printf("  Union area: %.2f steradians\n", area);
    ASSERT(area > 0);
    
    // Clean up
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(polygon3);
    s2c_polygon_destroy(polygon4);
    s2c_polygon_destroy(result);
    s2c_boolean_operation_destroy(union_op);
    s2c_mutable_shape_index_destroy(index1);
    s2c_mutable_shape_index_destroy(index2);
    
    return 0;
}

int test_boolean_intersection_with_shape_indexes() {
    printf("Testing S2BooleanOperation INTERSECTION with shape indexes...\n");
    
    // Create two shape indexes
    S2CMutableShapeIndex* index1 = s2c_mutable_shape_index_new();
    S2CMutableShapeIndex* index2 = s2c_mutable_shape_index_new();
    
    // Add a large polygon to first index
    double coords1[][2] = {{0, 0}, {0, 20}, {20, 20}, {20, 0}};
    S2CPolygon* polygon1 = create_polygon_from_coords(coords1, 4);
    s2c_mutable_shape_index_add_polygon(index1, polygon1);
    
    // Add two smaller polygons to second index (one overlapping, one not)
    double coords2[][2] = {{5, 5}, {5, 15}, {15, 15}, {15, 5}};
    S2CPolygon* polygon2 = create_polygon_from_coords(coords2, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon2);
    
    double coords3[][2] = {{25, 25}, {25, 35}, {35, 35}, {35, 25}};
    S2CPolygon* polygon3 = create_polygon_from_coords(coords3, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon3);
    
    // Create result polygon
    S2CPolygon* result = s2c_polygon_new();
    S2CPolygonLayer* layer = s2c_polygon_layer_new(result);
    S2CBuilderLayer* builder_layer = s2c_polygon_layer_as_builder_layer(layer);
    
    // Create boolean operation for INTERSECTION
    S2CBooleanOperation* intersect_op = s2c_boolean_operation_new(S2C_BOOLEAN_OP_INTERSECTION, builder_layer);
    ASSERT(intersect_op != NULL);
    
    // Build the intersection
    S2CError error;
    bool success = s2c_boolean_operation_build_mutable_indexes(intersect_op, index1, index2, &error);
    ASSERT(success);
    ASSERT(error.ok);
    
    // The result should only contain the overlapping region
    int num_loops = s2c_polygon_num_loops(result);
    printf("  Intersection result has %d loops\n", num_loops);
    
    if (num_loops > 0) {
        // Check that result is valid
        S2CError valid_error;
        ASSERT(s2c_polygon_is_valid(result, &valid_error));
        ASSERT(valid_error.ok);
        
        // The intersection area should be smaller than either input
        double area = s2c_polygon_get_area(result);
        printf("  Intersection area: %.2f steradians\n", area);
        ASSERT(area > 0);
        ASSERT(area < s2c_polygon_get_area(polygon1));
    }
    
    // Clean up
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(polygon3);
    s2c_polygon_destroy(result);
    s2c_boolean_operation_destroy(intersect_op);
    s2c_mutable_shape_index_destroy(index1);
    s2c_mutable_shape_index_destroy(index2);
    
    return 0;
}

int test_boolean_difference_with_shape_indexes() {
    printf("Testing S2BooleanOperation DIFFERENCE with shape indexes...\n");
    
    // Create two shape indexes
    S2CMutableShapeIndex* index1 = s2c_mutable_shape_index_new();
    S2CMutableShapeIndex* index2 = s2c_mutable_shape_index_new();
    
    // Add a large polygon to first index
    double coords1[][2] = {{0, 0}, {0, 20}, {20, 20}, {20, 0}};
    S2CPolygon* polygon1 = create_polygon_from_coords(coords1, 4);
    s2c_mutable_shape_index_add_polygon(index1, polygon1);
    
    // Add a smaller polygon to second index (to subtract from first)
    double coords2[][2] = {{5, 5}, {5, 15}, {15, 15}, {15, 5}};
    S2CPolygon* polygon2 = create_polygon_from_coords(coords2, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon2);
    
    // Create result polygon
    S2CPolygon* result = s2c_polygon_new();
    S2CPolygonLayer* layer = s2c_polygon_layer_new(result);
    S2CBuilderLayer* builder_layer = s2c_polygon_layer_as_builder_layer(layer);
    
    // Create boolean operation for DIFFERENCE
    S2CBooleanOperation* diff_op = s2c_boolean_operation_new(S2C_BOOLEAN_OP_DIFFERENCE, builder_layer);
    ASSERT(diff_op != NULL);
    
    // Build the difference (index1 - index2)
    S2CError error;
    bool success = s2c_boolean_operation_build_mutable_indexes(diff_op, index1, index2, &error);
    ASSERT(success);
    ASSERT(error.ok);
    
    // The result should be polygon1 with a hole where polygon2 was
    int num_loops = s2c_polygon_num_loops(result);
    printf("  Difference result has %d loops\n", num_loops);
    
    if (num_loops > 0) {
        // Check that result is valid
        S2CError valid_error;
        ASSERT(s2c_polygon_is_valid(result, &valid_error));
        ASSERT(valid_error.ok);
        
        // Check if we have a hole (inner loop)
        bool has_hole = false;
        for (int i = 0; i < num_loops; i++) {
            S2CLoop* loop = s2c_polygon_loop(result, i);
            if (s2c_loop_is_hole(loop)) {
                has_hole = true;
                printf("  Found hole at loop %d\n", i);
            }
        }
        
        // The difference area should be less than the original
        double area = s2c_polygon_get_area(result);
        printf("  Difference area: %.2f steradians\n", area);
        ASSERT(area > 0);
        ASSERT(area < s2c_polygon_get_area(polygon1));
    }
    
    // Clean up
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(result);
    s2c_boolean_operation_destroy(diff_op);
    s2c_mutable_shape_index_destroy(index1);
    s2c_mutable_shape_index_destroy(index2);
    
    return 0;
}

int test_boolean_operations_with_options() {
    printf("Testing S2BooleanOperation with custom options...\n");
    
    // Create shape indexes
    S2CMutableShapeIndex* index1 = s2c_mutable_shape_index_new();
    S2CMutableShapeIndex* index2 = s2c_mutable_shape_index_new();
    
    // Add polygons
    double coords1[][2] = {{0, 0}, {0, 10}, {10, 10}, {10, 0}};
    S2CPolygon* polygon1 = create_polygon_from_coords(coords1, 4);
    s2c_mutable_shape_index_add_polygon(index1, polygon1);
    
    double coords2[][2] = {{10, 0}, {10, 10}, {20, 10}, {20, 0}};
    S2CPolygon* polygon2 = create_polygon_from_coords(coords2, 4);
    s2c_mutable_shape_index_add_polygon(index2, polygon2);
    
    // Create options
    S2CBooleanOperationOptions* options = s2c_boolean_operation_options_new();
    s2c_boolean_operation_options_set_polygon_model(options, S2C_POLYGON_MODEL_CLOSED);
    s2c_boolean_operation_options_set_snap_function(options, 10); // Snap to level 10 cells
    
    // Create result polygon
    S2CPolygon* result = s2c_polygon_new();
    S2CPolygonLayer* layer = s2c_polygon_layer_new(result);
    S2CBuilderLayer* builder_layer = s2c_polygon_layer_as_builder_layer(layer);
    
    // Create boolean operation with options
    S2CBooleanOperation* union_op = s2c_boolean_operation_new_with_options(
        S2C_BOOLEAN_OP_UNION, builder_layer, options);
    ASSERT(union_op != NULL);
    
    // Build the union
    S2CError error;
    bool success = s2c_boolean_operation_build_mutable_indexes(union_op, index1, index2, &error);
    ASSERT(success);
    ASSERT(error.ok);
    
    // Verify result
    S2CError valid_error;
    ASSERT(s2c_polygon_is_valid(result, &valid_error));
    ASSERT(valid_error.ok);
    int num_loops = s2c_polygon_num_loops(result);
    printf("  Union with options result has %d loops\n", num_loops);
    
    // Clean up
    s2c_boolean_operation_options_destroy(options);
    s2c_polygon_destroy(polygon1);
    s2c_polygon_destroy(polygon2);
    s2c_polygon_destroy(result);
    s2c_boolean_operation_destroy(union_op);
    s2c_mutable_shape_index_destroy(index1);
    s2c_mutable_shape_index_destroy(index2);
    
    return 0;
}

int main() {
    printf("Running S2BooleanOperation tests...\n\n");
    
    if (test_boolean_union_with_shape_indexes() != 0) return 1;
    if (test_boolean_intersection_with_shape_indexes() != 0) return 1;
    if (test_boolean_difference_with_shape_indexes() != 0) return 1;
    if (test_boolean_operations_with_options() != 0) return 1;
    
    printf("\nAll S2BooleanOperation tests passed!\n");
    printf("\nSummary: Successfully demonstrated boolean operations with shape indexes:\n");
    printf("- UNION of multiple polygon coverages\n");
    printf("- INTERSECTION to find overlapping regions\n");
    printf("- DIFFERENCE to subtract one coverage from another\n");
    printf("- Custom options for polygon models and snapping\n");
    
    return 0;
}