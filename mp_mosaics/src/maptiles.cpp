/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */

    MosaicCanvas* createmosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> storetiles;
    int tilessize = theTiles.size();
    int a = 0;
    while (a < tilessize){
        storetiles.push_back(convertToXYZ(theTiles[a].getAverageColor()));
        a++;
    }
    KDTree<3> searcht(storetiles);
    int rows = createmosaic -> getRows();
    int cols = createmosaic ->getColumns();
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            Point<3> nearestpixel;
            nearestpixel = searcht.findNearestNeighbor(convertToXYZ(theSource.getRegionColor(i, j)));
            unsigned long k = 0;
            bool changed = false;
            unsigned long sizetiles = storetiles.size();
            while (k < sizetiles && (changed == false)){
                // if (changed == true){
                //     return;
                // }
                if (nearestpixel == storetiles[k]){
                    TileImage* currtile = &theTiles[k];
                    createmosaic->setTile(i, j, currtile);
                    changed = true;
                }
                changed = false;
                k++;
            }




        }
    }

    return createmosaic;

}

