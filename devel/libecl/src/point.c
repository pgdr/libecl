#include <stdlib.h>
#include <util.h>
#include <point.h>
#include <stdbool.h>





void point_compare( const point_type *p1 , const point_type * p2, bool * equal) {
  const double tolerance = 0.0001;
  if ((abs(p1->x - p2->x) + abs(p1->y - p2->y) + abs(p1->z - p2->z)) > tolerance)
    *equal = false;
}


void point_set(point_type * p , double x , double y , double z) {
  p->x = x;
  p->y = y;
  p->z = z;
}


void point_copy_values(point_type * p , const point_type * src) {
  point_set(p , src->x , src->y , src->z);
}


point_type * point_alloc_empty( ) {
  point_type * p = util_malloc( sizeof * p , __func__);
  return p;
}

point_type * point_alloc( double x , double y , double z) {
  point_type * p = util_malloc( sizeof * p , __func__);
  point_set( p , x , y , z );
  return p;
}


point_type * point_copyc( const point_type * p) {
  point_type * copy = util_alloc_copy( p , sizeof * p , __func__);
  return copy;
}


point_type * point_alloc_diff( const point_type * p1 , const point_type * p0) {
  point_type * diff = point_copyc( p1 );
  point_inplace_sub( diff , p0 );
  return diff;
}



void point_free( point_type * p) {
  free( p );
}


void point_fprintf( const point_type * p , FILE * stream ) {
  fprintf(stream , "%g %g %g ",p->x , p->y , p->z);
}

/*****************************************************************/
/* Math operations */

void point_inplace_add(point_type * point , const point_type * add) {
  point->x += add->x;
  point->y += add->y;
  point->z += add->z;
}


void point_inplace_sub(point_type * point , const point_type * sub) {
  point->x -= sub->x;
  point->y -= sub->y;
  point->z -= sub->z;
}


void point_inplace_scale(point_type * point , double scale_factor) {
  point->x *= scale_factor;
  point->y *= scale_factor;
  point->z *= scale_factor;
}


/**
   Will compute the vector cross product B x C and store the result in A.
*/

void point_vector_cross(point_type * A , const point_type * B , const point_type * C) {
  A->x =   B->y*C->z - B->z*C->y;
  A->y = -(B->x*C->z - B->z*C->x);
  A->z =   B->x*C->y - B->y*C->x;
}

double point_dot_product( const point_type * v1 , const point_type * v2) {
  return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}



/**
   Computes the normal vector spanned by the two vectors: v1 = (p1 - p0) and v2 = (p2 - p0). The
   result is stored in n.
*/

void point_normal_vector(point_type * n, const point_type * p0, const point_type * p1 , const point_type * p2) {
  point_type * v1 = point_alloc_diff( p1 , p0 );
  point_type * v2 = point_alloc_diff( p2 , p0 );

  point_vector_cross( n , v1 , v2 );

  free( v1 );
  free( v2 );
}



/**
   This function calculates the (signed) distance from point 'p' to
   the plane specifed by the plane vector 'n' andthe point
   'plane_point' which is part of the plane.
*/

double point_plane_distance(const point_type * p , const point_type * n , const point_type * plane_point) {
  point_type * diff = point_alloc_diff( p , plane_point );
  double d = point_dot_product( n , diff );
  free( diff );
  return d;
}


