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
#include "s2/s2boolean_operation.h"
#include "s2/s2buffer_operation.h"
#include "s2/s2earth.h"
#include "s2/s2edge_crossings.h"
#include "s2/s2predicates.h"
#include "s2/mutable_s2shape_index.h"

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
struct S2CBufferOperation { std::unique_ptr<S2BufferOperation> op; };
struct S2CMutableShapeIndex { MutableS2ShapeIndex index; };
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
    // TODO: Implement this when we have proper S2ShapeIndex wrappers
    // The Build method requires two S2ShapeIndex parameters
    if (error) {
        error->ok = false;
        error->text = copy_string("Boolean operation build not yet implemented");
    }
    return false;
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
S2CCellUnion* s2c_cellunion_new(S2CCellId** cellids, int num_cells) {
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

void s2c_cellunion_destroy(S2CCellUnion* cell_union) {
    delete cell_union;
}

int s2c_cellunion_num_cells(const S2CCellUnion* cell_union) {
    return cell_union ? cell_union->cell_union.num_cells() : 0;
}

bool s2c_cellunion_contains_cellid(const S2CCellUnion* cell_union, const S2CCellId* cellid) {
    return cell_union && cellid ? cell_union->cell_union.Contains(cellid->cellid) : false;
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

double s2c_earth_get_distance_km_latlng(const S2CLatLng* a, const S2CLatLng* b) {
    if (!a || !b) return 0.0;
    return S2Earth::GetDistanceKm(a->latlng, b->latlng);
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