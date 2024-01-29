/* Your code here! */
#include <iostream>
#include <random>
#include "maze.h"
#include "dsets.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <queue>
#include <map>
using namespace std;

SquareMaze::SquareMaze() {
    width_ = 0;
    height_ = 0;
}

/**
     * Makes a new SquareMaze of the given height and width.
     *
     * If this object already represents a maze it will clear all the
     * existing data before doing so. You will start with a square grid (like
     * graph paper) with the specified height and width. You will select
     * random walls to delete without creating a cycle, until there are no
     * more walls that could be deleted without creating a cycle. Do not
     * delete walls on the perimeter of the grid.
     *
     * Hints: You only need to store 2 bits per square: the "down" and
     * "right" walls. The finished maze is always a tree of corridors.)
     *
     * @param width The width of the SquareMaze (number of cells)
     * @param height The height of the SquareMaze (number of cells)
     */


void SquareMaze::makeMaze(int width, int height) {
    DisjointSets dset; // create disjoint set
    width_ = width;
    height_ = height;
    int dimen = width_ * height_; // get dimension of maze
    //srand(time(nullptr));
    dset.addelements(dimen); // create disjoint set with dimension
    auto now = chrono::high_resolution_clock::now(); // use chrono for randomization
    auto duration = now.time_since_epoch();
    auto micros = chrono::duration_cast<chrono::microseconds>(duration).count(); // initially used srand, but we need randomization in microsecond
    
    mt19937 engine(static_cast<unsigned int>(micros));
    uniform_int_distribution<int> randindex(0, dimen - 1); // random numbers between 0 to dimension
    uniform_int_distribution<int> randwall(0, 1); // random numbers between 0 and 1
    // right wall = left element
    // down wall = right element
    // if there is a wall -> 1 , 0 otherwise
    wall.clear(); // clear the vector/wall first
    // int i = 0;
    // while (i < dimen){
    //     wall.push_back({1,1});
    //     i++;
    // }
    wall.assign(dimen, {1,1}); // create a maze filled with grid walls
    // std::vector<int> randvec;
    // for (int i = 0; i < dimen; i++){
    //     randvec.push_back(i);
    // }
    // std::random_shuffle(randvec.begin(), randvec.end());
    int j = 0;
    while (dset.size(0) != dimen){ // check size of tree = dimen, size on anynode 
        //int index = rand() % dimen; // index = rand() % dimen
        //std::cout << index << std::endl;
        //int rwdw = rand() % 2;
        int index = randindex(engine); // get random index
        int rwdw = randwall(engine); // random whether right or bottom wall
        if (rwdw == 0 && ((index + 1) % width != 0)){ // if randomized to right wall and check that the right cell is in bound
            int nextcell = index + 1; // get the index of right cell
            if (!dset.sameset(index, nextcell)){ // if they are not in the same set
                wall[index].first = 0; // remove the right wall, the first element in wall vector
                dset.setunion(index, nextcell); // use setunion to put them in same disjoint set
            }
            else if (dimen > (index + width)){ // if cell bottom of the current cell exists (not out of bound)
                nextcell = index + width; // nextcell is the bottom cell of current one
                if (!dset.sameset(index, nextcell)){ // if they are not in the same disjoint set
                    wall[index].second = 0; // remove wall
                    dset.setunion(index, nextcell); // put them in same disjoint set
                }
            }
        }
        else if (rwdw == 1 && ((index + width) < dimen)){ // if randomized to bottom wall and check that the cell below is in bound
            int nextcell = index + width; // nextcell is the bottom cell of the current one
            if (!dset.sameset(index, nextcell)){
                wall[index].second = 0;
                dset.setunion(index, nextcell);
            }
            else if ((index + 1) % width != 0){
                nextcell = index + 1;
                if (!dset.sameset(index, nextcell)){
                    wall[index].first = 0;
                    dset.setunion(index, nextcell);
                }
            }

        }
        j++;
        }
    // std::random_shuffle(wall.begin(), wall.end());
    // int j = 0;
    // while (j < dimen - 1){
    //     int wallrand1 = rand() % 2;
    //     int wallrand2 = rand() % 2;
    //     int randa = 
    //     if (!(sameset(randa,randb))){
    //         setunion()
    //         wall[j] = {wallrand1, wallrand2}
    //     }
    //     j++;
    // }
    
    }
    


  /**
     * This uses your representation of the maze to determine whether it is
     * possible to travel in the given direction from the square at
     * coordinates (x,y).
     *
     * For example, after makeMaze(2,2), the possible input coordinates will
     * be (0,0), (0,1), (1,0), and (1,1).
     *
     * - dir = 0 represents a rightward step (+1 to the x coordinate)
     * - dir = 1 represents a downward step (+1 to the y coordinate)
     * - dir = 2 represents a leftward step (-1 to the x coordinate)
     * - dir = 3 represents an upward step (-1 to the y coordinate)
     *
     * You can not step off of the maze or through a wall.
     *
     * This function will be very helpful in solving the maze. It will also
     * be used by the grading program to verify that your maze is a tree that
     * occupies the whole grid, and to verify your maze solution. So make
     * sure that this function works!
     *
     * @param x The x coordinate of the current cell
     * @param y The y coordinate of the current cell
     * @param dir The desired direction to move from the current cell
     * @return whether you can travel in the specified direction
     */

bool SquareMaze::canTravel(int x, int y, int dir) const {
    int index = y * width_ + x;
    if (x >= width_ || y >= height_ || x < 0 || y < 0){ // check that it is out of bounds
        return false; // if it is,  return false
    }
    if (dir == 0){ // if go right
        if (wall[index].first == 1){ // if there is a right wall
            return false;  // cant go
        }
        else{ // otherwise can
            return true; 
        }
    }
    else if (dir == 1){
        if (wall[index].second == 1){
            return false;
        }
        else{
            return true;
        }
    }
    else if (dir == 2){
        if (wall[index - 1].first == 1){ // if left cell has right wall
            return false; // cant go
        }
        else{
            return true;
        }
    }
    else if (dir == 3){
        if (wall[index - width_].second == 1){ // if top cell has bottom wall
            return false; // cant  go
        }
        else{
            return true;
        }
    }
    else{
        return false;
    }
}

/**
     * Sets whether or not the specified wall exists.
     *
     * This function should be fast (constant time). You can assume that in
     * grading we will not make your maze a non-tree and then call one of the
     * other member functions. setWall should not prevent cycles from
     * occurring, but should simply set a wall to be present or not present.
     * Our tests will call setWall to copy a specific maze into your
     * implementation.
     *
     * @param x The x coordinate of the current cell
     * @param y The y coordinate of the current cell
     * @param dir Either 0 (right) or 1 (down), which specifies which wall to
     * set (same as the encoding explained in canTravel). You only need to
     * support setting the bottom and right walls of every square in the grid.
     * @param exists true if setting the wall to exist, false otherwise
     */

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    int index = y * width_ + x; // get index in 1d from 2d
    if (dir == 0){ // if right
        wall[index].first = exists; // make right wall
    }
    else if (dir == 1){ // if down
        wall[index].second = exists; // make bottom wall
    }
}

  /**
     * Solves this SquareMaze.
     *
     * For each square on the bottom row (maximum y coordinate), there is a
     * distance from the origin (i.e. the top-left cell), which is defined as
     * the length (measured as a number of steps) of the only path through
     * the maze from the origin to that square.
     *
     * Select the square in the bottom row with the largest distance from the
     * origin as the destination of the maze. solveMaze() returns the
     * winning path from the origin to the destination as a vector of
     * integers, where each integer represents the direction of a step, using
     * the same encoding as in canTravel().
     *
     * If multiple paths of maximum length exist, use the one with the
     * destination cell that has the smallest x value.
     *
     * Hint: this function should run in time linear in the number of cells
     * in the maze.
     *
     * @return a vector of directions taken to solve the maze
     */

std::pair<int, int> SquareMaze::gonext(int x, int y, int dir) {
    switch (dir) {
        case 0: return {x + 1, y};     // go right
        case 1: return {x, y + 1};     // go downward
        case 2: return {x - 1, y};     // go left
        case 3: return {x, y - 1};     // go upward
        default: return {x, y};        // else return the same x y
    }
}

int SquareMaze::adddirection(int dir) {
    switch (dir) {
        case 0: return 1; // go right, add 1
        case 1: return width_; // go down, add width
        case 2: return -1; // go left, minus 1
        case 3: return -1 * width_; // go up, minus width
        default: return 0;  // else add nothing
    }
}

bool SquareMaze::valid(int x, int y, int dir, const std::vector<bool>& visited) {
    std::pair<int,int> xy = gonext(x, y, dir); // get coordinates from gonext function
    int w = (y * width_ + x) + adddirection(dir); // the next direction added
    return ((xy.first) >= 0 && (xy.first) < width_ && (xy.second) >= 0 && (xy.second) < height_ && canTravel(x, y, dir) && !visited[w]); // check the bounds, cantravel, and whether already visited
}

vector<int> SquareMaze::solveMaze() {
    queue<int> queue; // declare queue
    std::vector<int> direction; // store which direction to go
    vector<int> distance(width_ * height_, 0); // distance to find the longest path
    vector<bool> visited (width_ * height_, 0); // check whether the cell is visited
    vector<int> prev (width_ * height_, -1); // keep track of prev cell for backtracking

    visited[0] = 1; // get the starting cell in queue
    queue.push(0);

    while(!queue.empty()){ 
        int v  = queue.front(); 
        queue.pop();
        int x = v % width_; // get x and y
        int y = v / width_;

        // if (v >= ((height_ - 1) * width_)&& v <= ((height_ - 1) * width_ + (width_ - 1))){
        //     return;
        // }

        // for (int w = 0; w < 4; w++){
        //     if (visited[w] == 0){
        //         visited[w] = 1;
        //         queue.push(w);
        //     }
        // }

        for (int i = 0; i < 4; i++){ // for 4 directions
            if (valid(x,y,i, visited) == 1){ // if it is valid
                int w = v + adddirection(i); // determine next one
                visited[w] = 1; // mark as visited
                queue.push(w); // push in queue
                distance[w] = distance[v] + 1; // go to the next one
                prev[w] = v; // change prev to current one
            }
            
        }

        
    }

    int lastrow = (height_ - 1) * width_; // the start index of cell in last row
    for (int i = lastrow; i < width_ * height_; i++){ // traverse through cells in the last row
        if (i == lastrow){ // skip the first one bc already checked
            continue;
        }
        if (distance[i] > distance[lastrow]){ // if distance is greater, replace
            lastrow = i;
        }
    }

    std::map<int, int> nextdirectiongo = {{1, 0}, {-1, 2}, {width_, 1}, {-width_, 3}}; // store the directions and difference
    for (int v = lastrow; v != 0; v = prev[v]) { // until reaches the first cell
        int difference = v - prev[v]; // determine next direction
        if (nextdirectiongo.find(difference) != nextdirectiongo.end()) {
            direction.push_back(nextdirectiongo[difference]); // store next direction
        }
    }

    reverse(direction.begin(), direction.end()); // since we backtrack, we reverse
    return direction; // return result

    
}

 /**
     * Draws the maze without the solution.
     *
     * First, create a new PNG. Set the dimensions of the PNG to
     * (width*10+1,height*10+1). where height and width were the arguments to
     * makeMaze. Blacken the entire topmost row and leftmost column of
     * pixels, except the entrance (1,0) through (9,0).  For each square in
     * the maze, call its maze coordinates (x,y). If the right wall exists,
     * then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from
     * 0 to 10. If the bottom wall exists, then blacken the pixels with
     * coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
     *
     * The resulting PNG will look like the sample image, except there will
     * be no exit from the maze and the red line will be missing.
     *
     * @return a PNG of the unsolved SquareMaze
     */

cs225::PNG* SquareMaze::drawMaze() const {
    cs225::PNG* drawnmaze = new cs225::PNG(width_ * 10 + 1, height_ * 10 + 1); // create new image based on dimension
    
    for (int i = 0; i < width_ * 10 + 1; i++) { // loop through pixels in first row
        if (i >= 1 && i <= 9){ // avoid pixels 1-9
            continue;
        }
        drawnmaze->getPixel(i, 0) = cs225::HSLAPixel(0, 0, 0, 1); // set to black
    }

     for (int i = 0; i < height_ * 10 + 1; i++) { // loop through pixels in first column
        drawnmaze->getPixel(0, i) = cs225::HSLAPixel(0, 0, 0, 1); // set to black
    }

    for (int i = 0; i < width_ * height_; i++){ // loop through the pixels in the maze
        int x = i % width_; // get x from index
        int y = i / width_; // get y from index

        if (wall[i].first == 1){ // if there is right wall
            for (int k = 0; k <= 10; k++){ // loop through and paint black
                drawnmaze->getPixel((x + 1)*10, (y * 10) + k) = cs225::HSLAPixel(0, 0, 0, 1);
            }
        }
        if (wall[i].second == 1){ // if there is bottom wall
            for (int k = 0; k <= 10; k++){
                drawnmaze->getPixel((x * 10) + k, (y + 1) * 10) = cs225::HSLAPixel(0, 0, 0, 1);
            }
        }

    }

    return drawnmaze; // return the image
    //return nullptr;
}

 /**
     * This function calls drawMaze, then solveMaze; it modifies the PNG
     * from drawMaze to show the solution vector and the exit.
     *
     * Start at pixel (5,5). Each direction in the solution vector
     * corresponds to a trail of 11 red pixels in the given direction. If the
     * first step is downward, color pixels (5,5) through (5,15) red. (Red is
     * 0,1,0.5,1 in HSLA). Then if the second step is right, color pixels (5,15)
     * through (15,15) red. Then if the third step is up, color pixels
     * (15,15) through (15,5) red. Continue in this manner until you get to
     * the end of the solution vector, so that your output looks analogous
     * the above picture.
     *
     * Make the exit by undoing the bottom wall of the destination square:
     * call the destination maze coordinates (x,y), and whiten the pixels
     * with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
     *
     * @return a PNG of the solved SquareMaze
     */

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* picture = drawMaze(); // get image from drawmaze
    vector<int> directions = solveMaze(); // create vector to store directions from solvemaze

    int i = 5 * width_ + 5; // y* width_ + x
    int x = i % width_; // get x from index
    int y = i / width_; // get y from index
    unsigned long a = 0; // counter
    while (a < directions.size()){  // continue until reaches the end of vector
        if (directions[a] == 1){ // downward
            for (int j = 0; j <= 10; j++){ // change 11 pixels
                picture -> getPixel(x, y + j) = cs225::HSLAPixel(0,1,0.5,1); // change pixel color to red
            }
            y = y + 10; // change y
        }
            else if (directions[a] == 0){ // right
               for (int j = 0; j <= 10; j++){
                   picture -> getPixel(x + j, y) = cs225::HSLAPixel(0,1,0.5,1);
               }
               x = x + 10;
            }
            else if (directions[a] == 3){ // upward
               for (int j = 0; j <= 10; j++){
                   picture -> getPixel(x, y - j) = cs225::HSLAPixel(0,1,0.5,1);
               }
               y = y - 10;
            }
            else if (directions[a] == 2){ // left
               for (int j = 0; j <= 10; j++){
                   picture -> getPixel(x - j, y) = cs225::HSLAPixel(0,1,0.5,1);
               }
               x = x - 10;
            }
    a++; // increment counter
    }

    int x1 = x / 10; // get x pixel
    int y1 = y / 10; // get y pixel
    int k = 1; // counter
    while (k <= 9){ // from 1 to 9
        picture->getPixel(x1 * 10 + k, (y1 + 1) * 10) = cs225::HSLAPixel(1,1,1,1); // create the exit by making the pixel white
        k++; // increment counter
    }


    return picture; // return the picture of the finished maze with solution
}
