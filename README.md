# S2C - C API for S2 Geometry

This is a C API wrapper for Google's S2 geometry library, providing a C-compatible interface to S2's spherical geometry functionality.

## Features

The C API provides access to core S2 functionality including:

- **Basic Types**: S2Point, S2LatLng, S2CellId, S1Angle
- **Shapes**: S2Cap, S2Cell, S2Loop, S2Polyline, S2Polygon
- **Regions**: S2LatLngRect, S2CellUnion
- **Operations**: Basic containment and intersection tests
- **Utilities**: S2RegionCoverer, S2Earth calculations

## Building

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF
make -j8
```

## Example Usage

```c
#include "s2c.h"

// Create a point from latitude/longitude
S2CLatLng* latlng = s2c_latlng_from_degrees(40.7, -74.0);
S2CPoint* point = s2c_latlng_to_point(latlng);

// Create a polygon
S2CPoint* vertices[] = {v1, v2, v3};
S2CLoop* loop = s2c_loop_new_from_points(vertices, 3);
S2CPolygon* polygon = s2c_polygon_new_from_loop(loop);

// Test containment
bool contains = s2c_polygon_contains(polygon, point);

// Cleanup
s2c_polygon_destroy(polygon);
s2c_point_destroy(point);
s2c_latlng_destroy(latlng);
```

## API Design

The API follows these conventions:

- All types are opaque pointers with `S2C` or `S1C` prefix
- Functions follow the pattern `s2c_type_operation()`
- Memory management uses explicit `new`/`destroy` functions
- Error handling through `S2CError` structure where applicable
- Boolean operations return `bool` directly

## Implementation Status

Currently implemented:
- ✓ Basic geometric types (Point, LatLng, Angle)
- ✓ Shape types (Loop, Polygon, Cap, Cell)
- ✓ Basic operations (containment, intersection tests)
- ✓ Region types (LatLngRect, CellUnion)
- ✓ Memory management functions

Not yet implemented:
- Boolean operations (union, intersection, difference)
- Advanced shape index operations
- Polyline operations
- Builder patterns for complex operations
- Full error propagation

## Examples

See the `examples/` directory for:
- `polygon_operations.c` - Creating and testing polygon operations
- `simple_polygon_test.c` - Basic polygon containment and intersection tests

## License

This wrapper follows the same Apache 2.0 license as the S2 geometry library.