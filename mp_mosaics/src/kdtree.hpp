/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    auto anum = first[curDim];
    auto bnum = second[curDim];
    if (anum < bnum){
      return true;
    }
    else if (anum > bnum){
      return false;
    }
    // else if (first[curDim] == second[curDim]){
    return first < second;
    //}
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double currentbestx = 0;
    double potentialx = 0;

    for (int i = 0; i < Dim; i++){
      currentbestx += ((target[i]-currentBest[i])*(target[i]-currentBest[i]));
      potentialx += ((target[i]-potential[i])*(target[i]-potential[i]));
    }

    if (potentialx < currentbestx){
      return true;
    }
    else{
      return false;
    }
    //  T currentbestx = ((target[0]-currentbest[0])*(target[0]-currentbest[0]))+((target[1]-currentbest[1])*(target[1]-currentbest[1]))+((target[2]-currentbest[2])*(target[2]-currentbest[2]));
    //  T potentialx = ((target[0]-potential[0])*(target[0]-potential[0]))+((target[1]-potential[1])*(target[1]-potential[1]))+((target[2]-potential[2])*(target[2]-potential[2]));
     
    //  if(potentialx < currentbestx){
    //   return true;
    //  }
     
    //  return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    if(newPoints.empty() == true){
      return;
    }
    vector<Point<Dim>> newpointstemp = newPoints;
    // auto cmp = [startingdimen](const Point<Dim>& first, const Point<Dim>& second) {
    //     return smallerDimVal(first, second, startingdimen);
    // };
    root = kdtreebuild(newpointstemp.begin(), newpointstemp.end(), 0);
    //root = kdtreebuild(newPoints.begin(), newPoints.end(), 0);
    /**
     * @todo Implement this function!
     */
}

template <int Dim>
template <typename RandIter>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::kdtreebuild(RandIter start, RandIter end, int dimen){
    if (start == end || start > end){
      return nullptr;
    }
    //int k = Dim;
    RandIter med = start + (((distance(start,end)) - 1) / 2);
    int dimentemp = ((dimen) % Dim);
    auto cmp = [dimentemp](const Point<Dim>& first, const Point<Dim>& second) {
      return smallerDimVal(first, second, dimentemp);
    };
    select(start, end, med, cmp);
    KDTreeNode* newtnode = new KDTreeNode(*med);
    if (med == start){
      newtnode -> left = nullptr;
    } else{
        newtnode -> left = kdtreebuild(start, med, dimentemp + 1);
    }
    if (med == end){
      newtnode -> right = nullptr;
    }else{
      newtnode -> right = kdtreebuild(next(med), end, dimentemp + 1);
    }
    return newtnode;

}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copytreebuild(const KDTreeNode* othertnode, int dimen) {
  //int k = Dim;
  if (othertnode != nullptr){
    KDTreeNode* copytnode = new KDTreeNode();
    copytnode -> point = othertnode -> point;
    copytnode -> left = copytreebuild(othertnode -> left, ((dimen + 1) % Dim));
    copytnode -> right = copytreebuild(othertnode -> right, ((dimen + 1) % Dim));

    return copytnode;

  }
  return nullptr;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
   root = copytreebuild(other.root, 0);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (this == &rhs){
    return *this;
    }
  cleartree(root);
  root = copytreebuild(rhs.root, 0);
  return *this;
}


template <int Dim>
void KDTree<Dim>::cleartree(KDTreeNode* tnode) {
  if (tnode != nullptr){
    cleartree(tnode -> left);
    cleartree(tnode -> right);
    delete tnode;
  }
  return;
}


template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
   cleartree(root);
}


template <int Dim>
double pointdistance(const Point<Dim>& point1, const Point<Dim>& point2){
  
    double distancepoint = 0;

    for (int i = 0; i < Dim; i++){
      distancepoint += ((point1[i]-point2[i])*(point1[i]-point2[i]));
    }
    return distancepoint;

}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    KDTreeNode* nearestnode = nearesthelper(query, root, 0);
    Point<Dim> nearestpoint = nearestnode -> point;
    return nearestpoint;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::nearesthelper(const Point<Dim>& point, KDTreeNode* rootnode, int dimen) const{
  if (rootnode == nullptr){
    return root;
  }
  Point<Dim> rootpoint = rootnode -> point;
  auto cmp = [dimen](const Point<Dim>& first, const Point<Dim>& second) {
      return smallerDimVal(first, second, dimen);
    };
  KDTreeNode* gonext;
  KDTreeNode* tempnext;
  if (cmp(point, rootpoint) == true){
      gonext = rootnode -> left;
      tempnext = rootnode -> right; 
  }
  else{
      gonext = rootnode -> right;
      tempnext = rootnode -> left;
  }

  KDTreeNode* potential = nearesthelper(point, gonext, (dimen + 1) % Dim);
  KDTreeNode* currentbest;
  if (potential != nullptr){
    if (pointdistance(point, potential -> point) < pointdistance(point, rootnode -> point)){
      currentbest = potential;
        }
    else{
      currentbest = rootnode;
        }
  }
  else{
    currentbest = rootnode;
  }

  double best_radius = pointdistance(point, currentbest -> point);
  double dist_parti = point[dimen] - rootpoint[dimen];
  double dist_parti_sq = dist_parti * dist_parti;

  if (!(best_radius < dist_parti_sq)){
      KDTreeNode* anotherpotential = nearesthelper(point, tempnext, (dimen + 1) % Dim);
      if (anotherpotential != nullptr) {
        double pointtoanother = pointdistance(point, anotherpotential -> point);
        double pointtocurrent = pointdistance(point, currentbest -> point);
        if (pointtoanother < pointtocurrent){
            currentbest = anotherpotential;
            }
            else if (pointtoanother == pointtocurrent){
              if (cmp(anotherpotential -> point, currentbest -> point) == true){
                currentbest = anotherpotential;
              }
            }
      // else{
      //   currentbest = rootnode;
  //}
      }

  }

  return currentbest;
}


template <typename RandIter, typename Comparator>
RandIter parti(RandIter start, RandIter end, RandIter pivotpoint, Comparator cmp){
  auto lastelem = prev(end);
  iter_swap(pivotpoint, lastelem);
  auto startswap = start;
  auto starttraverse = start;
  auto endtemp = end;

  while(starttraverse != lastelem){
    if(cmp(*starttraverse, *lastelem) == true){
      iter_swap(startswap, starttraverse);
      startswap++;
    }
    starttraverse++;
  }

  iter_swap(startswap, lastelem);
  return startswap;
}


template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */
    if (start == end || end == k || start > end){ // if empty or out of bound, quit
      return;
     }
    
    RandIter pivotpoint = start + ((distance(start,end)) / 2);
    pivotpoint = parti(start, end, pivotpoint, cmp);

    if (k == pivotpoint){
      return;
    }

    else if (cmp(*k,*pivotpoint)){
      select(start, pivotpoint, k, cmp);
    }

    else{
      select(pivotpoint + 1, end, k, cmp);
    }
}

