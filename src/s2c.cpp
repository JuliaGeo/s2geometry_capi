#include "s2c.h"
#include <cstring>
#include <vector>
#include <memory>

// S2 includes
#include "s2/r1interval.h"
#include "s2/s1angle.h"
#include "s2/s1chord_angle.h"
#include "s2/s1interval.h"
#include "s2/s2point.h"
#include "s2/s2latlng.h"
#include "s2/s2cell_id.h"
#include "s2/s2cell.h"
#include "s2/s2cap.h"
#include "s2/s2loop.h"
#include "s2/s2polyline.h"
#include "s2/s2polygon.h"
#include "s2/s2latlng_rect.h"
#include "s2/s2cell_union.h"
#include "s2/s2region_coverer.h"
#include "s2/s2builder.h"
#include "s2/s2builderutil_s2polygon_layer.h"
#include "s2/s2builderutil_snap_functions.h"
#include "s2/s2boolean_operation.h"
#include "s2/s2buffer_operation.h"
#include "s2/s2earth.h"
#include "s2/s2edge_crossings.h"
#include "s2/s2predicates.h"
#include "s2/mutable_s2shape_index.h"
#include "s2/s2shape_index.h"
#include "s2/s2contains_point_query.h"
#include "s2/s2closest_edge_query.h"
#include "s2/s2crossing_edge_query.h"
#include "s2/s2point_vector_shape.h"
#include "s2/s2lax_polyline_shape.h"
#include "s2/s2shapeutil_shape_edge_id.h"

// Wrapper structures
struct S2CPoint { S2Point point; };
struct S2CLatLng { S2LatLng latlng; };
struct S2CCellId { S2CellId cellid; };
struct S2CCell { S2Cell cell; };
struct S2CCap { S2Cap cap; };
struct S2CLoop { std::unique_ptr<S2Loop> loop; };
struct S2CPolyline { std::unique_ptr<S2Polyline> polyline; };
struct S2CPolygon { std::unique_ptr<S2Polygon> polygon; };
struct S2CLatLngRect { S2LatLngRect rect; };
struct S2CCellUnion { S2CellUnion cell_union; };
struct S2CRegionCoverer { S2RegionCoverer coverer; };
struct S2CBuilder { S2Builder builder; };
struct S2CBuilderLayer { std::unique_ptr<S2Builder::Layer> layer; };
struct S2CPolygonLayer { s2builderutil::S2PolygonLayer* layer; };
struct S2CBooleanOperation { std::unique_ptr<S2BooleanOperation> op; };
struct S2CBooleanOperationOptions { S2BooleanOperation::Options options; };
struct S2CBufferOperation { std::unique_ptr<S2BufferOperation> op; };
struct S2CMutableShapeIndex { MutableS2ShapeIndex index; };
struct S2CShapeIndex { MutableS2ShapeIndex index; };  // Use MutableS2ShapeIndex as concrete type
struct S2CContainsPointQuery { 
    std::unique_ptr<S2ContainsPointQuery<MutableS2ShapeIndex>> query; 
    const MutableS2ShapeIndex* index_ptr;
};
struct S2CClosestEdgeQuery { 
    std::unique_ptr<S2ClosestEdgeQuery> query;
    const MutableS2ShapeIndex* index_ptr;
};
struct S2CClosestEdgeResult {
    S1ChordAngle distance;
    int shape_id;
    int edge_id;
    S2Point edge_point;
};
struct S2CCrossingEdgeQuery {
    std::unique_ptr<S2CrossingEdgeQuery> query;
    const MutableS2ShapeIndex* index_ptr;
};
struct S2CCrossingEdgePair {
    int shape_id;
    int edge_id;
    S2Point a;
    S2Point b;
};
struct S1CAngle { S1Angle angle; };
struct S1CChordAngle { S1ChordAngle angle; };
struct R1CInterval { R1Interval interval; };
struct S1CInterval { S1Interval interval; };

// Constants
const int S2C_MAX_CELL_LEVEL = S2CellId::kMaxLevel;

// Helper functions
static char* copy_string(const std::string& str) {
    char* result = (char*)malloc(str.length() + 1);
    if (result) {
        strcpy(result, str.c_str());
    }
    return result;
}

// S2Point functions
S2CPoint* s2c_point_new(double x, double y, double z) {
    auto* p = new S2CPoint;
    p->point = S2Point(x, y, z);
    return p;
}

S2CPoint* s2c_point_from_latlng(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* p = new S2CPoint;
    p->point = latlng->latlng.ToPoint();
    return p;
}

void s2c_point_destroy(S2CPoint* point) {
    delete point;
}

double s2c_point_x(const S2CPoint* point) {
    return point ? point->point.x() : 0.0;
}

double s2c_point_y(const S2CPoint* point) {
    return point ? point->point.y() : 0.0;
}

double s2c_point_z(const S2CPoint* point) {
    return point ? point->point.z() : 0.0;
}

void s2c_point_get_coords(const S2CPoint* point, double* x, double* y, double* z) {
    if (point) {
        if (x) *x = point->point.x();
        if (y) *y = point->point.y();
        if (z) *z = point->point.z();
    }
}

double s2c_point_norm(const S2CPoint* point) {
    return point ? point->point.Norm() : 0.0;
}

S2CPoint* s2c_point_normalize(const S2CPoint* point) {
    if (!point) return nullptr;
    auto* p = new S2CPoint;
    p->point = point->point.Normalize();
    return p;
}

// S2LatLng functions
S2CLatLng* s2c_latlng_new(const S1CAngle* lat, const S1CAngle* lng) {
    if (!lat || !lng) return nullptr;
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng(lat->angle, lng->angle);
    return ll;
}

S2CLatLng* s2c_latlng_from_degrees(double lat_degrees, double lng_degrees) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromDegrees(lat_degrees, lng_degrees);
    return ll;
}

S2CLatLng* s2c_latlng_from_radians(double lat_radians, double lng_radians) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromRadians(lat_radians, lng_radians);
    return ll;
}

S2CLatLng* s2c_latlng_from_point(const S2CPoint* point) {
    if (!point) return nullptr;
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng(point->point);
    return ll;
}

void s2c_latlng_destroy(S2CLatLng* latlng) {
    delete latlng;
}

S1CAngle* s2c_latlng_lat(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* angle = new S1CAngle;
    angle->angle = latlng->latlng.lat();
    return angle;
}

S1CAngle* s2c_latlng_lng(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* angle = new S1CAngle;
    angle->angle = latlng->latlng.lng();
    return angle;
}

double s2c_latlng_lat_degrees(const S2CLatLng* latlng) {
    return latlng ? latlng->latlng.lat().degrees() : 0.0;
}

double s2c_latlng_lng_degrees(const S2CLatLng* latlng) {
    return latlng ? latlng->latlng.lng().degrees() : 0.0;
}

double s2c_latlng_lat_radians(const S2CLatLng* latlng) {
    return latlng ? latlng->latlng.lat().radians() : 0.0;
}

double s2c_latlng_lng_radians(const S2CLatLng* latlng) {
    return latlng ? latlng->latlng.lng().radians() : 0.0;
}

S2CPoint* s2c_latlng_to_point(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* p = new S2CPoint;
    p->point = latlng->latlng.ToPoint();
    return p;
}

bool s2c_latlng_is_valid(const S2CLatLng* latlng) {
    return latlng ? latlng->latlng.is_valid() : false;
}

// Additional S2LatLng functions - E6/E7 Constructors
S2CLatLng* s2c_latlng_from_e6(int32_t lat_e6, int32_t lng_e6) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromE6(lat_e6, lng_e6);
    return ll;
}

S2CLatLng* s2c_latlng_from_e7(int32_t lat_e7, int32_t lng_e7) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromE7(lat_e7, lng_e7);
    return ll;
}

S2CLatLng* s2c_latlng_from_unsigned_e6(uint32_t lat_e6, uint32_t lng_e6) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromUnsignedE6(lat_e6, lng_e6);
    return ll;
}

S2CLatLng* s2c_latlng_from_unsigned_e7(uint32_t lat_e7, uint32_t lng_e7) {
    auto* ll = new S2CLatLng;
    ll->latlng = S2LatLng::FromUnsignedE7(lat_e7, lng_e7);
    return ll;
}

// Other S2LatLng functions
S2CLatLng* s2c_latlng_normalized(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* ll = new S2CLatLng;
    ll->latlng = latlng->latlng.Normalized();
    return ll;
}

char* s2c_latlng_to_string_in_degrees(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    std::string str = latlng->latlng.ToStringInDegrees();
    char* result = (char*)malloc(str.size() + 1);
    strcpy(result, str.c_str());
    return result;
}

bool s2c_latlng_approx_equals(const S2CLatLng* a, const S2CLatLng* b) {
    return a && b ? a->latlng.ApproxEquals(b->latlng) : false;
}

// Get distance between two lat/lng points
S1CAngle* s2c_latlng_get_distance(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return nullptr;
    auto* angle = new S1CAngle;
    angle->angle = a->latlng.GetDistance(b->latlng);
    return angle;
}

// S1Angle functions
S1CAngle* s1c_angle_new(void) {
    return new S1CAngle;
}

S1CAngle* s1c_angle_from_radians(double radians) {
    auto* angle = new S1CAngle;
    angle->angle = S1Angle::Radians(radians);
    return angle;
}

S1CAngle* s1c_angle_from_degrees(double degrees) {
    auto* angle = new S1CAngle;
    angle->angle = S1Angle::Degrees(degrees);
    return angle;
}

S1CAngle* s1c_angle_from_e5(int32_t e5) {
    auto* angle = new S1CAngle;
    angle->angle = S1Angle::E5(e5);
    return angle;
}

S1CAngle* s1c_angle_from_e6(int32_t e6) {
    auto* angle = new S1CAngle;
    angle->angle = S1Angle::E6(e6);
    return angle;
}

S1CAngle* s1c_angle_from_e7(int32_t e7) {
    auto* angle = new S1CAngle;
    angle->angle = S1Angle::E7(e7);
    return angle;
}

void s1c_angle_destroy(S1CAngle* angle) {
    delete angle;
}

double s1c_angle_radians(const S1CAngle* angle) {
    return angle ? angle->angle.radians() : 0.0;
}

double s1c_angle_degrees(const S1CAngle* angle) {
    return angle ? angle->angle.degrees() : 0.0;
}

int32_t s1c_angle_e5(const S1CAngle* angle) {
    return angle ? angle->angle.e5() : 0;
}

int32_t s1c_angle_e6(const S1CAngle* angle) {
    return angle ? angle->angle.e6() : 0;
}

int32_t s1c_angle_e7(const S1CAngle* angle) {
    return angle ? angle->angle.e7() : 0;
}

S1CAngle* s1c_angle_abs(const S1CAngle* angle) {
    if (!angle) return nullptr;
    auto* result = new S1CAngle;
    result->angle = angle->angle.abs();
    return result;
}

S1CAngle* s1c_angle_normalize(const S1CAngle* angle) {
    if (!angle) return nullptr;
    auto* result = new S1CAngle;
    result->angle = angle->angle;
    result->angle.Normalize();
    return result;
}

S1CAngle* s1c_angle_normalized(const S1CAngle* angle) {
    if (!angle) return nullptr;
    auto* result = new S1CAngle;
    result->angle = angle->angle.Normalized();
    return result;
}

// S1ChordAngle functions
S1CChordAngle* s1c_chordangle_new(void) {
    return new S1CChordAngle;
}

S1CChordAngle* s1c_chordangle_infinity(void) {
    auto* angle = new S1CChordAngle;
    angle->angle = S1ChordAngle::Infinity();
    return angle;
}

void s1c_chordangle_destroy(S1CChordAngle* angle) {
    delete angle;
}

S1CAngle* s1c_chordangle_to_angle(const S1CChordAngle* angle) {
    if (!angle) return nullptr;
    auto* a = new S1CAngle;
    a->angle = angle->angle.ToAngle();
    return a;
}

double s1c_chordangle_degrees(const S1CChordAngle* angle) {
    return angle ? angle->angle.degrees() : 0.0;
}

// R1Interval functions
R1CInterval* r1c_interval_new(void) {
    auto* interval = new R1CInterval;
    interval->interval = R1Interval::Empty();
    return interval;
}

R1CInterval* r1c_interval_new_from_bounds(double lo, double hi) {
    auto* interval = new R1CInterval;
    interval->interval = R1Interval(lo, hi);
    return interval;
}

void r1c_interval_destroy(R1CInterval* interval) {
    delete interval;
}

double r1c_interval_lo(const R1CInterval* interval) {
    return interval ? interval->interval.lo() : 0.0;
}

double r1c_interval_hi(const R1CInterval* interval) {
    return interval ? interval->interval.hi() : 0.0;
}

double r1c_interval_get_length(const R1CInterval* interval) {
    return interval ? interval->interval.GetLength() : 0.0;
}

double r1c_interval_get_center(const R1CInterval* interval) {
    return interval ? interval->interval.GetCenter() : 0.0;
}

bool r1c_interval_is_empty(const R1CInterval* interval) {
    return interval ? interval->interval.is_empty() : true;
}

bool r1c_interval_contains(const R1CInterval* interval, double point) {
    return interval ? interval->interval.Contains(point) : false;
}

bool r1c_interval_contains_interval(const R1CInterval* a, const R1CInterval* b) {
    return a && b ? a->interval.Contains(b->interval) : false;
}

bool r1c_interval_intersects(const R1CInterval* a, const R1CInterval* b) {
    return a && b ? a->interval.Intersects(b->interval) : false;
}

R1CInterval* r1c_interval_intersection(const R1CInterval* a, const R1CInterval* b) {
    if (!a || !b) return nullptr;
    auto* result = new R1CInterval;
    result->interval = a->interval.Intersection(b->interval);
    return result;
}

R1CInterval* r1c_interval_union(const R1CInterval* a, const R1CInterval* b) {
    if (!a || !b) return nullptr;
    auto* result = new R1CInterval;
    result->interval = a->interval.Union(b->interval);
    return result;
}

// S1Interval functions
S1CInterval* s1c_interval_new(void) {
    auto* interval = new S1CInterval;
    interval->interval = S1Interval::Empty();
    return interval;
}

S1CInterval* s1c_interval_new_from_bounds(double lo, double hi) {
    auto* interval = new S1CInterval;
    interval->interval = S1Interval(lo, hi);
    return interval;
}

void s1c_interval_destroy(S1CInterval* interval) {
    delete interval;
}

double s1c_interval_lo(const S1CInterval* interval) {
    return interval ? interval->interval.lo() : 0.0;
}

double s1c_interval_hi(const S1CInterval* interval) {
    return interval ? interval->interval.hi() : 0.0;
}

double s1c_interval_get_length(const S1CInterval* interval) {
    return interval ? interval->interval.GetLength() : 0.0;
}

bool s1c_interval_is_empty(const S1CInterval* interval) {
    return interval ? interval->interval.is_empty() : true;
}

bool s1c_interval_is_full(const S1CInterval* interval) {
    return interval ? interval->interval.is_full() : false;
}

bool s1c_interval_contains(const S1CInterval* interval, double radians) {
    return interval ? interval->interval.Contains(radians) : false;
}

bool s1c_interval_contains_interval(const S1CInterval* a, const S1CInterval* b) {
    return a && b ? a->interval.Contains(b->interval) : false;
}

bool s1c_interval_intersects(const S1CInterval* a, const S1CInterval* b) {
    return a && b ? a->interval.Intersects(b->interval) : false;
}

S1CInterval* s1c_interval_intersection(const S1CInterval* a, const S1CInterval* b) {
    if (!a || !b) return nullptr;
    auto* result = new S1CInterval;
    result->interval = a->interval.Intersection(b->interval);
    return result;
}

S1CInterval* s1c_interval_union(const S1CInterval* a, const S1CInterval* b) {
    if (!a || !b) return nullptr;
    auto* result = new S1CInterval;
    result->interval = a->interval.Union(b->interval);
    return result;
}

// S2Cap functions
S2CCap* s2c_cap_new(void) {
    return new S2CCap;
}

S2CCap* s2c_cap_from_center_height(const S2CPoint* center, double height) {
    if (!center) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = S2Cap::FromCenterHeight(center->point, height);
    return cap;
}

S2CCap* s2c_cap_from_center_area(const S2CPoint* center, double area) {
    if (!center) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = S2Cap::FromCenterArea(center->point, area);
    return cap;
}

S2CCap* s2c_cap_from_point(const S2CPoint* point) {
    if (!point) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = S2Cap::FromPoint(point->point);
    return cap;
}

S2CCap* s2c_cap_empty(void) {
    auto* cap = new S2CCap;
    cap->cap = S2Cap::Empty();
    return cap;
}

S2CCap* s2c_cap_full(void) {
    auto* cap = new S2CCap;
    cap->cap = S2Cap::Full();
    return cap;
}

void s2c_cap_destroy(S2CCap* cap) {
    delete cap;
}

S2CPoint* s2c_cap_center(const S2CCap* cap) {
    if (!cap) return nullptr;
    auto* point = new S2CPoint;
    point->point = cap->cap.center();
    return point;
}

double s2c_cap_height(const S2CCap* cap) {
    return cap ? cap->cap.height() : 0.0;
}

bool s2c_cap_is_empty(const S2CCap* cap) {
    return cap ? cap->cap.is_empty() : true;
}

bool s2c_cap_is_full(const S2CCap* cap) {
    return cap ? cap->cap.is_full() : false;
}

bool s2c_cap_is_valid(const S2CCap* cap) {
    return cap ? cap->cap.is_valid() : false;
}

S2CCap* s2c_cap_clone(const S2CCap* cap) {
    if (!cap) return nullptr;
    auto* new_cap = new S2CCap;
    new_cap->cap = cap->cap;
    return new_cap;
}

bool s2c_cap_contains(const S2CCap* cap, const S2CPoint* point) {
    return cap && point ? cap->cap.Contains(point->point) : false;
}

bool s2c_cap_contains_cap(const S2CCap* cap, const S2CCap* other) {
    return cap && other ? cap->cap.Contains(other->cap) : false;
}

bool s2c_cap_intersects(const S2CCap* a, const S2CCap* b) {
    return a && b ? a->cap.Intersects(b->cap) : false;
}

S2CCap* s2c_cap_union(const S2CCap* a, const S2CCap* b) {
    if (!a || !b) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = a->cap.Union(b->cap);
    return cap;
}

S2CCap* s2c_cap_expanded(const S2CCap* cap, S1CAngle* angle) {
    if (!cap || !angle) return nullptr;
    auto* new_cap = new S2CCap;
    new_cap->cap = cap->cap.Expanded(angle->angle);
    return new_cap;
}

S2CCap* s2c_cap_get_cap_bound(const S2CCap* cap) {
    if (!cap) return nullptr;
    auto* bound = new S2CCap;
    bound->cap = cap->cap.GetCapBound();
    return bound;
}

S2CLatLngRect* s2c_cap_get_rect_bound(const S2CCap* cap) {
    if (!cap) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = cap->cap.GetRectBound();
    return rect;
}

S2CPoint* s2c_cap_get_centroid(const S2CCap* cap) {
    if (!cap) return nullptr;
    auto* point = new S2CPoint;
    point->point = cap->cap.GetCentroid();
    return point;
}

void s2c_cap_add_point(S2CCap* cap, const S2CPoint* point) {
    if (cap && point) {
        cap->cap.AddPoint(point->point);
    }
}

bool s2c_cap_approx_equals(const S2CCap* a, const S2CCap* b) {
    return a && b ? a->cap.ApproxEquals(b->cap) : false;
}

double s2c_cap_area(const S2CCap* cap) {
    return cap ? cap->cap.GetArea() : 0.0;
}

// S2Cell functions
S2CCell* s2c_cell_new(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* cell = new S2CCell;
    cell->cell = S2Cell(cellid->cellid);
    return cell;
}

void s2c_cell_destroy(S2CCell* cell) {
    delete cell;
}

S2CCellId* s2c_cell_id(const S2CCell* cell) {
    if (!cell) return nullptr;
    auto* id = new S2CCellId;
    id->cellid = cell->cell.id();
    return id;
}

int s2c_cell_face(const S2CCell* cell) {
    return cell ? cell->cell.face() : -1;
}

int s2c_cell_level(const S2CCell* cell) {
    return cell ? cell->cell.level() : -1;
}

S2CPoint* s2c_cell_get_center(const S2CCell* cell) {
    if (!cell) return nullptr;
    auto* point = new S2CPoint;
    point->point = cell->cell.GetCenter();
    return point;
}

S2CPoint* s2c_cell_get_vertex(const S2CCell* cell, int k) {
    if (!cell || k < 0 || k > 3) return nullptr;
    auto* point = new S2CPoint;
    point->point = cell->cell.GetVertex(k);
    return point;
}

// Additional S2Cell functions - Vertex and Edge Access
S2CLatLng* s2c_cell_get_s2latlng_vertex(const S2CCell* cell, int k) {
    if (!cell || k < 0 || k > 3) return nullptr;
    auto* latlng = new S2CLatLng;
    latlng->latlng = S2LatLng(cell->cell.GetVertex(k));
    return latlng;
}

S2CPoint* s2c_cell_get_edge(const S2CCell* cell, int k) {
    if (!cell || k < 0 || k > 3) return nullptr;
    auto* point = new S2CPoint;
    point->point = cell->cell.GetEdge(k);
    return point;
}

S2CLatLng* s2c_cell_get_s2latlng_edge(const S2CCell* cell, int k) {
    if (!cell || k < 0 || k > 3) return nullptr;
    auto* latlng = new S2CLatLng;
    latlng->latlng = S2LatLng(cell->cell.GetEdge(k));
    return latlng;
}

// Bounds
S2CCap* s2c_cell_get_cap_bound(const S2CCell* cell) {
    if (!cell) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = cell->cell.GetCapBound();
    return cap;
}

S2CLatLngRect* s2c_cell_get_rect_bound(const S2CCell* cell) {
    if (!cell) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = cell->cell.GetRectBound();
    return rect;
}

// Metrics
double s2c_cell_exact_area(const S2CCell* cell) {
    return cell ? cell->cell.ExactArea() : 0.0;
}

double s2c_cell_approx_area(const S2CCell* cell) {
    return cell ? cell->cell.ApproxArea() : 0.0;
}

double s2c_cell_average_area(int level) {
    return S2Cell::AverageArea(level);
}

// Distance and Containment
S1CChordAngle* s2c_cell_get_distance(const S2CCell* cell, const S2CPoint* point) {
    if (!cell || !point) return nullptr;
    auto* distance = new S1CChordAngle;
    distance->angle = cell->cell.GetDistance(point->point);
    return distance;
}

S1CChordAngle* s2c_cell_get_boundary_distance(const S2CCell* cell, const S2CPoint* point) {
    if (!cell || !point) return nullptr;
    auto* distance = new S1CChordAngle;
    distance->angle = cell->cell.GetBoundaryDistance(point->point);
    return distance;
}

bool s2c_cell_contains(const S2CCell* cell, const S2CPoint* point) {
    return cell && point ? cell->cell.Contains(point->point) : false;
}

bool s2c_cell_contains_cell(const S2CCell* cell, const S2CCell* other) {
    return cell && other ? cell->cell.Contains(other->cell) : false;
}

bool s2c_cell_may_intersect(const S2CCell* a, const S2CCell* b) {
    return a && b ? a->cell.MayIntersect(b->cell) : false;
}

// Serialization (basic implementation - full implementation would need encoder/decoder wrappers)
S2CCell* s2c_cell_clone(const S2CCell* cell) {
    if (!cell) return nullptr;
    auto* clone = new S2CCell;
    clone->cell = cell->cell;
    return clone;
}

void s2c_cell_encode(const S2CCell* cell, void* encoder) {
    // TODO: Implement proper encoder wrapper
    (void)cell;
    (void)encoder;
}

bool s2c_cell_decode(void* decoder, S2CCell** cell) {
    // TODO: Implement proper decoder wrapper
    (void)decoder;
    (void)cell;
    return false;
}

// S2CellId functions
S2CCellId* s2c_cellid_new(uint64_t id) {
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId(id);
    return cellid;
}

S2CCellId* s2c_cellid_from_latlng(const S2CLatLng* latlng) {
    if (!latlng) return nullptr;
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId(latlng->latlng);
    return cellid;
}

S2CCellId* s2c_cellid_from_point(const S2CPoint* point) {
    if (!point) return nullptr;
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId(point->point);
    return cellid;
}

void s2c_cellid_destroy(S2CCellId* cellid) {
    delete cellid;
}

bool s2c_cellid_is_valid(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.is_valid() : false;
}

int s2c_cellid_level(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.level() : -1;
}

S2CCellId* s2c_cellid_parent(const S2CCellId* cellid, int level) {
    if (!cellid || level < 0 || level > 30) return nullptr;
    auto* parent = new S2CCellId;
    parent->cellid = cellid->cellid.parent(level);
    return parent;
}

bool s2c_cellid_contains(const S2CCellId* a, const S2CCellId* b) {
    return a && b ? a->cellid.contains(b->cellid) : false;
}

char* s2c_cellid_to_string(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    std::string str = cellid->cellid.ToString();
    char* result = (char*)malloc(str.size() + 1);
    strcpy(result, str.c_str());
    return result;
}

void s2c_cellid_get_all_neighbors(const S2CCellId* cellid, S2CCellId* neighbors[4]) {
    if (!cellid) return;
    S2CellId nb[4];
    cellid->cellid.GetEdgeNeighbors(nb);
    for (int i = 0; i < 4; ++i) {
        neighbors[i] = new S2CCellId;
        neighbors[i]->cellid = nb[i];
    }
}

// Additional S2CellId functions - constructors
S2CCellId* s2c_cellid_begin(int level) {
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::Begin(level);
    return cellid;
}

S2CCellId* s2c_cellid_end(int level) {
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::End(level);
    return cellid;
}

S2CCellId* s2c_cellid_from_face_ij(int face, int i, int j, int level) {
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::FromFaceIJ(face, i, j).parent(level);
    return cellid;
}

S2CCellId* s2c_cellid_from_face_pos_level(int face, uint64_t pos, int level) {
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::FromFacePosLevel(face, pos, level);
    return cellid;
}

S2CCellId* s2c_cellid_from_token(const char* token) {
    if (!token) return nullptr;
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::FromToken(token);
    return cellid;
}

S2CCellId* s2c_cellid_from_debug_string(const char* str) {
    if (!str) return nullptr;
    auto* cellid = new S2CCellId;
    cellid->cellid = S2CellId::FromDebugString(str);
    return cellid;
}

// Accessors
uint64_t s2c_cellid_id(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.id() : 0;
}

int s2c_cellid_face(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.face() : -1;
}

uint64_t s2c_cellid_pos(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.pos() : 0;
}

// Predicates
bool s2c_cellid_is_face(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.is_face() : false;
}

bool s2c_cellid_is_leaf(const S2CCellId* cellid) {
    return cellid ? cellid->cellid.is_leaf() : false;
}

bool s2c_cellid_intersects(const S2CCellId* a, const S2CCellId* b) {
    return a && b ? a->cellid.intersects(b->cellid) : false;
}

// Navigation
S2CCellId* s2c_cellid_child(const S2CCellId* cellid, int position) {
    if (!cellid || position < 0 || position > 3) return nullptr;
    auto* child = new S2CCellId;
    child->cellid = cellid->cellid.child(position);
    return child;
}

S2CCellId* s2c_cellid_child_begin(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* child = new S2CCellId;
    child->cellid = cellid->cellid.child_begin();
    return child;
}

S2CCellId* s2c_cellid_child_end(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* child = new S2CCellId;
    child->cellid = cellid->cellid.child_end();
    return child;
}

int s2c_cellid_child_position(const S2CCellId* cellid, int level) {
    return cellid ? cellid->cellid.child_position(level) : -1;
}

S2CCellId* s2c_cellid_next(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* next = new S2CCellId;
    next->cellid = cellid->cellid.next();
    return next;
}

S2CCellId* s2c_cellid_prev(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* prev = new S2CCellId;
    prev->cellid = cellid->cellid.prev();
    return prev;
}

S2CCellId* s2c_cellid_range_min(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* range_min = new S2CCellId;
    range_min->cellid = cellid->cellid.range_min();
    return range_min;
}

S2CCellId* s2c_cellid_range_max(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* range_max = new S2CCellId;
    range_max->cellid = cellid->cellid.range_max();
    return range_max;
}

// Conversions
S2CLatLng* s2c_cellid_to_latlng(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* latlng = new S2CLatLng;
    latlng->latlng = cellid->cellid.ToLatLng();
    return latlng;
}

S2CPoint* s2c_cellid_to_point(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    auto* point = new S2CPoint;
    point->point = cellid->cellid.ToPoint();
    return point;
}

char* s2c_cellid_to_token(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    std::string token = cellid->cellid.ToToken();
    char* result = (char*)malloc(token.size() + 1);
    strcpy(result, token.c_str());
    return result;
}

void s2c_cellid_get_center_si_ti(const S2CCellId* cellid, int* psi, int* pti) {
    if (!cellid || !psi || !pti) return;
    cellid->cellid.GetCenterSiTi(psi, pti);
}

void s2c_cellid_to_face_ij_orientation(const S2CCellId* cellid, int* pface, int* pi, int* pj, int* orientation) {
    if (!cellid || !pface || !pi || !pj) return;
    int orient = 0;
    *pface = cellid->cellid.ToFaceIJOrientation(pi, pj, &orient);
    if (orientation) *orientation = orient;
}

// Get edge neighbors (rename existing function)
void s2c_cellid_get_edge_neighbors(const S2CCellId* cellid, S2CCellId* neighbors[4]) {
    s2c_cellid_get_all_neighbors(cellid, neighbors);
}

// Append neighbors functions
void s2c_cellid_append_all_neighbors(const S2CCellId* cellid, int nbr_level, S2CCellId*** neighbors, int* count) {
    if (!cellid || !neighbors || !count) return;
    
    std::vector<S2CellId> nbrs;
    cellid->cellid.AppendAllNeighbors(nbr_level, &nbrs);
    
    *count = nbrs.size();
    *neighbors = (S2CCellId**)malloc(sizeof(S2CCellId*) * nbrs.size());
    
    for (size_t i = 0; i < nbrs.size(); ++i) {
        (*neighbors)[i] = new S2CCellId;
        (*neighbors)[i]->cellid = nbrs[i];
    }
}

void s2c_cellid_append_vertex_neighbors(const S2CCellId* cellid, int level, S2CCellId*** neighbors, int* count) {
    if (!cellid || !neighbors || !count) return;
    
    std::vector<S2CellId> nbrs;
    cellid->cellid.AppendVertexNeighbors(level, &nbrs);
    
    *count = nbrs.size();
    *neighbors = (S2CCellId**)malloc(sizeof(S2CCellId*) * nbrs.size());
    
    for (size_t i = 0; i < nbrs.size(); ++i) {
        (*neighbors)[i] = new S2CCellId;
        (*neighbors)[i]->cellid = nbrs[i];
    }
}

// S2Loop functions
S2CLoop* s2c_loop_new(void) {
    auto* loop = new S2CLoop;
    loop->loop = std::make_unique<S2Loop>();
    return loop;
}

S2CLoop* s2c_loop_new_from_points(const S2CPoint** vertices, int num_vertices) {
    if (!vertices || num_vertices < 3) return nullptr;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->point);
        }
    }
    
    auto* loop = new S2CLoop;
    loop->loop = std::make_unique<S2Loop>(points);
    return loop;
}

void s2c_loop_destroy(S2CLoop* loop) {
    delete loop;
}

int s2c_loop_num_vertices(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->num_vertices() : 0;
}

S2CPoint* s2c_loop_vertex(const S2CLoop* loop, int i) {
    if (!loop || !loop->loop || i < 0 || i >= loop->loop->num_vertices()) {
        return nullptr;
    }
    auto* p = new S2CPoint;
    p->point = loop->loop->vertex(i);
    return p;
}

bool s2c_loop_is_valid(const S2CLoop* loop, S2CError* error) {
    if (!loop || !loop->loop) return false;
    bool valid = loop->loop->IsValid();
    if (error) {
        error->ok = valid;
        error->text = valid ? nullptr : copy_string("Loop validation failed");
    }
    return valid;
}

double s2c_loop_get_area(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->GetArea() : 0.0;
}

bool s2c_loop_contains(const S2CLoop* loop, const S2CPoint* point) {
    return loop && loop->loop && point ? loop->loop->Contains(point->point) : false;
}

S2CLoop* s2c_loop_clone(const S2CLoop* loop) {
    if (!loop || !loop->loop) return nullptr;
    auto* new_loop = new S2CLoop;
    new_loop->loop = std::unique_ptr<S2Loop>(loop->loop->Clone());
    return new_loop;
}

// Additional S2Loop functions - Constructors and Initialization
S2CLoop* s2c_loop_new_from_latlngs(const S2CLatLng** vertices, int num_vertices) {
    if (!vertices || num_vertices < 3) return nullptr;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->latlng.ToPoint());
        }
    }
    
    auto* loop = new S2CLoop;
    loop->loop = std::make_unique<S2Loop>(points);
    return loop;
}

void s2c_loop_init(S2CLoop* loop, const S2CPoint** vertices, int num_vertices) {
    if (!loop || !loop->loop || !vertices || num_vertices < 3) return;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->point);
        }
    }
    
    loop->loop->Init(points);
}

void s2c_loop_init_from_latlngs(S2CLoop* loop, const S2CLatLng** vertices, int num_vertices) {
    if (!loop || !loop->loop || !vertices || num_vertices < 3) return;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->latlng.ToPoint());
        }
    }
    
    loop->loop->Init(points);
}

// Vertex Access
S2CLatLng* s2c_loop_get_s2latlng_vertex(const S2CLoop* loop, int i) {
    if (!loop || !loop->loop || i < 0 || i >= loop->loop->num_vertices()) {
        return nullptr;
    }
    auto* latlng = new S2CLatLng;
    latlng->latlng = S2LatLng(loop->loop->vertex(i));
    return latlng;
}

// Properties
bool s2c_loop_is_normalized(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->IsNormalized() : false;
}

void s2c_loop_normalize(S2CLoop* loop) {
    if (loop && loop->loop) {
        loop->loop->Normalize();
    }
}

int s2c_loop_depth(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->depth() : 0;
}

bool s2c_loop_is_hole(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->is_hole() : false;
}

int s2c_loop_sign(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->sign() : 0;
}

bool s2c_loop_is_empty(const S2CLoop* loop) {
    return loop && loop->loop ? loop->loop->is_empty() : true;
}

// Geometric Operations
S2CPoint* s2c_loop_get_centroid(const S2CLoop* loop) {
    if (!loop || !loop->loop) return nullptr;
    auto* centroid = new S2CPoint;
    centroid->point = loop->loop->GetCentroid();
    return centroid;
}

S1CChordAngle* s2c_loop_get_distance(const S2CLoop* loop, const S2CPoint* point) {
    if (!loop || !loop->loop || !point) return nullptr;
    auto* distance = new S1CChordAngle;
    distance->angle = S1ChordAngle(loop->loop->GetDistance(point->point));
    return distance;
}

S2CPoint* s2c_loop_project(const S2CLoop* loop, const S2CPoint* point, S1CChordAngle** distance) {
    if (!loop || !loop->loop || !point) return nullptr;
    
    S2Point projected = loop->loop->Project(point->point);
    
    auto* result = new S2CPoint;
    result->point = projected;
    
    if (distance) {
        // Calculate distance from point to projected point
        *distance = new S1CChordAngle;
        (*distance)->angle = S1ChordAngle(point->point, projected);
    }
    
    return result;
}

bool s2c_loop_contains_loop(const S2CLoop* a, const S2CLoop* b) {
    return a && a->loop && b && b->loop ? a->loop->Contains(*b->loop) : false;
}

bool s2c_loop_intersects(const S2CLoop* a, const S2CLoop* b) {
    return a && a->loop && b && b->loop ? a->loop->Intersects(*b->loop) : false;
}

bool s2c_loop_equals(const S2CLoop* a, const S2CLoop* b) {
    return a && a->loop && b && b->loop ? a->loop->Equals(*b->loop) : false;
}

// Bounds
S2CCap* s2c_loop_get_cap_bound(const S2CLoop* loop) {
    if (!loop || !loop->loop) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = loop->loop->GetCapBound();
    return cap;
}

S2CLatLngRect* s2c_loop_get_rect_bound(const S2CLoop* loop) {
    if (!loop || !loop->loop) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = loop->loop->GetRectBound();
    return rect;
}

bool s2c_loop_may_intersect_cell(const S2CLoop* loop, const S2CCell* cell) {
    return loop && loop->loop && cell ? loop->loop->MayIntersect(cell->cell) : false;
}

// Serialization
void s2c_loop_encode(const S2CLoop* loop, void* encoder) {
    // TODO: Implement proper encoder wrapper
    (void)loop;
    (void)encoder;
}

bool s2c_loop_decode(void* decoder, S2CLoop** loop) {
    // TODO: Implement proper decoder wrapper
    (void)decoder;
    (void)loop;
    return false;
}

// S2Polyline functions
S2CPolyline* s2c_polyline_new(void) {
    auto* polyline = new S2CPolyline;
    polyline->polyline = std::make_unique<S2Polyline>();
    return polyline;
}

S2CPolyline* s2c_polyline_new_from_latlngs(const S2CLatLng** vertices, int num_vertices) {
    if (!vertices || num_vertices < 2) return nullptr;
    
    std::vector<S2LatLng> latlngs;
    latlngs.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            latlngs.push_back(vertices[i]->latlng);
        }
    }
    
    auto* polyline = new S2CPolyline;
    polyline->polyline = std::make_unique<S2Polyline>(latlngs);
    return polyline;
}

S2CPolyline* s2c_polyline_new_from_points(const S2CPoint** vertices, int num_vertices) {
    if (!vertices || num_vertices < 2) return nullptr;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->point);
        }
    }
    
    auto* polyline = new S2CPolyline;
    polyline->polyline = std::make_unique<S2Polyline>(points);
    return polyline;
}

void s2c_polyline_destroy(S2CPolyline* polyline) {
    delete polyline;
}

void s2c_polyline_init_from_s2latlngs(S2CPolyline* polyline, const S2CLatLng** vertices, int num_vertices) {
    if (!polyline || !polyline->polyline || !vertices || num_vertices < 2) return;
    
    std::vector<S2LatLng> latlngs;
    latlngs.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            latlngs.push_back(vertices[i]->latlng);
        }
    }
    
    polyline->polyline->Init(latlngs);
}

void s2c_polyline_init_from_s2points(S2CPolyline* polyline, const S2CPoint** vertices, int num_vertices) {
    if (!polyline || !polyline->polyline || !vertices || num_vertices < 2) return;
    
    std::vector<S2Point> points;
    points.reserve(num_vertices);
    for (int i = 0; i < num_vertices; ++i) {
        if (vertices[i]) {
            points.push_back(vertices[i]->point);
        }
    }
    
    polyline->polyline->Init(points);
}

int s2c_polyline_num_vertices(const S2CPolyline* polyline) {
    return polyline && polyline->polyline ? polyline->polyline->num_vertices() : 0;
}

S2CPoint* s2c_polyline_vertex(const S2CPolyline* polyline, int i) {
    if (!polyline || !polyline->polyline || i < 0 || i >= polyline->polyline->num_vertices()) {
        return nullptr;
    }
    auto* p = new S2CPoint;
    p->point = polyline->polyline->vertex(i);
    return p;
}

S1CAngle* s2c_polyline_get_length(const S2CPolyline* polyline) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* angle = new S1CAngle;
    angle->angle = polyline->polyline->GetLength();
    return angle;
}

S2CPoint* s2c_polyline_get_centroid(const S2CPolyline* polyline) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* p = new S2CPoint;
    p->point = polyline->polyline->GetCentroid();
    return p;
}

S2CPoint* s2c_polyline_interpolate(const S2CPolyline* polyline, double fraction) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* p = new S2CPoint;
    p->point = polyline->polyline->Interpolate(fraction);
    return p;
}

double s2c_polyline_uninterpolate(const S2CPolyline* polyline, const S2CPoint* point) {
    if (!polyline || !polyline->polyline || !point) return -1.0;
    int next_vertex = -1;
    return polyline->polyline->UnInterpolate(point->point, next_vertex);
}

S2CPoint* s2c_polyline_project(const S2CPolyline* polyline, const S2CPoint* point, int* next_vertex) {
    if (!polyline || !polyline->polyline || !point) return nullptr;
    auto* p = new S2CPoint;
    int next = -1;
    p->point = polyline->polyline->Project(point->point, &next);
    if (next_vertex) *next_vertex = next;
    return p;
}

bool s2c_polyline_is_on_right(const S2CPolyline* polyline, const S2CPoint* point) {
    return polyline && polyline->polyline && point ? polyline->polyline->IsOnRight(point->point) : false;
}

bool s2c_polyline_intersects(const S2CPolyline* a, const S2CPolyline* b) {
    return a && a->polyline && b && b->polyline ? a->polyline->Intersects(*b->polyline) : false;
}

void s2c_polyline_reverse(S2CPolyline* polyline) {
    if (polyline && polyline->polyline) {
        polyline->polyline->Reverse();
    }
}

S2CPolyline* s2c_polyline_get_suffix(const S2CPolyline* polyline, double fraction) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* suffix = new S2CPolyline;
    suffix->polyline = std::make_unique<S2Polyline>();
    int next_vertex = -1;
    S2Point first_point = polyline->polyline->GetSuffix(fraction, &next_vertex);
    // Build a new polyline from the suffix
    std::vector<S2Point> points;
    points.push_back(first_point);
    for (int i = next_vertex; i < polyline->polyline->num_vertices(); ++i) {
        points.push_back(polyline->polyline->vertex(i));
    }
    suffix->polyline->Init(points);
    return suffix;
}

bool s2c_polyline_is_valid(const S2CPolyline* polyline, S2CError* error) {
    if (!polyline || !polyline->polyline) return false;
    bool valid = polyline->polyline->IsValid();
    if (error) {
        error->ok = valid;
        error->text = valid ? nullptr : copy_string("Polyline validation failed");
    }
    return valid;
}

bool s2c_polyline_approx_equals(const S2CPolyline* a, const S2CPolyline* b) {
    return a && a->polyline && b && b->polyline ? a->polyline->ApproxEquals(*b->polyline) : false;
}

bool s2c_polyline_contains(const S2CPolyline* polyline, const S2CCell* cell) {
    return polyline && polyline->polyline && cell ? polyline->polyline->Contains(cell->cell) : false;
}

bool s2c_polyline_may_intersect_cell(const S2CPolyline* polyline, const S2CCell* cell) {
    return polyline && polyline->polyline && cell ? polyline->polyline->MayIntersect(cell->cell) : false;
}

S2CCap* s2c_polyline_get_cap_bound(const S2CPolyline* polyline) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = polyline->polyline->GetCapBound();
    return cap;
}

S2CLatLngRect* s2c_polyline_get_rect_bound(const S2CPolyline* polyline) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = polyline->polyline->GetRectBound();
    return rect;
}

S2CPolyline* s2c_polyline_clone(const S2CPolyline* polyline) {
    if (!polyline || !polyline->polyline) return nullptr;
    auto* new_polyline = new S2CPolyline;
    new_polyline->polyline = std::unique_ptr<S2Polyline>(polyline->polyline->Clone());
    return new_polyline;
}

char* s2c_polyline_encode(const S2CPolyline* polyline, size_t* length) {
    // TODO: Implement encoding
    if (length) *length = 0;
    return nullptr;
}

bool s2c_polyline_decode(S2CPolyline* polyline, const char* data, size_t length) {
    // TODO: Implement decoding
    return false;
}

// S2Polygon functions
S2CPolygon* s2c_polygon_new(void) {
    auto* polygon = new S2CPolygon;
    polygon->polygon = std::make_unique<S2Polygon>();
    return polygon;
}

S2CPolygon* s2c_polygon_new_from_loop(S2CLoop* loop) {
    if (!loop || !loop->loop) return nullptr;
    auto* polygon = new S2CPolygon;
    // Transfer ownership of the loop
    polygon->polygon = std::make_unique<S2Polygon>(std::move(loop->loop));
    return polygon;
}

S2CPolygon* s2c_polygon_new_from_loops(S2CLoop** loops, int num_loops) {
    if (!loops || num_loops <= 0) return nullptr;
    
    std::vector<std::unique_ptr<S2Loop>> loop_vec;
    loop_vec.reserve(num_loops);
    
    for (int i = 0; i < num_loops; ++i) {
        if (loops[i] && loops[i]->loop) {
            loop_vec.push_back(std::move(loops[i]->loop));
        }
    }
    
    auto* polygon = new S2CPolygon;
    polygon->polygon = std::make_unique<S2Polygon>();
    polygon->polygon->InitNested(std::move(loop_vec));
    return polygon;
}

void s2c_polygon_destroy(S2CPolygon* polygon) {
    delete polygon;
}

int s2c_polygon_num_loops(const S2CPolygon* polygon) {
    return polygon && polygon->polygon ? polygon->polygon->num_loops() : 0;
}

S2CLoop* s2c_polygon_loop(const S2CPolygon* polygon, int i) {
    if (!polygon || !polygon->polygon || i < 0 || i >= polygon->polygon->num_loops()) {
        return nullptr;
    }
    auto* loop = new S2CLoop;
    loop->loop = std::unique_ptr<S2Loop>(polygon->polygon->loop(i)->Clone());
    return loop;
}

bool s2c_polygon_is_empty(const S2CPolygon* polygon) {
    return polygon && polygon->polygon ? polygon->polygon->is_empty() : true;
}

bool s2c_polygon_is_valid(const S2CPolygon* polygon, S2CError* error) {
    if (!polygon || !polygon->polygon) return false;
    bool valid = polygon->polygon->IsValid();
    if (error) {
        error->ok = valid;
        error->text = valid ? nullptr : copy_string("Polygon validation failed");
    }
    return valid;
}

double s2c_polygon_get_area(const S2CPolygon* polygon) {
    return polygon && polygon->polygon ? polygon->polygon->GetArea() : 0.0;
}

S2CPoint* s2c_polygon_get_centroid(const S2CPolygon* polygon) {
    if (!polygon || !polygon->polygon) return nullptr;
    auto* p = new S2CPoint;
    p->point = polygon->polygon->GetCentroid();
    return p;
}

bool s2c_polygon_contains(const S2CPolygon* polygon, const S2CPoint* point) {
    return polygon && polygon->polygon && point ? polygon->polygon->Contains(point->point) : false;
}

bool s2c_polygon_intersects(const S2CPolygon* a, const S2CPolygon* b) {
    return a && a->polygon && b && b->polygon ? a->polygon->Intersects(*b->polygon) : false;
}

bool s2c_polygon_contains_polygon(const S2CPolygon* a, const S2CPolygon* b) {
    return a && a->polygon && b && b->polygon ? a->polygon->Contains(*b->polygon) : false;
}

bool s2c_polygon_boundary_near(const S2CPolygon* polygon, const S2CPoint* point, S1CAngle* max_distance) {
    if (!polygon || !polygon->polygon || !point || !max_distance) return false;
    // BoundaryNear takes another polygon as parameter, not a point
    // Create a small polygon around the point
    S2Polygon point_polygon(S2Cell(S2CellId(point->point)));
    return polygon->polygon->BoundaryNear(point_polygon, max_distance->angle);
}

bool s2c_polygon_equals(const S2CPolygon* a, const S2CPolygon* b) {
    return a && a->polygon && b && b->polygon ? a->polygon->Equals(*b->polygon) : false;
}

S1CChordAngle* s2c_polygon_get_distance(const S2CPolygon* polygon, const S2CPoint* point) {
    if (!polygon || !polygon->polygon || !point) return nullptr;
    auto* angle = new S1CChordAngle;
    S1Angle s1angle = polygon->polygon->GetDistance(point->point);
    angle->angle = S1ChordAngle(s1angle);
    return angle;
}

S2CPoint* s2c_polygon_project(const S2CPolygon* polygon, const S2CPoint* point) {
    if (!polygon || !polygon->polygon || !point) return nullptr;
    auto* p = new S2CPoint;
    p->point = polygon->polygon->Project(point->point);
    return p;
}

void s2c_polygon_init_to_union(S2CPolygon* polygon, S2CPolygon** polygons, int num_polygons) {
    if (!polygon || !polygon->polygon || !polygons || num_polygons <= 0) return;
    
    // InitToUnion only takes 2 polygons at a time in the basic version
    // For multiple polygons, we need to union them iteratively
    if (num_polygons == 1) {
        if (polygons[0] && polygons[0]->polygon) {
            polygon->polygon->Copy(*polygons[0]->polygon);
        }
    } else if (num_polygons >= 2) {
        if (polygons[0] && polygons[0]->polygon && polygons[1] && polygons[1]->polygon) {
            polygon->polygon->InitToUnion(*polygons[0]->polygon, *polygons[1]->polygon);
            
            // Union with remaining polygons one by one
            for (int i = 2; i < num_polygons; ++i) {
                if (polygons[i] && polygons[i]->polygon) {
                    S2Polygon temp;
                    temp.InitToUnion(*polygon->polygon, *polygons[i]->polygon);
                    polygon->polygon->Copy(temp);
                }
            }
        }
    }
}

void s2c_polygon_copy(S2CPolygon* dest, const S2CPolygon* src) {
    if (!dest || !dest->polygon || !src || !src->polygon) return;
    dest->polygon->Copy(*src->polygon);
}

int s2c_polygon_num_vertices(const S2CPolygon* polygon) {
    return polygon && polygon->polygon ? polygon->polygon->num_vertices() : 0;
}

int s2c_polygon_get_last_descendant(const S2CPolygon* polygon, int k) {
    return polygon && polygon->polygon && k >= 0 && k < polygon->polygon->num_loops() ? 
        polygon->polygon->GetLastDescendant(k) : -1;
}

void s2c_polygon_get_overlap_fractions(const S2CPolygon* a, const S2CPolygon* b, double* a_fraction, double* b_fraction) {
    if (!a || !a->polygon || !b || !b->polygon) {
        if (a_fraction) *a_fraction = 0.0;
        if (b_fraction) *b_fraction = 0.0;
        return;
    }
    
    auto fractions = S2Polygon::GetOverlapFractions(*a->polygon, *b->polygon);
    if (a_fraction) *a_fraction = fractions.first;
    if (b_fraction) *b_fraction = fractions.second;
}

S2CPolyline** s2c_polygon_intersect_with_polyline(const S2CPolygon* polygon, const S2CPolyline* polyline, int* num_polylines) {
    if (!polygon || !polygon->polygon || !polyline || !polyline->polyline || !num_polylines) {
        if (num_polylines) *num_polylines = 0;
        return nullptr;
    }
    
    std::vector<std::unique_ptr<S2Polyline>> result = polygon->polygon->IntersectWithPolyline(*polyline->polyline);
    *num_polylines = result.size();
    
    if (result.empty()) return nullptr;
    
    S2CPolyline** array = (S2CPolyline**)malloc(result.size() * sizeof(S2CPolyline*));
    for (size_t i = 0; i < result.size(); ++i) {
        array[i] = new S2CPolyline;
        array[i]->polyline = std::move(result[i]);
    }
    
    return array;
}

bool s2c_polygon_may_intersect_cell(const S2CPolygon* polygon, const S2CCell* cell) {
    return polygon && polygon->polygon && cell ? polygon->polygon->MayIntersect(cell->cell) : false;
}

S2CCap* s2c_polygon_get_cap_bound(const S2CPolygon* polygon) {
    if (!polygon || !polygon->polygon) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = polygon->polygon->GetCapBound();
    return cap;
}

S2CLatLngRect* s2c_polygon_get_rect_bound(const S2CPolygon* polygon) {
    if (!polygon || !polygon->polygon) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = polygon->polygon->GetRectBound();
    return rect;
}

char* s2c_polygon_encode(const S2CPolygon* polygon, size_t* length) {
    // TODO: Implement encoding
    if (length) *length = 0;
    return nullptr;
}

bool s2c_polygon_decode(S2CPolygon* polygon, const char* data, size_t length) {
    // TODO: Implement decoding
    return false;
}

S2CPolygon* s2c_polygon_clone(const S2CPolygon* polygon) {
    if (!polygon || !polygon->polygon) return nullptr;
    auto* new_polygon = new S2CPolygon;
    new_polygon->polygon = std::unique_ptr<S2Polygon>(polygon->polygon->Clone());
    return new_polygon;
}

// S2Builder functions
S2CBuilder* s2c_builder_new(void) {
    return new S2CBuilder;
}

void s2c_builder_destroy(S2CBuilder* builder) {
    delete builder;
}

void s2c_builder_start_layer(S2CBuilder* builder, S2CBuilderLayer* layer) {
    if (builder && layer && layer->layer) {
        builder->builder.StartLayer(std::move(layer->layer));
    }
}

void s2c_builder_add_edge(S2CBuilder* builder, const S2CPoint* v0, const S2CPoint* v1) {
    if (builder && v0 && v1) {
        builder->builder.AddEdge(v0->point, v1->point);
    }
}

bool s2c_builder_build(S2CBuilder* builder, S2CError* error) {
    if (!builder) return false;
    S2Error s2_error;
    bool result = builder->builder.Build(&s2_error);
    if (error) {
        error->ok = s2_error.ok();
        error->text = copy_string(s2_error.text());
    }
    return result;
}

// S2BuilderLayer functions
void s2c_builder_layer_destroy(S2CBuilderLayer* layer) {
    delete layer;
}

// S2PolygonLayer functions
S2CPolygonLayer* s2c_polygon_layer_new(S2CPolygon* polygon) {
    if (!polygon || !polygon->polygon) return nullptr;
    auto* layer = new S2CPolygonLayer;
    layer->layer = new s2builderutil::S2PolygonLayer(polygon->polygon.get());
    return layer;
}

S2CBuilderLayer* s2c_polygon_layer_as_builder_layer(S2CPolygonLayer* layer) {
    if (!layer || !layer->layer) return nullptr;
    auto* builder_layer = new S2CBuilderLayer;
    builder_layer->layer = std::unique_ptr<S2Builder::Layer>(layer->layer);
    layer->layer = nullptr; // Transfer ownership
    return builder_layer;
}

// Additional S2Cap constructor
S2CCap* s2c_cap_from_center_angle(const S2CPoint* center, const S1CAngle* angle) {
    if (!center || !angle) return nullptr;
    return new S2CCap{S2Cap(center->point, angle->angle)};
}

// Additional S2Cell constructor
S2CCell* s2c_cell_new_from_cellid(const S2CCellId* cellid) {
    if (!cellid) return nullptr;
    return new S2CCell{S2Cell(cellid->cellid)};
}

// Additional S2CellId function
bool s2c_cellid_contains_point(const S2CCellId* cellid, const S2CPoint* point) {
    if (!cellid || !point) return false;
    return S2Cell(cellid->cellid).Contains(point->point);
}

// S2BooleanOperation functions
S2CBooleanOperation* s2c_boolean_operation_new(S2CBooleanOpType op_type, S2CBuilderLayer* layer) {
    if (!layer || !layer->layer) return nullptr;
    
    S2BooleanOperation::OpType s2_op_type;
    switch (op_type) {
        case S2C_BOOLEAN_OP_UNION:
            s2_op_type = S2BooleanOperation::OpType::UNION;
            break;
        case S2C_BOOLEAN_OP_INTERSECTION:
            s2_op_type = S2BooleanOperation::OpType::INTERSECTION;
            break;
        case S2C_BOOLEAN_OP_DIFFERENCE:
            s2_op_type = S2BooleanOperation::OpType::DIFFERENCE;
            break;
        case S2C_BOOLEAN_OP_SYMMETRIC_DIFFERENCE:
            s2_op_type = S2BooleanOperation::OpType::SYMMETRIC_DIFFERENCE;
            break;
        default:
            return nullptr;
    }
    
    auto* op = new S2CBooleanOperation;
    op->op = std::make_unique<S2BooleanOperation>(s2_op_type, std::move(layer->layer));
    return op;
}

void s2c_boolean_operation_destroy(S2CBooleanOperation* op) {
    delete op;
}

bool s2c_boolean_operation_build(S2CBooleanOperation* op, S2CError* error) {
    if (!op || !op->op) return false;
    // This version requires shape indexes to be added via Build method
    if (error) {
        error->ok = false;
        error->text = copy_string("Use s2c_boolean_operation_build_indexes instead");
    }
    return false;
}

// S2BooleanOperation::Options implementation
S2CBooleanOperationOptions* s2c_boolean_operation_options_new(void) {
    return new S2CBooleanOperationOptions;
}

void s2c_boolean_operation_options_destroy(S2CBooleanOperationOptions* options) {
    delete options;
}

void s2c_boolean_operation_options_set_polygon_model(S2CBooleanOperationOptions* options, S2CPolygonModel model) {
    if (!options) return;
    
    S2BooleanOperation::PolygonModel s2_model;
    switch (model) {
        case S2C_POLYGON_MODEL_OPEN:
            s2_model = S2BooleanOperation::PolygonModel::OPEN;
            break;
        case S2C_POLYGON_MODEL_SEMI_OPEN:
            s2_model = S2BooleanOperation::PolygonModel::SEMI_OPEN;
            break;
        case S2C_POLYGON_MODEL_CLOSED:
            s2_model = S2BooleanOperation::PolygonModel::CLOSED;
            break;
        default:
            s2_model = S2BooleanOperation::PolygonModel::SEMI_OPEN;
    }
    options->options.set_polygon_model(s2_model);
}

void s2c_boolean_operation_options_set_polyline_model(S2CBooleanOperationOptions* options, S2CPolylineModel model) {
    if (!options) return;
    
    S2BooleanOperation::PolylineModel s2_model;
    switch (model) {
        case S2C_POLYLINE_MODEL_OPEN:
            s2_model = S2BooleanOperation::PolylineModel::OPEN;
            break;
        case S2C_POLYLINE_MODEL_SEMI_OPEN:
            s2_model = S2BooleanOperation::PolylineModel::SEMI_OPEN;
            break;
        case S2C_POLYLINE_MODEL_CLOSED:
            s2_model = S2BooleanOperation::PolylineModel::CLOSED;
            break;
        default:
            s2_model = S2BooleanOperation::PolylineModel::SEMI_OPEN;
    }
    options->options.set_polyline_model(s2_model);
}

void s2c_boolean_operation_options_set_snap_function(S2CBooleanOperationOptions* options, int snap_level) {
    if (!options || snap_level < 0 || snap_level > 30) return;
    // Create a S2CellIdSnapFunction with the specified level
    auto snap_func = s2builderutil::S2CellIdSnapFunction(snap_level);
    options->options.set_snap_function(snap_func);
}

// Create boolean operation with options
S2CBooleanOperation* s2c_boolean_operation_new_with_options(S2CBooleanOpType op_type, S2CBuilderLayer* layer, 
                                                           const S2CBooleanOperationOptions* options) {
    if (!layer || !layer->layer) return nullptr;
    
    S2BooleanOperation::OpType s2_op_type;
    switch (op_type) {
        case S2C_BOOLEAN_OP_UNION:
            s2_op_type = S2BooleanOperation::OpType::UNION;
            break;
        case S2C_BOOLEAN_OP_INTERSECTION:
            s2_op_type = S2BooleanOperation::OpType::INTERSECTION;
            break;
        case S2C_BOOLEAN_OP_DIFFERENCE:
            s2_op_type = S2BooleanOperation::OpType::DIFFERENCE;
            break;
        case S2C_BOOLEAN_OP_SYMMETRIC_DIFFERENCE:
            s2_op_type = S2BooleanOperation::OpType::SYMMETRIC_DIFFERENCE;
            break;
        default:
            return nullptr;
    }
    
    auto* boolean_op = new S2CBooleanOperation;
    if (options) {
        boolean_op->op = std::make_unique<S2BooleanOperation>(
            s2_op_type, std::move(layer->layer), options->options);
    } else {
        boolean_op->op = std::make_unique<S2BooleanOperation>(
            s2_op_type, std::move(layer->layer));
    }
    return boolean_op;
}

// Build boolean operation with shape indexes
bool s2c_boolean_operation_build_indexes(S2CBooleanOperation* op, const S2CShapeIndex* a, const S2CShapeIndex* b, S2CError* error) {
    if (!op || !op->op || !a || !b) {
        if (error) {
            error->ok = false;
            error->text = copy_string("Invalid parameters for boolean operation build");
        }
        return false;
    }
    
    S2Error s2_error;
    bool result = op->op->Build(a->index, b->index, &s2_error);
    
    if (error) {
        error->ok = s2_error.ok();
        error->text = copy_string(s2_error.text().c_str());
    }
    
    return result;
}

bool s2c_boolean_operation_build_mutable_indexes(S2CBooleanOperation* op, const S2CMutableShapeIndex* a, const S2CMutableShapeIndex* b, S2CError* error) {
    if (!op || !op->op || !a || !b) {
        if (error) {
            error->ok = false;
            error->text = copy_string("Invalid parameters for boolean operation build");
        }
        return false;
    }
    
    S2Error s2_error;
    bool result = op->op->Build(a->index, b->index, &s2_error);
    
    if (error) {
        error->ok = s2_error.ok();
        error->text = copy_string(s2_error.text().c_str());
    }
    
    return result;
}

// S2MutableShapeIndex functions
S2CMutableShapeIndex* s2c_mutable_shape_index_new(void) {
    return new S2CMutableShapeIndex;
}

void s2c_mutable_shape_index_destroy(S2CMutableShapeIndex* index) {
    delete index;
}

void s2c_mutable_shape_index_add_polygon(S2CMutableShapeIndex* index, S2CPolygon* polygon) {
    if (index && polygon && polygon->polygon) {
        auto shape = std::make_unique<S2Polygon::OwningShape>(
            std::unique_ptr<S2Polygon>(polygon->polygon->Clone())
        );
        index->index.Add(std::move(shape));
    }
}

void s2c_mutable_shape_index_add_polyline(S2CMutableShapeIndex* index, S2CPolyline* polyline) {
    if (index && polyline && polyline->polyline) {
        std::vector<S2Point> points;
        for (int i = 0; i < polyline->polyline->num_vertices(); i++) {
            points.push_back(polyline->polyline->vertex(i));
        }
        auto shape = std::make_unique<S2LaxPolylineShape>(points);
        index->index.Add(std::move(shape));
    }
}

void s2c_mutable_shape_index_add_point(S2CMutableShapeIndex* index, const S2CPoint* point) {
    if (index && point) {
        std::vector<S2Point> points = {point->point};
        auto shape = std::make_unique<S2PointVectorShape>(points);
        index->index.Add(std::move(shape));
    }
}

void s2c_mutable_shape_index_add_loop(S2CMutableShapeIndex* index, S2CLoop* loop) {
    if (index && loop && loop->loop) {
        auto shape = std::make_unique<S2Loop::OwningShape>(
            std::unique_ptr<S2Loop>(loop->loop->Clone())
        );
        index->index.Add(std::move(shape));
    }
}

int s2c_mutable_shape_index_num_shape_ids(const S2CMutableShapeIndex* index) {
    return index ? index->index.num_shape_ids() : 0;
}

int s2c_mutable_shape_index_num_edges(const S2CMutableShapeIndex* index) {
    if (!index) return 0;
    int count = 0;
    for (int i = 0; i < index->index.num_shape_ids(); i++) {
        const S2Shape* shape = index->index.shape(i);
        if (shape) count += shape->num_edges();
    }
    return count;
}

void s2c_mutable_shape_index_minimize(S2CMutableShapeIndex* index) {
    if (index) {
        index->index.Minimize();
    }
}

void s2c_mutable_shape_index_force_build(S2CMutableShapeIndex* index) {
    if (index) {
        index->index.ForceBuild();
    }
}

S2CShapeIndex* s2c_mutable_shape_index_snapshot(const S2CMutableShapeIndex* index) {
    if (!index) return nullptr;
    auto* snapshot = new S2CShapeIndex;
    // Since S2CShapeIndex is just a MutableS2ShapeIndex, we can copy shapes
    for (int i = 0; i < index->index.num_shape_ids(); i++) {
        const S2Shape* shape = index->index.shape(i);
        if (shape) {
            // Clone each shape into the new index
            // This requires using the OwningShape wrappers
            S2Shape* cloned_shape = nullptr;
            
            // Try to determine shape type by dimensions and edge count
            if (shape->dimension() == 2) {
                // Likely a polygon shape - create a new owning copy
                std::vector<S2Point> vertices;
                for (int e = 0; e < shape->num_edges(); e++) {
                    S2Shape::Edge edge = shape->edge(e);
                    vertices.push_back(edge.v0);
                }
                if (!vertices.empty()) {
                    vertices.push_back(shape->edge(shape->num_edges() - 1).v1);
                    auto loop = std::make_unique<S2Loop>(vertices);
                    snapshot->index.Add(std::make_unique<S2Loop::OwningShape>(std::move(loop)));
                }
            } else if (shape->dimension() == 1) {
                // Likely a polyline shape
                std::vector<S2Point> vertices;
                for (int e = 0; e < shape->num_edges(); e++) {
                    S2Shape::Edge edge = shape->edge(e);
                    if (e == 0) vertices.push_back(edge.v0);
                    vertices.push_back(edge.v1);
                }
                snapshot->index.Add(std::make_unique<S2LaxPolylineShape>(vertices));
            } else if (shape->dimension() == 0) {
                // Point shape
                std::vector<S2Point> points;
                for (int e = 0; e < shape->num_edges(); e++) {
                    S2Shape::Edge edge = shape->edge(e);
                    points.push_back(edge.v0);
                }
                snapshot->index.Add(std::make_unique<S2PointVectorShape>(points));
            }
        }
    }
    snapshot->index.ForceBuild();
    return snapshot;
}

// S2ShapeIndex functions
S2CShapeIndex* s2c_shape_index_new(void) {
    return new S2CShapeIndex;
}

void s2c_shape_index_destroy(S2CShapeIndex* index) {
    delete index;
}

int s2c_shape_index_num_shape_ids(const S2CShapeIndex* index) {
    return index ? index->index.num_shape_ids() : 0;
}

int s2c_shape_index_num_edges(const S2CShapeIndex* index) {
    if (!index) return 0;
    int count = 0;
    for (int i = 0; i < index->index.num_shape_ids(); i++) {
        const S2Shape* shape = index->index.shape(i);
        if (shape) count += shape->num_edges();
    }
    return count;
}

void s2c_shape_index_minimize(S2CShapeIndex* index) {
    // S2ShapeIndex is immutable, so minimize is a no-op
    // The minimize happens when creating from MutableS2ShapeIndex
}

// S2ContainsPointQuery functions
S2CContainsPointQuery* s2c_contains_point_query_new(const S2CShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CContainsPointQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2ContainsPointQuery<MutableS2ShapeIndex>>(&index->index);
    return query;
}

S2CContainsPointQuery* s2c_contains_point_query_new_mutable(const S2CMutableShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CContainsPointQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2ContainsPointQuery<MutableS2ShapeIndex>>(&index->index);
    return query;
}

void s2c_contains_point_query_destroy(S2CContainsPointQuery* query) {
    delete query;
}

bool s2c_contains_point_query_contains(S2CContainsPointQuery* query, const S2CPoint* point) {
    if (!query || !query->query || !point) return false;
    return query->query->Contains(point->point);
}

int s2c_contains_point_query_containing_shapes(S2CContainsPointQuery* query, const S2CPoint* point, int** shape_ids) {
    if (!query || !query->query || !point || !shape_ids) {
        if (shape_ids) *shape_ids = nullptr;
        return 0;
    }
    
    // GetContainingShapes returns vector<S2Shape*>, we need to extract shape IDs
    auto shapes = query->query->GetContainingShapes(point->point);
    if (shapes.empty()) {
        *shape_ids = nullptr;
        return 0;
    }
    
    *shape_ids = (int*)malloc(sizeof(int) * shapes.size());
    if (*shape_ids) {
        // Find shape IDs by searching the index
        int count = 0;
        for (const S2Shape* shape : shapes) {
            for (int i = 0; i < query->index_ptr->num_shape_ids(); i++) {
                if (query->index_ptr->shape(i) == shape) {
                    (*shape_ids)[count++] = i;
                    break;
                }
            }
        }
        return count;
    }
    return 0;
}

// S2ClosestEdgeQuery functions
S2CClosestEdgeQuery* s2c_closest_edge_query_new(const S2CShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CClosestEdgeQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2ClosestEdgeQuery>(&index->index);
    return query;
}

S2CClosestEdgeQuery* s2c_closest_edge_query_new_mutable(const S2CMutableShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CClosestEdgeQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2ClosestEdgeQuery>(&index->index);
    return query;
}

void s2c_closest_edge_query_destroy(S2CClosestEdgeQuery* query) {
    delete query;
}

void s2c_closest_edge_query_set_max_results(S2CClosestEdgeQuery* query, int max_results) {
    if (query && query->query) {
        query->query->mutable_options()->set_max_results(max_results);
    }
}

void s2c_closest_edge_query_set_max_distance(S2CClosestEdgeQuery* query, const S1CChordAngle* max_distance) {
    if (query && query->query && max_distance) {
        query->query->mutable_options()->set_max_distance(max_distance->angle);
    }
}

void s2c_closest_edge_query_set_max_error(S2CClosestEdgeQuery* query, const S1CAngle* max_error) {
    if (query && query->query && max_error) {
        query->query->mutable_options()->set_max_error(max_error->angle);
    }
}

void s2c_closest_edge_query_set_include_interiors(S2CClosestEdgeQuery* query, bool include_interiors) {
    if (query && query->query) {
        query->query->mutable_options()->set_include_interiors(include_interiors);
    }
}

S2CClosestEdgeResult* s2c_closest_edge_query_find_closest_edge(S2CClosestEdgeQuery* query, const S2CPoint* target) {
    if (!query || !query->query || !target) return nullptr;
    
    S2ClosestEdgeQuery::PointTarget point_target(target->point);
    auto results = query->query->FindClosestEdges(&point_target);
    
    if (results.empty()) return nullptr;
    
    const auto& result = results[0];
    auto* c_result = new S2CClosestEdgeResult;
    c_result->distance = result.distance();
    c_result->shape_id = result.shape_id();
    c_result->edge_id = result.edge_id();
    c_result->edge_point = query->index_ptr->shape(result.shape_id())->edge(result.edge_id()).v0;
    
    return c_result;
}

int s2c_closest_edge_query_find_closest_edges(S2CClosestEdgeQuery* query, const S2CPoint* target, S2CClosestEdgeResult*** results) {
    if (!query || !query->query || !target || !results) {
        if (results) *results = nullptr;
        return 0;
    }
    
    S2ClosestEdgeQuery::PointTarget point_target(target->point);
    auto s2_results = query->query->FindClosestEdges(&point_target);
    
    if (s2_results.empty()) {
        *results = nullptr;
        return 0;
    }
    
    *results = (S2CClosestEdgeResult**)malloc(sizeof(S2CClosestEdgeResult*) * s2_results.size());
    if (!*results) return 0;
    
    for (size_t i = 0; i < s2_results.size(); i++) {
        const auto& result = s2_results[i];
        (*results)[i] = new S2CClosestEdgeResult;
        (*results)[i]->distance = result.distance();
        (*results)[i]->shape_id = result.shape_id();
        (*results)[i]->edge_id = result.edge_id();
        (*results)[i]->edge_point = query->index_ptr->shape(result.shape_id())->edge(result.edge_id()).v0;
    }
    
    return s2_results.size();
}

// S2ClosestEdgeResult accessors
void s2c_closest_edge_result_destroy(S2CClosestEdgeResult* result) {
    delete result;
}

S1CChordAngle* s2c_closest_edge_result_distance(const S2CClosestEdgeResult* result) {
    if (!result) return nullptr;
    return new S1CChordAngle{result->distance};
}

int s2c_closest_edge_result_shape_id(const S2CClosestEdgeResult* result) {
    return result ? result->shape_id : -1;
}

int s2c_closest_edge_result_edge_id(const S2CClosestEdgeResult* result) {
    return result ? result->edge_id : -1;
}

S2CPoint* s2c_closest_edge_result_edge_point(const S2CClosestEdgeResult* result) {
    if (!result) return nullptr;
    return new S2CPoint{result->edge_point};
}

// S2CrossingEdgeQuery functions
S2CCrossingEdgeQuery* s2c_crossing_edge_query_new(const S2CShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CCrossingEdgeQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2CrossingEdgeQuery>(&index->index);
    return query;
}

S2CCrossingEdgeQuery* s2c_crossing_edge_query_new_mutable(const S2CMutableShapeIndex* index) {
    if (!index) return nullptr;
    auto* query = new S2CCrossingEdgeQuery;
    query->index_ptr = &index->index;
    query->query = std::make_unique<S2CrossingEdgeQuery>(&index->index);
    return query;
}

void s2c_crossing_edge_query_destroy(S2CCrossingEdgeQuery* query) {
    delete query;
}

int s2c_crossing_edge_query_get_crossings(S2CCrossingEdgeQuery* query, 
                                          const S2CPoint* a0, const S2CPoint* a1,
                                          int shape_id,
                                          S2CCrossingEdgePair*** crossings) {
    if (!query || !query->query || !a0 || !a1 || !crossings || shape_id < 0) {
        if (crossings) *crossings = nullptr;
        return 0;
    }
    
    const S2Shape* shape = query->index_ptr->shape(shape_id);
    if (!shape) {
        *crossings = nullptr;
        return 0;
    }
    
    // Get all candidate edges for this shape
    auto candidates = query->query->GetCandidates(a0->point, a1->point, *shape);
    
    if (candidates.empty()) {
        *crossings = nullptr;
        return 0;
    }
    
    std::vector<S2CCrossingEdgePair*> results;
    
    for (const auto& shape_edge : candidates) {
        int edge_id = shape_edge.edge_id;
        S2Shape::Edge edge = shape->edge(edge_id);
        if (S2::CrossingSign(a0->point, a1->point, edge.v0, edge.v1) >= 0) {
            auto* pair = new S2CCrossingEdgePair;
            pair->shape_id = shape_id;
            pair->edge_id = edge_id;
            pair->a = edge.v0;
            pair->b = edge.v1;
            results.push_back(pair);
        }
    }
    
    if (results.empty()) {
        *crossings = nullptr;
        return 0;
    }
    
    *crossings = (S2CCrossingEdgePair**)malloc(sizeof(S2CCrossingEdgePair*) * results.size());
    if (*crossings) {
        std::copy(results.begin(), results.end(), *crossings);
    }
    
    return results.size();
}

bool s2c_crossing_edge_query_edge_intersects(S2CCrossingEdgeQuery* query,
                                             const S2CPoint* a0, const S2CPoint* a1) {
    if (!query || !query->query || !a0 || !a1) return false;
    
    // Get all candidate edges from all shapes
    auto candidates = query->query->GetCandidates(a0->point, a1->point);
    
    for (const auto& shape_edge : candidates) {
        int shape_id = shape_edge.shape_id;
        int edge_id = shape_edge.edge_id;
        const S2Shape* shape = query->index_ptr->shape(shape_id);
        if (shape) {
            S2Shape::Edge edge = shape->edge(edge_id);
            if (S2::CrossingSign(a0->point, a1->point, edge.v0, edge.v1) >= 0) {
                return true;
            }
        }
    }
    
    return false;
}

// S2CrossingEdgePair accessors
void s2c_crossing_edge_pair_destroy(S2CCrossingEdgePair* pair) {
    delete pair;
}

int s2c_crossing_edge_pair_shape_id(const S2CCrossingEdgePair* pair) {
    return pair ? pair->shape_id : -1;
}

int s2c_crossing_edge_pair_edge_id(const S2CCrossingEdgePair* pair) {
    return pair ? pair->edge_id : -1;
}

S2CPoint* s2c_crossing_edge_pair_a(const S2CCrossingEdgePair* pair) {
    if (!pair) return nullptr;
    return new S2CPoint{pair->a};
}

S2CPoint* s2c_crossing_edge_pair_b(const S2CCrossingEdgePair* pair) {
    if (!pair) return nullptr;
    return new S2CPoint{pair->b};
}

// Memory management helpers
void s2c_free_string(char* str) {
    free(str);
}

void s2c_free_string_array(char** array, int count) {
    if (array) {
        for (int i = 0; i < count; ++i) {
            free(array[i]);
        }
        free(array);
    }
}

// S2CellUnion functions
// NOTE: This function has been replaced by s2c_cellunion_new_from_cellids below

void s2c_cellunion_destroy(S2CCellUnion* cell_union) {
    delete cell_union;
}

int s2c_cellunion_num_cells(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.num_cells() : 0;
}

bool s2c_cellunion_contains_cellid(const S2CCellUnion* cell_union, const S2CCellId* cellid) {
    return cell_union && cellid ? cell_union->cell_union.Contains(cellid->cellid) : false;
}

// Additional S2CellUnion functions - Constructors
S2CCellUnion* s2c_cellunion_new(void) {
    auto* cell_union = new S2CCellUnion;
    cell_union->cell_union = S2CellUnion();
    return cell_union;
}

S2CCellUnion* s2c_cellunion_new_from_ids(const uint64_t* cell_ids, int num_cells) {
    if (!cell_ids || num_cells <= 0) return nullptr;
    
    std::vector<uint64_t> ids(cell_ids, cell_ids + num_cells);
    auto* cell_union = new S2CCellUnion;
    cell_union->cell_union = S2CellUnion(ids);
    return cell_union;
}

S2CCellUnion* s2c_cellunion_new_from_cellids(const S2CCellId** cellids, int num_cells) {
    if (!cellids || num_cells <= 0) return nullptr;
    
    std::vector<S2CellId> ids;
    ids.reserve(num_cells);
    for (int i = 0; i < num_cells; ++i) {
        if (cellids[i]) {
            ids.push_back(cellids[i]->cellid);
        }
    }
    
    auto* cell_union = new S2CCellUnion;
    cell_union->cell_union = S2CellUnion(ids);
    return cell_union;
}

S2CCellUnion* s2c_cellunion_from_normalized(const S2CCellId** cellids, int num_cells) {
    if (!cellids || num_cells <= 0) return nullptr;
    
    std::vector<S2CellId> ids;
    ids.reserve(num_cells);
    for (int i = 0; i < num_cells; ++i) {
        if (cellids[i]) {
            ids.push_back(cellids[i]->cellid);
        }
    }
    
    auto* cell_union = new S2CCellUnion;
    cell_union->cell_union = S2CellUnion::FromNormalized(std::move(ids));
    return cell_union;
}

void s2c_cellunion_init(S2CCellUnion* cell_union, const uint64_t* cell_ids, int num_cells) {
    if (!cell_union || !cell_ids || num_cells <= 0) return;
    
    std::vector<uint64_t> ids(cell_ids, cell_ids + num_cells);
    cell_union->cell_union.Init(ids);
}

// Accessors
S2CCellId* s2c_cellunion_cell_id(const S2CCellUnion* cell_union, int i) {
    if (!cell_union || i < 0 || i >= cell_union->cell_union.num_cells()) return nullptr;
    auto* cellid = new S2CCellId;
    cellid->cellid = cell_union->cell_union.cell_id(i);
    return cellid;
}

const uint64_t* s2c_cellunion_cell_ids(const S2CCellUnion* cell_union) {
    if (!cell_union) return nullptr;
    
    // We need to create a temporary array of uint64_t ids
    // Note: This is not ideal as we're returning a pointer to temporary data
    // A better approach would be to fill a provided buffer
    static std::vector<uint64_t> ids;
    ids.clear();
    for (const auto& cellid : cell_union->cell_union.cell_ids()) {
        ids.push_back(cellid.id());
    }
    return ids.data();
}

bool s2c_cellunion_empty(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.empty() : true;
}

// Normalization
bool s2c_cellunion_is_normalized(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.IsNormalized() : false;
}

void s2c_cellunion_normalize(S2CCellUnion* cell_union) {
    if (cell_union) {
        cell_union->cell_union.Normalize();
    }
}

void s2c_cellunion_normalize_in_place(S2CCellId** cellids, int* num_cells) {
    if (!cellids || !num_cells || *num_cells <= 0) return;
    
    std::vector<S2CellId> ids;
    ids.reserve(*num_cells);
    for (int i = 0; i < *num_cells; ++i) {
        if (cellids[i]) {
            ids.push_back(cellids[i]->cellid);
            delete cellids[i];  // Clean up old cell ids
        }
    }
    
    S2CellUnion::Normalize(&ids);
    
    *num_cells = ids.size();
    for (size_t i = 0; i < ids.size(); ++i) {
        cellids[i] = new S2CCellId;
        cellids[i]->cellid = ids[i];
    }
}

void s2c_cellunion_denormalize(const S2CCellUnion* cell_union, int min_level, int level_mod, S2CCellId*** output, int* count) {
    if (!cell_union || !output || !count) return;
    
    std::vector<S2CellId> denormalized;
    cell_union->cell_union.Denormalize(min_level, level_mod, &denormalized);
    
    *count = denormalized.size();
    *output = (S2CCellId**)malloc(sizeof(S2CCellId*) * denormalized.size());
    
    for (size_t i = 0; i < denormalized.size(); ++i) {
        (*output)[i] = new S2CCellId;
        (*output)[i]->cellid = denormalized[i];
    }
}

// Metrics
double s2c_cellunion_exact_area(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.ExactArea() : 0.0;
}

double s2c_cellunion_approx_area(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.ApproxArea() : 0.0;
}

// Boolean Operations
bool s2c_cellunion_contains(const S2CCellUnion* cell_union, const S2CPoint* point) {
    return cell_union && point ? cell_union->cell_union.Contains(point->point) : false;
}

bool s2c_cellunion_contains_cellunion(const S2CCellUnion* a, const S2CCellUnion* b) {
    return a && b ? a->cell_union.Contains(b->cell_union) : false;
}

bool s2c_cellunion_intersects(const S2CCellUnion* a, const S2CCellUnion* b) {
    return a && b ? a->cell_union.Intersects(b->cell_union) : false;
}

bool s2c_cellunion_intersects_cellid(const S2CCellUnion* cell_union, const S2CCellId* cellid) {
    return cell_union && cellid ? cell_union->cell_union.Intersects(cellid->cellid) : false;
}

S2CCellUnion* s2c_cellunion_intersection(const S2CCellUnion* a, const S2CCellUnion* b) {
    if (!a || !b) return nullptr;
    auto* result = new S2CCellUnion;
    result->cell_union = a->cell_union.Intersection(b->cell_union);
    return result;
}

S2CCellUnion* s2c_cellunion_get_difference(const S2CCellUnion* a, const S2CCellUnion* b) {
    if (!a || !b) return nullptr;
    auto* result = new S2CCellUnion;
    result->cell_union = a->cell_union.Difference(b->cell_union);
    return result;
}

// Other Operations
bool s2c_cellunion_may_intersect_cell(const S2CCellUnion* cell_union, const S2CCell* cell) {
    return cell_union && cell ? cell_union->cell_union.MayIntersect(cell->cell) : false;
}

S2CCap* s2c_cellunion_get_cap_bound(const S2CCellUnion* cell_union) {
    if (!cell_union) return nullptr;
    auto* cap = new S2CCap;
    cap->cap = cell_union->cell_union.GetCapBound();
    return cap;
}

S2CLatLngRect* s2c_cellunion_get_rect_bound(const S2CCellUnion* cell_union) {
    if (!cell_union) return nullptr;
    auto* rect = new S2CLatLngRect;
    rect->rect = cell_union->cell_union.GetRectBound();
    return rect;
}

S2CCellUnion* s2c_cellunion_clone(const S2CCellUnion* cell_union) {
    if (!cell_union) return nullptr;
    auto* clone = new S2CCellUnion;
    clone->cell_union = cell_union->cell_union;
    return clone;
}

void s2c_cellunion_encode(const S2CCellUnion* cell_union, void* encoder) {
    // TODO: Implement proper encoder wrapper
    (void)cell_union;
    (void)encoder;
}

bool s2c_cellunion_decode(void* decoder, S2CCellUnion** cell_union) {
    // TODO: Implement proper decoder wrapper
    (void)decoder;
    (void)cell_union;
    return false;
}

// S2Earth functions
double s2c_earth_radius_km(void) {
    return S2Earth::RadiusKm();
}

double s2c_earth_radius_meters(void) {
    return S2Earth::RadiusMeters();
}

double s2c_earth_km_to_radians(double km) {
    return S2Earth::KmToRadians(km);
}

double s2c_earth_radians_to_km(double radians) {
    return S2Earth::RadiansToKm(radians);
}

double s2c_earth_meters_to_radians(double meters) {
    return S2Earth::MetersToRadians(meters);
}

double s2c_earth_radians_to_meters(double radians) {
    return S2Earth::RadiansToMeters(radians);
}

// Additional S2Earth functions
double s2c_earth_radius(void) {
    return S2Earth::RadiusMeters();
}

double s2c_earth_highest_altitude(void) {
    return S2Earth::HighestAltitudeMeters();
}

double s2c_earth_highest_altitude_km(void) {
    return S2Earth::HighestAltitudeMeters() / 1000.0;
}

double s2c_earth_highest_altitude_meters(void) {
    return S2Earth::HighestAltitudeMeters();
}

double s2c_earth_lowest_altitude(void) {
    return S2Earth::LowestAltitudeMeters();
}

double s2c_earth_lowest_altitude_km(void) {
    return S2Earth::LowestAltitudeMeters() / 1000.0;
}

double s2c_earth_lowest_altitude_meters(void) {
    return S2Earth::LowestAltitudeMeters();
}

// Area conversions
double s2c_earth_square_km_to_steradians(double km2) {
    return S2Earth::SquareKmToSteradians(km2);
}

double s2c_earth_square_meters_to_steradians(double m2) {
    return S2Earth::SquareMetersToSteradians(m2);
}

double s2c_earth_steradians_to_square_km(double steradians) {
    return S2Earth::SteradiansToSquareKm(steradians);
}

double s2c_earth_steradians_to_square_meters(double steradians) {
    return S2Earth::SteradiansToSquareMeters(steradians);
}

// Bearing and navigation
S1CAngle* s2c_earth_get_initial_bearing(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return nullptr;
    auto* angle = new S1CAngle;
    angle->angle = S2Earth::GetInitialBearing(a->latlng, b->latlng);
    return angle;
}

double s2c_earth_to_longitude_radians(double distance_meters, double latitude_radians) {
    return S2Earth::ToLongitudeRadians(util::units::Meters(distance_meters), latitude_radians);
}

// Angle/Distance conversions
S1CAngle* s2c_earth_to_angle(double meters) {
    auto* angle = new S1CAngle;
    angle->angle = S2Earth::ToAngle(util::units::Meters(meters));
    return angle;
}

S1CChordAngle* s2c_earth_to_chord_angle(double meters) {
    auto* angle = new S1CChordAngle;
    angle->angle = S2Earth::ToChordAngle(util::units::Meters(meters));
    return angle;
}

double s2c_earth_to_distance_angle(const S1CAngle* angle) {
    return angle ? S2Earth::ToDistance(angle->angle).value() : 0.0;
}

double s2c_earth_to_distance_chord_angle(const S1CChordAngle* angle) {
    return angle ? S2Earth::ToDistance(angle->angle).value() : 0.0;
}

double s2c_earth_to_km_angle(const S1CAngle* angle) {
    return angle ? S2Earth::ToKm(angle->angle) : 0.0;
}

double s2c_earth_to_km_chord_angle(const S1CChordAngle* angle) {
    return angle ? S2Earth::ToKm(angle->angle) : 0.0;
}

double s2c_earth_to_meters_angle(const S1CAngle* angle) {
    return angle ? S2Earth::ToMeters(angle->angle) : 0.0;
}

double s2c_earth_to_meters_chord_angle(const S1CChordAngle* angle) {
    return angle ? S2Earth::ToMeters(angle->angle) : 0.0;
}

double s2c_earth_to_radians(double meters) {
    return S2Earth::ToRadians(util::units::Meters(meters));
}

// S2Earth distance functions
S1CAngle* s2c_earth_get_distance_latlng(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return nullptr;
    return new S1CAngle{a->latlng.GetDistance(b->latlng)};
}

S1CAngle* s2c_earth_get_distance_point(const S2CPoint* a, const S2CPoint* b) {
    if (!a || !b) return nullptr;
    return new S1CAngle{S1Angle(a->point, b->point)};
}

double s2c_earth_get_distance_km_latlng(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return 0.0;
    return S2Earth::GetDistanceKm(a->latlng, b->latlng);
}

double s2c_earth_get_distance_km_point(const S2CPoint* a, const S2CPoint* b) {
    if (!a || !b) return 0.0;
    S1Angle angle(a->point, b->point);
    return S2Earth::ToKm(angle);
}

double s2c_earth_get_distance_meters_latlng(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return 0.0;
    return S2Earth::GetDistanceMeters(a->latlng, b->latlng);
}

double s2c_earth_get_distance_meters_point(const S2CPoint* a, const S2CPoint* b) {
    if (!a || !b) return 0.0;
    S1Angle angle(a->point, b->point);
    return S2Earth::ToMeters(angle);
}

// S2 utility functions
int s2c_crossing_sign(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c, const S2CPoint* d) {
    if (!a || !b || !c || !d) return 0;
    return S2::CrossingSign(a->point, b->point, c->point, d->point);
}

S2CPoint* s2c_get_intersection(const S2CPoint* a0, const S2CPoint* a1, const S2CPoint* b0, const S2CPoint* b1) {
    if (!a0 || !a1 || !b0 || !b1) return nullptr;
    S2Point intersection = S2::GetIntersection(a0->point, a1->point, b0->point, b1->point);
    auto* result = new S2CPoint;
    result->point = intersection;
    return result;
}

S2CPoint* s2c_interpolate(double t, const S2CPoint* a, const S2CPoint* b) {
    if (!a || !b) return nullptr;
    S2Point interpolated = S2::Interpolate(a->point, b->point, t);
    auto* result = new S2CPoint;
    result->point = interpolated;
    return result;
}

S2CPoint* s2c_rotate(const S2CPoint* point, const S2CPoint* axis, const S1CAngle* angle) {
    if (!point || !axis || !angle) return nullptr;
    S2Point rotated = S2::Rotate(point->point, axis->point, angle->angle);
    auto* result = new S2CPoint;
    result->point = rotated;
    return result;
}

S1CAngle* s2c_turn_angle(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c) {
    if (!a || !b || !c) return nullptr;
    auto* angle = new S1CAngle;
    // TurnAngle returns the angle by which (b,c) turns from (a,b)
    double cross = S2::RobustCrossProd(a->point, b->point).DotProd(c->point);
    double dot = a->point.DotProd(c->point);
    angle->angle = S1Angle::Radians(atan2(cross, dot));
    return angle;
}

bool s2c_update_min_distance(const S2CPoint* x, const S2CPoint* a, const S2CPoint* b, S1CChordAngle* min_dist) {
    if (!x || !a || !b || !min_dist) return false;
    return S2::UpdateMinDistance(x->point, a->point, b->point, &min_dist->angle);
}

bool s2c_ordered_ccw(const S2CPoint* a, const S2CPoint* b, const S2CPoint* c, const S2CPoint* o) {
    if (!a || !b || !c || !o) return false;
    return s2pred::OrderedCCW(a->point, b->point, c->point, o->point);
}

// Error handling functions
S2CError* s2c_error_new(void) {
    auto* error = new S2CError;
    error->ok = true;
    error->text = nullptr;
    return error;
}

void s2c_error_destroy(S2CError* error) {
    if (error) {
        free(error->text);
        delete error;
    }
}

bool s2c_error_is_ok(const S2CError* error) {
    return error ? error->ok : true;
}

int s2c_error_code(const S2CError* error) {
    return error && !error->ok ? 1 : 0;
}

const char* s2c_error_message(const S2CError* error) {
    return error ? error->text : nullptr;
}

void s2c_error_set(S2CError* error, int code, const char* message) {
    if (error) {
        error->ok = (code == 0);
        free(error->text);
        error->text = message ? copy_string(message) : nullptr;
    }
}

void s2c_error_clear(S2CError* error) {
    if (error) {
        error->ok = true;
        free(error->text);
        error->text = nullptr;
    }
}

void s2c_free_cellid_array(S2CCellId** array, int count) {
    if (array) {
        for (int i = 0; i < count; ++i) {
            delete array[i];
        }
        free(array);
    }
}

void s2c_free_polyline_array(S2CPolyline** array, int count) {
    if (array) {
        for (int i = 0; i < count; ++i) {
            delete array[i];
        }
        free(array);
    }
}

// S2LatLngRect implementation
S2CLatLngRect* s2c_latlngrect_new(void) {
    return new S2CLatLngRect{S2LatLngRect::Empty()};
}

S2CLatLngRect* s2c_latlngrect_new_from_latlng(const S2CLatLng* lo, const S2CLatLng* hi) {
    if (!lo || !hi) return nullptr;
    return new S2CLatLngRect{S2LatLngRect(lo->latlng, hi->latlng)};
}

S2CLatLngRect* s2c_latlngrect_from_center_size(const S2CLatLng* center, const S2CLatLng* size) {
    if (!center || !size) return nullptr;
    return new S2CLatLngRect{S2LatLngRect::FromCenterSize(center->latlng, size->latlng)};
}

S2CLatLngRect* s2c_latlngrect_from_point(const S2CLatLng* point) {
    if (!point) return nullptr;
    return new S2CLatLngRect{S2LatLngRect::FromPoint(point->latlng)};
}

S2CLatLngRect* s2c_latlngrect_from_point_pair(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return nullptr;
    return new S2CLatLngRect{S2LatLngRect::FromPointPair(a->latlng, b->latlng)};
}

S2CLatLngRect* s2c_latlngrect_empty(void) {
    return new S2CLatLngRect{S2LatLngRect::Empty()};
}

S2CLatLngRect* s2c_latlngrect_full(void) {
    return new S2CLatLngRect{S2LatLngRect::Full()};
}

void s2c_latlngrect_destroy(S2CLatLngRect* rect) {
    delete rect;
}

S2CLatLng* s2c_latlngrect_lo(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLng{rect->rect.lo()};
}

S2CLatLng* s2c_latlngrect_hi(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLng{rect->rect.hi()};
}

S1CInterval* s2c_latlngrect_lat(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    R1Interval lat = rect->rect.lat();
    return new S1CInterval{S1Interval(lat.lo(), lat.hi())};
}

S1CInterval* s2c_latlngrect_lng(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    S1Interval lng = rect->rect.lng();
    return new S1CInterval{lng};
}

double s2c_latlngrect_lat_lo(const S2CLatLngRect* rect) {
    return rect ? rect->rect.lat_lo().radians() : 0.0;
}

double s2c_latlngrect_lat_hi(const S2CLatLngRect* rect) {
    return rect ? rect->rect.lat_hi().radians() : 0.0;
}

double s2c_latlngrect_lng_lo(const S2CLatLngRect* rect) {
    return rect ? rect->rect.lng_lo().radians() : 0.0;
}

double s2c_latlngrect_lng_hi(const S2CLatLngRect* rect) {
    return rect ? rect->rect.lng_hi().radians() : 0.0;
}

bool s2c_latlngrect_is_empty(const S2CLatLngRect* rect) {
    return rect ? rect->rect.is_empty() : true;
}

bool s2c_latlngrect_is_valid(const S2CLatLngRect* rect) {
    return rect ? rect->rect.is_valid() : false;
}

bool s2c_latlngrect_is_point(const S2CLatLngRect* rect) {
    return rect ? rect->rect.is_point() : false;
}

S2CLatLng* s2c_latlngrect_get_center(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLng{rect->rect.GetCenter()};
}

S2CLatLng* s2c_latlngrect_get_size(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLng{rect->rect.GetSize()};
}

S2CLatLng* s2c_latlngrect_get_vertex(const S2CLatLngRect* rect, int k) {
    if (!rect || k < 0 || k > 3) return nullptr;
    return new S2CLatLng{rect->rect.GetVertex(k)};
}

double s2c_latlngrect_area(const S2CLatLngRect* rect) {
    return rect ? rect->rect.Area() : 0.0;
}

S2CPoint* s2c_latlngrect_get_centroid(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CPoint{rect->rect.GetCentroid()};
}

bool s2c_latlngrect_contains(const S2CLatLngRect* rect, const S2CLatLng* point) {
    return rect && point ? rect->rect.Contains(point->latlng) : false;
}

bool s2c_latlngrect_contains_rect(const S2CLatLngRect* a, const S2CLatLngRect* b) {
    return a && b ? a->rect.Contains(b->rect) : false;
}

bool s2c_latlngrect_intersects(const S2CLatLngRect* a, const S2CLatLngRect* b) {
    return a && b ? a->rect.Intersects(b->rect) : false;
}

S2CLatLngRect* s2c_latlngrect_union(const S2CLatLngRect* a, const S2CLatLngRect* b) {
    if (!a || !b) return nullptr;
    return new S2CLatLngRect{a->rect.Union(b->rect)};
}

S2CLatLngRect* s2c_latlngrect_intersection(const S2CLatLngRect* a, const S2CLatLngRect* b) {
    if (!a || !b) return nullptr;
    return new S2CLatLngRect{a->rect.Intersection(b->rect)};
}

void s2c_latlngrect_add_point(S2CLatLngRect* rect, const S2CLatLng* point) {
    if (rect && point) {
        rect->rect.AddPoint(point->latlng);
    }
}

S2CLatLngRect* s2c_latlngrect_expanded_by_distance(const S2CLatLngRect* rect, S1CAngle* distance) {
    if (!rect || !distance) return nullptr;
    return new S2CLatLngRect{rect->rect.ExpandedByDistance(distance->angle)};
}

S1CAngle* s2c_latlngrect_get_distance(const S2CLatLngRect* rect, const S2CLatLng* point) {
    if (!rect || !point) return nullptr;
    return new S1CAngle{rect->rect.GetDistance(point->latlng)};
}

bool s2c_latlngrect_approx_equals(const S2CLatLngRect* a, const S2CLatLngRect* b) {
    return a && b ? a->rect.ApproxEquals(b->rect) : false;
}

bool s2c_latlngrect_may_intersect_cell(const S2CLatLngRect* rect, const S2CCell* cell) {
    return rect && cell ? rect->rect.MayIntersect(cell->cell) : false;
}

S2CCap* s2c_latlngrect_get_cap_bound(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CCap{rect->rect.GetCapBound()};
}

S2CLatLngRect* s2c_latlngrect_get_rect_bound(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLngRect{rect->rect.GetRectBound()};
}

S2CLatLngRect* s2c_latlngrect_clone(const S2CLatLngRect* rect) {
    if (!rect) return nullptr;
    return new S2CLatLngRect{rect->rect};
}

char* s2c_latlngrect_encode(const S2CLatLngRect* rect, size_t* length) {
    if (!rect || !length) return nullptr;
    Encoder encoder;
    rect->rect.Encode(&encoder);
    *length = encoder.length();
    char* result = (char*)malloc(*length);
    if (result) {
        memcpy(result, encoder.base(), *length);
    }
    return result;
}

bool s2c_latlngrect_decode(S2CLatLngRect* rect, const char* data, size_t length) {
    if (!rect || !data) return false;
    Decoder decoder(data, length);
    return rect->rect.Decode(&decoder);
}

// S2Earth additional implementations  
S1CChordAngle* s2c_earth_to_chord_angle_meters(double meters) {
    auto* chord = new S1CChordAngle;
    chord->angle = S1ChordAngle(S2Earth::ToAngle(util::units::Meters(meters)));
    return chord;
}

S1CAngle* s2c_earth_to_angle_meters(double meters) {
    auto* angle = new S1CAngle;
    angle->angle = S2Earth::ToAngle(util::units::Meters(meters));
    return angle;
}

double s2c_earth_to_longitude_radians_meters(double meters, double latitude_radians) {
    return S2Earth::ToLongitudeRadians(util::units::Meters(meters), latitude_radians);
}

double s2c_earth_to_radians_meters(double meters) {
    return S2Earth::ToRadians(util::units::Meters(meters));
}

// S2RegionCoverer stub implementation
S2CRegionCoverer* s2c_regioncoverer_new(void) {
    auto* coverer = new S2CRegionCoverer;
    coverer->coverer = S2RegionCoverer();
    return coverer;
}

void s2c_regioncoverer_destroy(S2CRegionCoverer* coverer) {
    delete coverer;
}

void s2c_regioncoverer_set_max_cells(S2CRegionCoverer* coverer, int max_cells) {
    if (coverer) {
        coverer->coverer.mutable_options()->set_max_cells(max_cells);
    }
}

void s2c_regioncoverer_set_min_level(S2CRegionCoverer* coverer, int min_level) {
    if (coverer) {
        coverer->coverer.mutable_options()->set_min_level(min_level);
    }
}

void s2c_regioncoverer_set_max_level(S2CRegionCoverer* coverer, int max_level) {
    if (coverer) {
        coverer->coverer.mutable_options()->set_max_level(max_level);
    }
}

// S2RegionCoverer getter functions
int s2c_regioncoverer_max_cells(const S2CRegionCoverer* coverer) {
    if (!coverer) return 0;
    return coverer->coverer.options().max_cells();
}

int s2c_regioncoverer_min_level(const S2CRegionCoverer* coverer) {
    if (!coverer) return 0;
    return coverer->coverer.options().min_level();
}

int s2c_regioncoverer_max_level(const S2CRegionCoverer* coverer) {
    if (!coverer) return 0;
    return coverer->coverer.options().max_level();
}

int s2c_regioncoverer_level_mod(const S2CRegionCoverer* coverer) {
    if (!coverer) return 1;
    return coverer->coverer.options().level_mod();
}

int s2c_regioncoverer_true_max_level(const S2CRegionCoverer* coverer) {
    if (!coverer) return 0;
    // true_max_level is max_level adjusted for level_mod
    int max_level = coverer->coverer.options().max_level();
    int level_mod = coverer->coverer.options().level_mod();
    if (level_mod > 1 && max_level >= 0) {
        return max_level - (max_level % level_mod);
    }
    return max_level;
}

// S2RegionCoverer setter functions
void s2c_regioncoverer_set_fixed_level(S2CRegionCoverer* coverer, int fixed_level) {
    if (coverer) {
        coverer->coverer.mutable_options()->set_fixed_level(fixed_level);
    }
}

void s2c_regioncoverer_set_level_mod(S2CRegionCoverer* coverer, int level_mod) {
    if (coverer) {
        coverer->coverer.mutable_options()->set_level_mod(level_mod);
    }
}

// S2RegionCoverer covering functions
void s2c_regioncoverer_get_covering_cap(S2CRegionCoverer* coverer, const S2CCap* cap, S2CCellId*** covering, int* count) {
    if (!coverer || !cap || !covering || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetCovering(cap->cap, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *covering = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*covering)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *covering = nullptr;
    }
}

void s2c_regioncoverer_get_covering_rect(S2CRegionCoverer* coverer, const S2CLatLngRect* rect, S2CCellId*** covering, int* count) {
    if (!coverer || !rect || !covering || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetCovering(rect->rect, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *covering = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*covering)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *covering = nullptr;
    }
}

void s2c_regioncoverer_get_covering_cell(S2CRegionCoverer* coverer, const S2CCell* cell, S2CCellId*** covering, int* count) {
    if (!coverer || !cell || !covering || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetCovering(cell->cell, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *covering = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*covering)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *covering = nullptr;
    }
}

void s2c_regioncoverer_get_covering_polygon(S2CRegionCoverer* coverer, const S2CPolygon* polygon, S2CCellId*** covering, int* count) {
    if (!coverer || !polygon || !covering || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetCovering(*polygon->polygon, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *covering = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*covering)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *covering = nullptr;
    }
}

// S2RegionCoverer interior covering functions
void s2c_regioncoverer_get_interior_covering_cap(S2CRegionCoverer* coverer, const S2CCap* cap, S2CCellId*** interior, int* count) {
    if (!coverer || !cap || !interior || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetInteriorCovering(cap->cap, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *interior = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*interior)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *interior = nullptr;
    }
}

void s2c_regioncoverer_get_interior_covering_rect(S2CRegionCoverer* coverer, const S2CLatLngRect* rect, S2CCellId*** interior, int* count) {
    if (!coverer || !rect || !interior || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetInteriorCovering(rect->rect, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *interior = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*interior)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *interior = nullptr;
    }
}

void s2c_regioncoverer_get_interior_covering_cell(S2CRegionCoverer* coverer, const S2CCell* cell, S2CCellId*** interior, int* count) {
    if (!coverer || !cell || !interior || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetInteriorCovering(cell->cell, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *interior = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*interior)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *interior = nullptr;
    }
}

void s2c_regioncoverer_get_interior_covering_polygon(S2CRegionCoverer* coverer, const S2CPolygon* polygon, S2CCellId*** interior, int* count) {
    if (!coverer || !polygon || !interior || !count) {
        if (count) *count = 0;
        return;
    }
    
    std::vector<S2CellId> cells;
    coverer->coverer.GetInteriorCovering(*polygon->polygon, &cells);
    
    *count = cells.size();
    if (*count > 0) {
        *interior = (S2CCellId**)malloc(sizeof(S2CCellId*) * (*count));
        for (int i = 0; i < *count; i++) {
            (*interior)[i] = new S2CCellId{cells[i]};
        }
    } else {
        *interior = nullptr;
    }
}