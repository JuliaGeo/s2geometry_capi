#ifndef S2C_H
#define S2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error handling
typedef struct S2CError {
    bool ok;
    char* text;
} S2CError;

// Opaque pointer types
typedef struct S2CPoint S2CPoint;
typedef struct S2CLatLng S2CLatLng;
typedef struct S2CCellId S2CCellId;
typedef struct S2CCell S2CCell;
typedef struct S2CCap S2CCap;
typedef struct S2CLoop S2CLoop;
typedef struct S2CPolyline S2CPolyline;
typedef struct S2CPolygon S2CPolygon;
typedef struct S2CLatLngRect S2CLatLngRect;
typedef struct S2CCellUnion S2CCellUnion;
typedef struct S2CRegionCoverer S2CRegionCoverer;
typedef struct S2CRegionTermIndexer S2CRegionTermIndexer;
typedef struct S2CBuilder S2CBuilder;
typedef struct S2CBuilderLayer S2CBuilderLayer;
typedef struct S2CPolygonLayer S2CPolygonLayer;
typedef struct S2CBooleanOperation S2CBooleanOperation;
typedef struct S2CBufferOperation S2CBufferOperation;
typedef struct S2CMutableShapeIndex S2CMutableShapeIndex;
typedef struct S1CAngle S1CAngle;
typedef struct S1CChordAngle S1CChordAngle;
typedef struct S1CInterval S1CInterval;
typedef struct R1CInterval R1CInterval;

// Enumerations
typedef enum {
    S2C_CROSSING_SIGN_NEGATIVE = -1,
    S2C_CROSSING_SIGN_ZERO = 0,
    S2C_CROSSING_SIGN_POSITIVE = 1
} S2CCrossingSign;

typedef enum {
    S2C_BOOLEAN_OP_UNION,
    S2C_BOOLEAN_OP_INTERSECTION,
    S2C_BOOLEAN_OP_DIFFERENCE,
    S2C_BOOLEAN_OP_SYMMETRIC_DIFFERENCE
} S2CBooleanOpType;

typedef enum {
    S2C_EDGE_TYPE_DIRECTED,
    S2C_EDGE_TYPE_UNDIRECTED
} S2CEdgeType;

// S2Point functions
S2CPoint* s2c_point_new(double x, double y, double z);
S2CPoint* s2c_point_from_latlng(const S2CLatLng* latlng);
void s2c_point_destroy(S2CPoint* point);
double s2c_point_x(const S2CPoint* point);
double s2c_point_y(const S2CPoint* point);
double s2c_point_z(const S2CPoint* point);
void s2c_point_get_coords(const S2CPoint* point, double* x, double* y, double* z);
double s2c_point_norm(const S2CPoint* point);
S2CPoint* s2c_point_normalize(const S2CPoint* point);

// S2LatLng functions
S2CLatLng* s2c_latlng_new(const S1CAngle* lat, const S1CAngle* lng);
S2CLatLng* s2c_latlng_from_degrees(double lat_degrees, double lng_degrees);
S2CLatLng* s2c_latlng_from_radians(double lat_radians, double lng_radians);
S2CLatLng* s2c_latlng_from_e6(int32_t lat_e6, int32_t lng_e6);
S2CLatLng* s2c_latlng_from_e7(int32_t lat_e7, int32_t lng_e7);
S2CLatLng* s2c_latlng_from_unsigned_e6(uint32_t lat_e6, uint32_t lng_e6);
S2CLatLng* s2c_latlng_from_unsigned_e7(uint32_t lat_e7, uint32_t lng_e7);
S2CLatLng* s2c_latlng_from_point(const S2CPoint* point);
void s2c_latlng_destroy(S2CLatLng* latlng);
S1CAngle* s2c_latlng_lat(const S2CLatLng* latlng);
S1CAngle* s2c_latlng_lng(const S2CLatLng* latlng);
S2CPoint* s2c_latlng_to_point(const S2CLatLng* latlng);
bool s2c_latlng_is_valid(const S2CLatLng* latlng);
S2CLatLng* s2c_latlng_normalized(const S2CLatLng* latlng);
S1CAngle* s2c_latlng_get_distance(const S2CLatLng* a, const S2CLatLng* b);
char* s2c_latlng_to_string_in_degrees(const S2CLatLng* latlng);
bool s2c_latlng_approx_equals(const S2CLatLng* a, const S2CLatLng* b);

// S2CellId functions
S2CCellId* s2c_cellid_new(uint64_t id);
S2CCellId* s2c_cellid_begin(int level);
S2CCellId* s2c_cellid_end(int level);
S2CCellId* s2c_cellid_from_face_ij(int face, int i, int j, int level);
S2CCellId* s2c_cellid_from_face_pos_level(int face, uint64_t pos, int level);
S2CCellId* s2c_cellid_from_latlng(const S2CLatLng* latlng);
S2CCellId* s2c_cellid_from_point(const S2CPoint* point);
S2CCellId* s2c_cellid_from_token(const char* token);
S2CCellId* s2c_cellid_from_debug_string(const char* str);
void s2c_cellid_destroy(S2CCellId* cellid);
uint64_t s2c_cellid_id(const S2CCellId* cellid);
int s2c_cellid_face(const S2CCellId* cellid);
uint64_t s2c_cellid_pos(const S2CCellId* cellid);
int s2c_cellid_level(const S2CCellId* cellid);
bool s2c_cellid_is_valid(const S2CCellId* cellid);
bool s2c_cellid_is_face(const S2CCellId* cellid);
bool s2c_cellid_is_leaf(const S2CCellId* cellid);
S2CCellId* s2c_cellid_parent(const S2CCellId* cellid, int level);
S2CCellId* s2c_cellid_child(const S2CCellId* cellid, int position);
S2CCellId* s2c_cellid_child_begin(const S2CCellId* cellid);
S2CCellId* s2c_cellid_child_end(const S2CCellId* cellid);
int s2c_cellid_child_position(const S2CCellId* cellid, int level);
S2CCellId* s2c_cellid_next(const S2CCellId* cellid);
S2CCellId* s2c_cellid_prev(const S2CCellId* cellid);
S2CCellId* s2c_cellid_range_min(const S2CCellId* cellid);
S2CCellId* s2c_cellid_range_max(const S2CCellId* cellid);
bool s2c_cellid_contains(const S2CCellId* a, const S2CCellId* b);
bool s2c_cellid_intersects(const S2CCellId* a, const S2CCellId* b);
S2CLatLng* s2c_cellid_to_latlng(const S2CCellId* cellid);
S2CPoint* s2c_cellid_to_point(const S2CCellId* cellid);
char* s2c_cellid_to_string(const S2CCellId* cellid);
char* s2c_cellid_to_token(const S2CCellId* cellid);
void s2c_cellid_get_center_si_ti(const S2CCellId* cellid, int* psi, int* pti);
void s2c_cellid_get_edge_neighbors(const S2CCellId* cellid, S2CCellId* neighbors[4]);
void s2c_cellid_to_face_ij_orientation(const S2CCellId* cellid, int* pface, int* pi, int* pj, int* orientation);
void s2c_cellid_append_all_neighbors(const S2CCellId* cellid, int nbr_level, S2CCellId*** neighbors, int* count);
void s2c_cellid_append_vertex_neighbors(const S2CCellId* cellid, int level, S2CCellId*** neighbors, int* count);

// S1Angle functions
S1CAngle* s1c_angle_new(void);
S1CAngle* s1c_angle_from_radians(double radians);
S1CAngle* s1c_angle_from_degrees(double degrees);
S1CAngle* s1c_angle_from_e5(int32_t e5);
S1CAngle* s1c_angle_from_e6(int32_t e6);
S1CAngle* s1c_angle_from_e7(int32_t e7);
S1CAngle* s1c_angle_from_unsigned_e6(uint32_t e6);
void s1c_angle_destroy(S1CAngle* angle);
double s1c_angle_radians(const S1CAngle* angle);
double s1c_angle_degrees(const S1CAngle* angle);
int32_t s1c_angle_e5(const S1CAngle* angle);
int32_t s1c_angle_e6(const S1CAngle* angle);
int32_t s1c_angle_e7(const S1CAngle* angle);
S1CAngle* s1c_angle_abs(const S1CAngle* angle);
S1CAngle* s1c_angle_normalize(const S1CAngle* angle);
S1CAngle* s1c_angle_normalized(const S1CAngle* angle);

// S1ChordAngle functions
S1CChordAngle* s1c_chordangle_new(void);
S1CChordAngle* s1c_chordangle_infinity(void);
void s1c_chordangle_destroy(S1CChordAngle* angle);
S1CAngle* s1c_chordangle_to_angle(const S1CChordAngle* angle);
double s1c_chordangle_degrees(const S1CChordAngle* angle);

// S2Cap functions
S2CCap* s2c_cap_new(void);
S2CCap* s2c_cap_from_center_height(const S2CPoint* center, double height);
S2CCap* s2c_cap_from_center_area(const S2CPoint* center, double area);
S2CCap* s2c_cap_from_point(const S2CPoint* point);
S2CCap* s2c_cap_empty(void);
S2CCap* s2c_cap_full(void);
void s2c_cap_destroy(S2CCap* cap);
S2CPoint* s2c_cap_center(const S2CCap* cap);
double s2c_cap_height(const S2CCap* cap);
bool s2c_cap_is_empty(const S2CCap* cap);
bool s2c_cap_is_valid(const S2CCap* cap);
S2CCap* s2c_cap_clone(const S2CCap* cap);
bool s2c_cap_contains(const S2CCap* cap, const S2CPoint* point);
bool s2c_cap_contains_cap(const S2CCap* cap, const S2CCap* other);
bool s2c_cap_intersects(const S2CCap* a, const S2CCap* b);
S2CCap* s2c_cap_union(const S2CCap* a, const S2CCap* b);
S2CCap* s2c_cap_expanded(const S2CCap* cap, S1CAngle* angle);
S2CCap* s2c_cap_get_cap_bound(const S2CCap* cap);
S2CLatLngRect* s2c_cap_get_rect_bound(const S2CCap* cap);
S2CPoint* s2c_cap_get_centroid(const S2CCap* cap);
void s2c_cap_add_point(S2CCap* cap, const S2CPoint* point);
bool s2c_cap_approx_equals(const S2CCap* a, const S2CCap* b);
bool s2c_cap_may_intersect_cell(const S2CCap* cap, const S2CCell* cell);
char* s2c_cap_encode(const S2CCap* cap, size_t* length);
bool s2c_cap_decode(S2CCap* cap, const char* data, size_t length);

// S2Cell functions
S2CCell* s2c_cell_new(const S2CCellId* cellid);
void s2c_cell_destroy(S2CCell* cell);
S2CCellId* s2c_cell_id(const S2CCell* cell);
int s2c_cell_face(const S2CCell* cell);
int s2c_cell_level(const S2CCell* cell);
S2CPoint* s2c_cell_get_center(const S2CCell* cell);
S2CPoint* s2c_cell_get_vertex(const S2CCell* cell, int k);
S2CLatLng* s2c_cell_get_s2latlng_vertex(const S2CCell* cell, int k);
S2CPoint* s2c_cell_get_edge(const S2CCell* cell, int k);
S2CLatLng* s2c_cell_get_s2latlng_edge(const S2CCell* cell, int k);
S2CCap* s2c_cell_get_cap_bound(const S2CCell* cell);
S2CLatLngRect* s2c_cell_get_rect_bound(const S2CCell* cell);
double s2c_cell_exact_area(const S2CCell* cell);
double s2c_cell_approx_area(const S2CCell* cell);
double s2c_cell_average_area(int level);
S1CChordAngle* s2c_cell_get_distance(const S2CCell* cell, const S2CPoint* point);
S1CChordAngle* s2c_cell_get_boundary_distance(const S2CCell* cell, const S2CPoint* point);
bool s2c_cell_contains(const S2CCell* cell, const S2CPoint* point);
bool s2c_cell_contains_cell(const S2CCell* cell, const S2CCell* other);
bool s2c_cell_may_intersect(const S2CCell* a, const S2CCell* b);
S2CCell* s2c_cell_clone(const S2CCell* cell);
char* s2c_cell_encode(const S2CCell* cell, size_t* length);
bool s2c_cell_decode(S2CCell* cell, const char* data, size_t length);

// S2Loop functions
S2CLoop* s2c_loop_new(void);
S2CLoop* s2c_loop_new_from_points(const S2CPoint** vertices, int num_vertices);
S2CLoop* s2c_loop_new_from_latlngs(const S2CLatLng** vertices, int num_vertices);
void s2c_loop_destroy(S2CLoop* loop);
void s2c_loop_init(S2CLoop* loop, const S2CPoint** vertices, int num_vertices);
void s2c_loop_init_from_latlngs(S2CLoop* loop, const S2CLatLng** vertices, int num_vertices);
int s2c_loop_num_vertices(const S2CLoop* loop);
S2CPoint* s2c_loop_vertex(const S2CLoop* loop, int i);
S2CLatLng* s2c_loop_get_s2latlng_vertex(const S2CLoop* loop, int i);
bool s2c_loop_is_valid(const S2CLoop* loop, S2CError* error);
bool s2c_loop_is_normalized(const S2CLoop* loop);
void s2c_loop_normalize(S2CLoop* loop);
int s2c_loop_depth(const S2CLoop* loop);
bool s2c_loop_is_hole(const S2CLoop* loop);
int s2c_loop_sign(const S2CLoop* loop);
bool s2c_loop_is_empty(const S2CLoop* loop);
double s2c_loop_get_area(const S2CLoop* loop);
S2CPoint* s2c_loop_get_centroid(const S2CLoop* loop);
S1CChordAngle* s2c_loop_get_distance(const S2CLoop* loop, const S2CPoint* point);
S2CPoint* s2c_loop_project(const S2CLoop* loop, const S2CPoint* point, S1CChordAngle** distance);
bool s2c_loop_contains(const S2CLoop* loop, const S2CPoint* point);
bool s2c_loop_contains_loop(const S2CLoop* a, const S2CLoop* b);
bool s2c_loop_intersects(const S2CLoop* a, const S2CLoop* b);
bool s2c_loop_equals(const S2CLoop* a, const S2CLoop* b);
S2CCap* s2c_loop_get_cap_bound(const S2CLoop* loop);
S2CLatLngRect* s2c_loop_get_rect_bound(const S2CLoop* loop);
bool s2c_loop_may_intersect_cell(const S2CLoop* loop, const S2CCell* cell);
S2CLoop* s2c_loop_clone(const S2CLoop* loop);
char* s2c_loop_encode(const S2CLoop* loop, size_t* length);
bool s2c_loop_decode(S2CLoop* loop, const char* data, size_t length);

// S2Polyline functions
S2CPolyline* s2c_polyline_new(void);
S2CPolyline* s2c_polyline_new_from_latlngs(const S2CLatLng** vertices, int num_vertices);
S2CPolyline* s2c_polyline_new_from_points(const S2CPoint** vertices, int num_vertices);
void s2c_polyline_destroy(S2CPolyline* polyline);
void s2c_polyline_init_from_s2latlngs(S2CPolyline* polyline, const S2CLatLng** vertices, int num_vertices);
void s2c_polyline_init_from_s2points(S2CPolyline* polyline, const S2CPoint** vertices, int num_vertices);
int s2c_polyline_num_vertices(const S2CPolyline* polyline);
S2CPoint* s2c_polyline_vertex(const S2CPolyline* polyline, int i);
S1CAngle* s2c_polyline_get_length(const S2CPolyline* polyline);
S2CPoint* s2c_polyline_get_centroid(const S2CPolyline* polyline);
S2CPoint* s2c_polyline_interpolate(const S2CPolyline* polyline, double fraction);
double s2c_polyline_uninterpolate(const S2CPolyline* polyline, const S2CPoint* point);
S2CPoint* s2c_polyline_project(const S2CPolyline* polyline, const S2CPoint* point, int* next_vertex);
bool s2c_polyline_is_on_right(const S2CPolyline* polyline, const S2CPoint* point);
bool s2c_polyline_intersects(const S2CPolyline* a, const S2CPolyline* b);
void s2c_polyline_reverse(S2CPolyline* polyline);
S2CPolyline* s2c_polyline_get_suffix(const S2CPolyline* polyline, double fraction);
bool s2c_polyline_is_valid(const S2CPolyline* polyline, S2CError* error);
bool s2c_polyline_approx_equals(const S2CPolyline* a, const S2CPolyline* b);
bool s2c_polyline_contains(const S2CPolyline* polyline, const S2CCell* cell);
bool s2c_polyline_may_intersect_cell(const S2CPolyline* polyline, const S2CCell* cell);
S2CCap* s2c_polyline_get_cap_bound(const S2CPolyline* polyline);
S2CLatLngRect* s2c_polyline_get_rect_bound(const S2CPolyline* polyline);
S2CPolyline* s2c_polyline_clone(const S2CPolyline* polyline);
char* s2c_polyline_encode(const S2CPolyline* polyline, size_t* length);
bool s2c_polyline_decode(S2CPolyline* polyline, const char* data, size_t length);

// S2Polygon functions
S2CPolygon* s2c_polygon_new(void);
S2CPolygon* s2c_polygon_new_from_loop(S2CLoop* loop);
S2CPolygon* s2c_polygon_new_from_loops(S2CLoop** loops, int num_loops);
void s2c_polygon_destroy(S2CPolygon* polygon);
void s2c_polygon_init(S2CPolygon* polygon, S2CLoop* loop);
void s2c_polygon_init_nested(S2CPolygon* polygon, S2CLoop** loops, int num_loops);
void s2c_polygon_init_to_union(S2CPolygon* polygon, S2CPolygon** polygons, int num_polygons);
void s2c_polygon_copy(S2CPolygon* dest, const S2CPolygon* src);
int s2c_polygon_num_loops(const S2CPolygon* polygon);
S2CLoop* s2c_polygon_loop(const S2CPolygon* polygon, int i);
int s2c_polygon_num_vertices(const S2CPolygon* polygon);
int s2c_polygon_get_last_descendant(const S2CPolygon* polygon, int k);
bool s2c_polygon_is_empty(const S2CPolygon* polygon);
bool s2c_polygon_is_valid(const S2CPolygon* polygon, S2CError* error);
double s2c_polygon_get_area(const S2CPolygon* polygon);
S2CPoint* s2c_polygon_get_centroid(const S2CPolygon* polygon);
S1CChordAngle* s2c_polygon_get_distance(const S2CPolygon* polygon, const S2CPoint* point);
S2CPoint* s2c_polygon_project(const S2CPolygon* polygon, const S2CPoint* point);
bool s2c_polygon_contains(const S2CPolygon* polygon, const S2CPoint* point);
bool s2c_polygon_contains_polygon(const S2CPolygon* a, const S2CPolygon* b);
bool s2c_polygon_intersects(const S2CPolygon* a, const S2CPolygon* b);
bool s2c_polygon_boundary_near(const S2CPolygon* polygon, const S2CPoint* point, S1CAngle* max_distance);
bool s2c_polygon_equals(const S2CPolygon* a, const S2CPolygon* b);
void s2c_polygon_get_overlap_fractions(const S2CPolygon* a, const S2CPolygon* b, double* a_fraction, double* b_fraction);
S2CPolyline** s2c_polygon_intersect_with_polyline(const S2CPolygon* polygon, const S2CPolyline* polyline, int* num_polylines);
bool s2c_polygon_may_intersect_cell(const S2CPolygon* polygon, const S2CCell* cell);
S2CCap* s2c_polygon_get_cap_bound(const S2CPolygon* polygon);
S2CLatLngRect* s2c_polygon_get_rect_bound(const S2CPolygon* polygon);
S2CPolygon* s2c_polygon_clone(const S2CPolygon* polygon);
char* s2c_polygon_encode(const S2CPolygon* polygon, size_t* length);
bool s2c_polygon_decode(S2CPolygon* polygon, const char* data, size_t length);

// S2LatLngRect functions
S2CLatLngRect* s2c_latlngrect_new(void);
S2CLatLngRect* s2c_latlngrect_new_from_latlng(const S2CLatLng* lo, const S2CLatLng* hi);
S2CLatLngRect* s2c_latlngrect_from_center_size(const S2CLatLng* center, const S2CLatLng* size);
S2CLatLngRect* s2c_latlngrect_from_point(const S2CLatLng* point);
S2CLatLngRect* s2c_latlngrect_from_point_pair(const S2CLatLng* a, const S2CLatLng* b);
S2CLatLngRect* s2c_latlngrect_empty(void);
S2CLatLngRect* s2c_latlngrect_full(void);
void s2c_latlngrect_destroy(S2CLatLngRect* rect);
S2CLatLng* s2c_latlngrect_lo(const S2CLatLngRect* rect);
S2CLatLng* s2c_latlngrect_hi(const S2CLatLngRect* rect);
S1CInterval* s2c_latlngrect_lat(const S2CLatLngRect* rect);
S1CInterval* s2c_latlngrect_lng(const S2CLatLngRect* rect);
double s2c_latlngrect_lat_lo(const S2CLatLngRect* rect);
double s2c_latlngrect_lat_hi(const S2CLatLngRect* rect);
double s2c_latlngrect_lng_lo(const S2CLatLngRect* rect);
double s2c_latlngrect_lng_hi(const S2CLatLngRect* rect);
bool s2c_latlngrect_is_empty(const S2CLatLngRect* rect);
bool s2c_latlngrect_is_valid(const S2CLatLngRect* rect);
bool s2c_latlngrect_is_point(const S2CLatLngRect* rect);
S2CLatLng* s2c_latlngrect_get_center(const S2CLatLngRect* rect);
S2CLatLng* s2c_latlngrect_get_size(const S2CLatLngRect* rect);
S2CLatLng* s2c_latlngrect_get_vertex(const S2CLatLngRect* rect, int k);
double s2c_latlngrect_area(const S2CLatLngRect* rect);
S2CPoint* s2c_latlngrect_get_centroid(const S2CLatLngRect* rect);
bool s2c_latlngrect_contains(const S2CLatLngRect* rect, const S2CLatLng* point);
bool s2c_latlngrect_contains_rect(const S2CLatLngRect* a, const S2CLatLngRect* b);
bool s2c_latlngrect_intersects(const S2CLatLngRect* a, const S2CLatLngRect* b);
S2CLatLngRect* s2c_latlngrect_union(const S2CLatLngRect* a, const S2CLatLngRect* b);
S2CLatLngRect* s2c_latlngrect_intersection(const S2CLatLngRect* a, const S2CLatLngRect* b);
void s2c_latlngrect_add_point(S2CLatLngRect* rect, const S2CLatLng* point);
S2CLatLngRect* s2c_latlngrect_expanded_by_distance(const S2CLatLngRect* rect, S1CAngle* distance);
S1CAngle* s2c_latlngrect_get_distance(const S2CLatLngRect* rect, const S2CLatLng* point);
bool s2c_latlngrect_approx_equals(const S2CLatLngRect* a, const S2CLatLngRect* b);
bool s2c_latlngrect_may_intersect_cell(const S2CLatLngRect* rect, const S2CCell* cell);
S2CCap* s2c_latlngrect_get_cap_bound(const S2CLatLngRect* rect);
S2CLatLngRect* s2c_latlngrect_get_rect_bound(const S2CLatLngRect* rect);
S2CLatLngRect* s2c_latlngrect_clone(const S2CLatLngRect* rect);
char* s2c_latlngrect_encode(const S2CLatLngRect* rect, size_t* length);
bool s2c_latlngrect_decode(S2CLatLngRect* rect, const char* data, size_t length);

// S2CellUnion functions
S2CCellUnion* s2c_cellunion_new(void);
S2CCellUnion* s2c_cellunion_new_from_ids(const uint64_t* cell_ids, int num_cells);
S2CCellUnion* s2c_cellunion_new_from_cellids(S2CCellId** cellids, int num_cells);
S2CCellUnion* s2c_cellunion_from_normalized(S2CCellId** cellids, int num_cells);
void s2c_cellunion_destroy(S2CCellUnion* cell_union);
void s2c_cellunion_init(S2CCellUnion* cell_union, const uint64_t* cell_ids, int num_cells);
int s2c_cellunion_num_cells(const S2CCellUnion* cell_union);
S2CCellId* s2c_cellunion_cell_id(const S2CCellUnion* cell_union, int i);
uint64_t* s2c_cellunion_cell_ids(const S2CCellUnion* cell_union, int* num_cells);
bool s2c_cellunion_empty(const S2CCellUnion* cell_union);
bool s2c_cellunion_is_normalized(const S2CCellUnion* cell_union);
void s2c_cellunion_normalize(S2CCellUnion* cell_union);
void s2c_cellunion_normalize_in_place(S2CCellId** cellids, int* num_cells);
void s2c_cellunion_denormalize(const S2CCellUnion* cell_union, int min_level, int level_mod, S2CCellId*** output, int* count);
double s2c_cellunion_exact_area(const S2CCellUnion* cell_union);
double s2c_cellunion_approx_area(const S2CCellUnion* cell_union);
bool s2c_cellunion_contains(const S2CCellUnion* cell_union, const S2CPoint* point);
bool s2c_cellunion_contains_cellid(const S2CCellUnion* cell_union, const S2CCellId* cellid);
bool s2c_cellunion_contains_cellunion(const S2CCellUnion* a, const S2CCellUnion* b);
bool s2c_cellunion_intersects(const S2CCellUnion* a, const S2CCellUnion* b);
bool s2c_cellunion_intersects_cellid(const S2CCellUnion* cell_union, const S2CCellId* cellid);
S2CCellUnion* s2c_cellunion_intersection(const S2CCellUnion* a, const S2CCellUnion* b);
S2CCellUnion* s2c_cellunion_get_difference(const S2CCellUnion* a, const S2CCellUnion* b);
bool s2c_cellunion_may_intersect_cell(const S2CCellUnion* cell_union, const S2CCell* cell);
S2CCap* s2c_cellunion_get_cap_bound(const S2CCellUnion* cell_union);
S2CLatLngRect* s2c_cellunion_get_rect_bound(const S2CCellUnion* cell_union);
S2CCellUnion* s2c_cellunion_clone(const S2CCellUnion* cell_union);
char* s2c_cellunion_encode(const S2CCellUnion* cell_union, size_t* length);
bool s2c_cellunion_decode(S2CCellUnion* cell_union, const char* data, size_t length);

// S2RegionCoverer functions
S2CRegionCoverer* s2c_regioncoverer_new(void);
void s2c_regioncoverer_destroy(S2CRegionCoverer* coverer);
int s2c_regioncoverer_max_cells(const S2CRegionCoverer* coverer);
void s2c_regioncoverer_set_max_cells(S2CRegionCoverer* coverer, int max_cells);
int s2c_regioncoverer_min_level(const S2CRegionCoverer* coverer);
void s2c_regioncoverer_set_min_level(S2CRegionCoverer* coverer, int min_level);
int s2c_regioncoverer_max_level(const S2CRegionCoverer* coverer);
void s2c_regioncoverer_set_max_level(S2CRegionCoverer* coverer, int max_level);
void s2c_regioncoverer_set_fixed_level(S2CRegionCoverer* coverer, int fixed_level);
int s2c_regioncoverer_level_mod(const S2CRegionCoverer* coverer);
void s2c_regioncoverer_set_level_mod(S2CRegionCoverer* coverer, int level_mod);
int s2c_regioncoverer_true_max_level(const S2CRegionCoverer* coverer);
void s2c_regioncoverer_get_covering_cap(S2CRegionCoverer* coverer, const S2CCap* cap, S2CCellId*** covering, int* count);
void s2c_regioncoverer_get_covering_rect(S2CRegionCoverer* coverer, const S2CLatLngRect* rect, S2CCellId*** covering, int* count);
void s2c_regioncoverer_get_covering_cell(S2CRegionCoverer* coverer, const S2CCell* cell, S2CCellId*** covering, int* count);
void s2c_regioncoverer_get_covering_polygon(S2CRegionCoverer* coverer, const S2CPolygon* polygon, S2CCellId*** covering, int* count);
void s2c_regioncoverer_get_interior_covering_cap(S2CRegionCoverer* coverer, const S2CCap* cap, S2CCellId*** interior, int* count);
void s2c_regioncoverer_get_interior_covering_rect(S2CRegionCoverer* coverer, const S2CLatLngRect* rect, S2CCellId*** interior, int* count);
void s2c_regioncoverer_get_interior_covering_cell(S2CRegionCoverer* coverer, const S2CCell* cell, S2CCellId*** interior, int* count);
void s2c_regioncoverer_get_interior_covering_polygon(S2CRegionCoverer* coverer, const S2CPolygon* polygon, S2CCellId*** interior, int* count);

// S2RegionTermIndexer functions
S2CRegionTermIndexer* s2c_regiontermindexer_new(void);
void s2c_regiontermindexer_destroy(S2CRegionTermIndexer* indexer);
int s2c_regiontermindexer_max_cells(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_max_cells(S2CRegionTermIndexer* indexer, int max_cells);
int s2c_regiontermindexer_min_level(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_min_level(S2CRegionTermIndexer* indexer, int min_level);
int s2c_regiontermindexer_max_level(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_max_level(S2CRegionTermIndexer* indexer, int max_level);
void s2c_regiontermindexer_set_fixed_level(S2CRegionTermIndexer* indexer, int fixed_level);
int s2c_regiontermindexer_level_mod(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_level_mod(S2CRegionTermIndexer* indexer, int level_mod);
int s2c_regiontermindexer_true_max_level(const S2CRegionTermIndexer* indexer);
bool s2c_regiontermindexer_index_contains_points_only(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_index_contains_points_only(S2CRegionTermIndexer* indexer, bool value);
bool s2c_regiontermindexer_optimize_for_space(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_optimize_for_space(S2CRegionTermIndexer* indexer, bool value);
char s2c_regiontermindexer_marker_character(const S2CRegionTermIndexer* indexer);
void s2c_regiontermindexer_set_marker_character(S2CRegionTermIndexer* indexer, char ch);
char** s2c_regiontermindexer_get_index_terms_for_point(S2CRegionTermIndexer* indexer, const S2CPoint* point, const char* prefix, int* count);
char** s2c_regiontermindexer_get_index_terms_for_cap(S2CRegionTermIndexer* indexer, const S2CCap* cap, const char* prefix, int* count);
char** s2c_regiontermindexer_get_index_terms_for_rect(S2CRegionTermIndexer* indexer, const S2CLatLngRect* rect, const char* prefix, int* count);
char** s2c_regiontermindexer_get_index_terms_for_polygon(S2CRegionTermIndexer* indexer, const S2CPolygon* polygon, const char* prefix, int* count);
char** s2c_regiontermindexer_get_index_terms_for_canonical_covering(S2CRegionTermIndexer* indexer, const S2CCellUnion* covering, const char* prefix, int* count);
char** s2c_regiontermindexer_get_query_terms_for_point(S2CRegionTermIndexer* indexer, const S2CPoint* point, const char* prefix, int* count);
char** s2c_regiontermindexer_get_query_terms_for_cap(S2CRegionTermIndexer* indexer, const S2CCap* cap, const char* prefix, int* count);
char** s2c_regiontermindexer_get_query_terms_for_rect(S2CRegionTermIndexer* indexer, const S2CLatLngRect* rect, const char* prefix, int* count);
char** s2c_regiontermindexer_get_query_terms_for_polygon(S2CRegionTermIndexer* indexer, const S2CPolygon* polygon, const char* prefix, int* count);
char** s2c_regiontermindexer_get_query_terms_for_canonical_covering(S2CRegionTermIndexer* indexer, const S2CCellUnion* covering, const char* prefix, int* count);

// S2Builder functions
S2CBuilder* s2c_builder_new(void);
void s2c_builder_destroy(S2CBuilder* builder);
void s2c_builder_start_layer(S2CBuilder* builder, S2CBuilderLayer* layer);
void s2c_builder_add_edge(S2CBuilder* builder, const S2CPoint* v0, const S2CPoint* v1);
bool s2c_builder_build(S2CBuilder* builder, S2CError* error);

// S2BuilderLayer (base for layers)
void s2c_builder_layer_destroy(S2CBuilderLayer* layer);

// S2PolygonLayer functions
S2CPolygonLayer* s2c_polygon_layer_new(S2CPolygon* polygon);
S2CPolygonLayer* s2c_polygon_layer_new_with_options(S2CPolygon* polygon, S2CEdgeType edge_type);
S2CBuilderLayer* s2c_polygon_layer_as_builder_layer(S2CPolygonLayer* layer);

// S2BooleanOperation functions
S2CBooleanOperation* s2c_boolean_operation_new(S2CBooleanOpType op_type, S2CBuilderLayer* layer);
void s2c_boolean_operation_destroy(S2CBooleanOperation* op);
bool s2c_boolean_operation_build(S2CBooleanOperation* op, S2CError* error);

// S2BufferOperation functions
S2CBufferOperation* s2c_buffer_operation_new(S2CBuilderLayer* layer);
S2CBufferOperation* s2c_buffer_operation_new_with_options(S2CBuilderLayer* layer, S1CAngle* buffer_radius, double error_fraction);
void s2c_buffer_operation_destroy(S2CBufferOperation* op);
void s2c_buffer_operation_add_polygon(S2CBufferOperation* op, S2CPolygon* polygon);
void s2c_buffer_operation_add_point(S2CBufferOperation* op, const S2CPoint* point);
bool s2c_buffer_operation_build(S2CBufferOperation* op, S2CError* error);

// S2MutableShapeIndex functions
S2CMutableShapeIndex* s2c_mutable_shape_index_new(void);
void s2c_mutable_shape_index_destroy(S2CMutableShapeIndex* index);
void s2c_mutable_shape_index_add_polygon(S2CMutableShapeIndex* index, S2CPolygon* polygon);

// S2 utility functions
S1CAngle* s2c_interpolate(double t, const S2CPoint* a, const S2CPoint* b);
int s2c_crossing_sign(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c, const S2CPoint* d);
bool s2c_get_intersection(const S2CPoint* a0, const S2CPoint* a1, const S2CPoint* b0, const S2CPoint* b1, S2CPoint** x);
S2CPoint* s2c_rotate(const S2CPoint* point, const S2CPoint* axis, S1CAngle* angle);
S1CAngle* s2c_turn_angle(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c);
bool s2c_update_min_distance(const S2CPoint* x, const S2CPoint* a, const S2CPoint* b, S1CChordAngle** min_dist);
bool s2c_ordered_ccw(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c, const S2CPoint* o);

// S2Earth functions
S1CAngle* s2c_earth_radius_angle(void);
double s2c_earth_radius_km(void);
double s2c_earth_radius_meters(void);
S1CAngle* s2c_earth_highest_altitude_angle(void);
double s2c_earth_highest_altitude_km(void);
double s2c_earth_highest_altitude_meters(void);
S1CAngle* s2c_earth_lowest_altitude_angle(void);
double s2c_earth_lowest_altitude_km(void);
double s2c_earth_lowest_altitude_meters(void);
double s2c_earth_km_to_radians(double km);
double s2c_earth_radians_to_km(double radians);
double s2c_earth_meters_to_radians(double meters);
double s2c_earth_radians_to_meters(double radians);
double s2c_earth_square_km_to_steradians(double km2);
double s2c_earth_steradians_to_square_km(double steradians);
double s2c_earth_square_meters_to_steradians(double m2);
double s2c_earth_steradians_to_square_meters(double steradians);
S1CAngle* s2c_earth_get_distance_latlng(const S2CLatLng* a, const S2CLatLng* b);
S1CAngle* s2c_earth_get_distance_point(const S2CPoint* a, const S2CPoint* b);
double s2c_earth_get_distance_km_latlng(const S2CLatLng* a, const S2CLatLng* b);
double s2c_earth_get_distance_km_point(const S2CPoint* a, const S2CPoint* b);
double s2c_earth_get_distance_meters_latlng(const S2CLatLng* a, const S2CLatLng* b);
double s2c_earth_get_distance_meters_point(const S2CPoint* a, const S2CPoint* b);
S1CAngle* s2c_earth_get_initial_bearing(const S2CLatLng* a, const S2CLatLng* b);
S1CAngle* s2c_earth_to_angle_meters(double meters);
S1CChordAngle* s2c_earth_to_chord_angle_meters(double meters);
double s2c_earth_to_distance_angle(const S1CAngle* angle);
double s2c_earth_to_distance_chord_angle(const S1CChordAngle* angle);
double s2c_earth_to_km_angle(const S1CAngle* angle);
double s2c_earth_to_km_chord_angle(const S1CChordAngle* angle);
double s2c_earth_to_meters_angle(const S1CAngle* angle);
double s2c_earth_to_meters_chord_angle(const S1CChordAngle* angle);
double s2c_earth_to_radians_meters(double meters);
double s2c_earth_to_longitude_radians_meters(double meters, double latitude_radians);

// Memory management helpers
void s2c_free_string(char* str);
void s2c_free_string_array(char** array, int count);
void s2c_free_cellid_array(S2CCellId** array, int count);
void s2c_free_polyline_array(S2CPolyline** array, int count);

// Constants
extern const int S2C_MAX_CELL_LEVEL;

#ifdef __cplusplus
}
#endif

#endif // S2C_H