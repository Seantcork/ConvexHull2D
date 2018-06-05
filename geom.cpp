#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <math.h>
#include <algorithm>
#include <iostream>

#include <vector>

using namespace std;

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
double pi = 3.14159265358979323846264338327950288419716939937510;

int signed_area2D(point2D a, point2D b, point2D c) {
  int result =  0;
  result = ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x));
  return result;
}



/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
  int result = 0;
  result = p.x * (q.y - r.y) + q.x * (r.y - p.y) + r.x * (p.y - q.y);
  if (result != 0){
    return 0;
  }
  else {
    return 1;
  }
}



/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2D a, point2D b, point2D c) {
  int result = 0;
  result = signed_area2D(a, b, c);
  if(result > 0){
    return 1;
  }
  else{
    return 0;
  }
}


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2D a, point2D b, point2D c) {
  int left = 0;
  left = left_strictly(a, b, c);
  int on = 0;
  on = collinear(a, b, c);

  if(left == 1 || on == 1){
    return 1;
  }
  else{
    return 0;
  }
}

bool angleCompare(point2D a, point2D b){
  if (a.angle == b.angle){
    return a.distance < b.distance;
  }
  else{
    return a.angle < b.angle;
  }
}


int distance(point2D a, point2D b){
  double result = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
  return result;
}

/* compute the convex hull of the points in p; the points on the CH
   are returned as a vector
*/
vector<point2D> graham_scan(vector<point2D> p) {

  cout << "computing hull.." << endl;
  stack <point2D> ch;
  vector<point2D> result;
  point2D p0 = p[0];

  //finds isolated points with lowest y value. If there are two points with
  //y value then it picks the poitns witht the smallest x coordinate or leftmoset
  //x coordinate
  for(std::vector<point2D>::iterator it = p.begin() +1; it != p.end(); it++){
    if(it -> y < p0.y){
      p0.x = it -> x;
      p0.y = it -> y;
    }
    if(it -> y <= p0.y && it -> x < p0.y){
      p0.x = it -> x;
      p0.y = it -> y;
    }
  }

  //precalculate the  angles according to p0.
  for(std::vector<point2D>::iterator it = p.begin(); it != p.end(); it++){
    double angle = atan2(it -> y - p0.y , it -> x - p0.x);
    angle = angle * (180 / pi);
    it -> angle = angle;
    it -> distance = distance(p0, *it);
  }

  //sorts the angles readially with a vector comparator/
  //the vecotr comparator return the angle that smsallest first
  //if the angles are the same give the shortest one first.
  std::sort(p.begin(), p.end(), angleCompare);

  //push p0, s1 and s2 onto the stack
  ch.push(*p.begin());
  ch.push(*(p.begin() + 1));
  ch.push(*(p.begin() + 2));

  //for all the remaining poitns in the radially sorted vector
  for(std::vector<point2D>::iterator it = p.begin() + 3; it != p.end(); it++){

    point2D p2 = ch.top();
    //get rid of dupicate points
    if(p2.x == it->x && p2.y == it->y) {
      continue;
    }
    ch.pop();
    point2D p1 = ch.top();

    //if next point is left of angle
    if(left_strictly(p1, p2, *it)){
      ch.push(p2);
      ch.push(*it);
    }
    //if its collinear
    else if(collinear(p1, p2, *it)){
      ch.push(p2);
      ch.push(*it);
    }
    else{
      //while its not left pop
      while(left_strictly(p1, p2, *it) != 1){
        p2 = ch.top();
        ch.pop();
        p1 = ch.top();
      }

      //push back on
      ch.push(p2);
      ch.push(*it);
    }
  }
  //push p0 back on stack.
  ch.push(*p.begin());


  point2D top;
  //pop all points on stack off and add them to the vecotr result
  for(int i = 0; !(ch.empty()); i++ ){
    top = ch.top();
    result.push_back(top);
    ch.pop();
  }

  cout << "..done\n" << endl;
  return result;
}
