#include <iostream>
#include <cmath>
#include <vector>
#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image &picture, unsigned max) {
    main_image = new Image(picture);
    max_stickers = max;
}

StickerSheet::~StickerSheet() {
    delete main_image;
}

StickerSheet::StickerSheet(const StickerSheet &other) {
    main_image = new Image(*(other.main_image));
    max_stickers = other.max_stickers;
    std::vector<Image*> copy_stickers;
    for (unsigned i = 0; i < other.stickers.size(); i++) {
            copy_stickers.push_back(other.stickers[i]);
    }
    x_coordinates = other.x_coordinates;
    y_coordinates = other.y_coordinates;
    stickers = copy_stickers;

}

const StickerSheet & StickerSheet::operator=(const StickerSheet &other) {
    if (this == &other) {
        return *this;
    }
    this->~StickerSheet();
    main_image = new Image(*(other.main_image));
    max_stickers = other.max_stickers;
    std::vector<Image*> copy_stickers;
    for (unsigned i = 0; i < other.stickers.size(); i++) {
            copy_stickers.push_back(other.stickers[i]);
    }
    x_coordinates = other.x_coordinates;
    y_coordinates = other.y_coordinates;
    stickers = copy_stickers;

    return *this;
}

void StickerSheet::changeMaxStickers(unsigned max){ //edit
    if (max < stickers.size()){
        stickers.erase(stickers.begin() + max, stickers.end());
    }
    max_stickers = max;
    
       }

int StickerSheet::addSticker(Image &sticker, int x, int y) {
    stickers.push_back(&sticker);
    x_coordinates.push_back(x);
    y_coordinates.push_back(y);
    if (stickers.size() > max_stickers) {
        max_stickers++;
    }
    return stickers.size() - 1;
}

int StickerSheet::setStickerAtLayer(Image &sticker, unsigned layer, int x, int y) {
    if (layer >= max_stickers) {
        return -1;
    }
    /*if (stickers[layer] != nullptr) {
        delete stickers[layer];
    }*/
    stickers[layer] = &sticker;
    x_coordinates[layer] = x;
    y_coordinates[layer] = y;
    return layer;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if (index >= stickers.size()) {
        return false;
    }
    x_coordinates[index] = x;
    y_coordinates[index] = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index) { // edit
    if (stickers[index] != nullptr && index <= max_stickers) {
        stickers.erase(stickers.begin() + index);
        x_coordinates.erase(x_coordinates.begin() + index);
        y_coordinates.erase(y_coordinates.begin() + index);
        //stickers[index] = nullptr;
    }
}

Image* StickerSheet::getSticker(unsigned index) {
    if (index >= stickers.size()) {
        return NULL;
    }
    return stickers[index];
}

int StickerSheet::layers () const{
        return max_stickers;
       }

Image StickerSheet::render() const {
    
    Image final_img;
    unsigned new_width = main_image->width();
    unsigned new_height = main_image->height();
    int x_coordinates_shift = 0, y_coordinates_shift = 0;
    unsigned i = 0;
    while (i < stickers.size()) {
            if (x_coordinates[i] < 0) {
                x_coordinates_shift = std::max(x_coordinates_shift, -x_coordinates[i]);
            } else {
                new_width = std::max(new_width, x_coordinates[i] + stickers[i]->width());
            }
            if (x_coordinates[i] < 0) {
                y_coordinates_shift = std::max(y_coordinates_shift, -y_coordinates[i]);
            } else {
                new_height = std::max(new_height, y_coordinates[i] + stickers[i]->height());
            }
        i++;
    }

    new_width = new_width + x_coordinates_shift;
    new_height = new_height + y_coordinates_shift;
    final_img.resize(new_width, new_height);

    for (unsigned x = 0; x < main_image->width(); x++) {
        for (unsigned y = 0; y < main_image->height(); y++) {
            final_img.getPixel(x + x_coordinates_shift, y + y_coordinates_shift) = main_image->getPixel(x, y);
        }
    }

    for (unsigned i = 0; i < stickers.size(); i++) {
            for (unsigned x = 0; x < stickers[i]->width(); x++) {
                for (unsigned y = 0; y < stickers[i]->height(); y++) {
                    HSLAPixel &sticker_pixel = stickers[i]->getPixel(x, y);
                    if (sticker_pixel.a != 0) {
                        int final_x = x + x_coordinates[i] + x_coordinates_shift;
                        int final_y = y + y_coordinates[i] + y_coordinates_shift;
                        final_img.getPixel(final_x, final_y) = sticker_pixel;
                    }
                }
            }
    }

    return final_img;
}
