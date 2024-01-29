#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    work_list.pop_front();
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    work_list.pop_back();
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.front();
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    return work_list.back();
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    png_ = png;
    start_ = start;
    tolerance_ = tolerance;
    fns_ = fns;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    //return ImageTraversal::Iterator();
    //return ImageTraversal::Iterator(png_, start_, tolerance_, this);
    return ImageTraversal::Iterator(png_, start_, tolerance_, this);

  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();

  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    //tolerance_ = 0.0;
    // png_ = PNG();
    // traversal_ = nullptr;
    // current_ = Point(-1,-1);
  }

 ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, ImageTraversal *traversal) {
    /** @todo [Part 1] */

    traversal_ = traversal;
    current_ = start;

    unsigned width_ = 0;
    unsigned height_ = 0;

    visited_.resize(png.width());
    while(width_ < png.width()){
      visited_[width_].resize(png.height());
      while(height_ < png.height()){
        visited_[width_][height_] = false;
        height_++;
      }
      width_++;
    }

    work_list_.push_back(start);

  }


  /**
  * Iterator increment operator.
  *
  * Advances the traversal of the image.
  */

 
 ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
    if (work_list_.empty()){
      return *this;
    }
    std::vector<Point> neighbors;
    visited_[current_.x][current_.y] = true;
    traversal_->fns_.pop(work_list_);
    if (cantraverse(Point(current_.x + 1, current_.y))) traversal_->fns_.add(work_list_, (Point(current_.x + 1, current_.y)) );
    if (cantraverse(Point(current_.x, current_.y + 1))) traversal_->fns_.add(work_list_, (Point(current_.x, current_.y + 1)) );
    if (cantraverse(Point(current_.x - 1, current_.y))) traversal_->fns_.add(work_list_, (Point(current_.x - 1, current_.y)) );
    if (cantraverse(Point(current_.x, current_.y - 1))) traversal_->fns_.add(work_list_, (Point(current_.x, current_.y - 1)) );
    if (empty()) {
      return *this;
    }
    current_ = traversal_->fns_.peek(work_list_);
    while(!empty() && visited_[current_.x][current_.y] == true) {
      traversal_->fns_.pop(work_list_);
      if (empty()) {
        return *this;
      }
      current_ = traversal_->fns_.peek(work_list_);
    }
    return *this;

    
    

  }

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return current_;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if (empty()) {
      return false;
    }
    return true;
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    return work_list_.empty();
  }

}