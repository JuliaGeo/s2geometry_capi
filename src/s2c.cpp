#include "s2c.h"
#include <cstring>
#include <vector>
#include <memory>

// S2 includes
#include "s2/s1angle.h"
#include "s2/s1chord_angle.h"
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