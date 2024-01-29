#include "Image.h"
#include "cs225/HSLAPixel.h"
#include <iostream>
#include <cmath>
#include "cs225/PNG.h"
using namespace std;
using namespace cs225;

Image::Image() : PNG() {}

Image::Image(unsigned int width, unsigned int height) : PNG(width, height) {}

void Image::lighten() {
    this->lighten(0.1);
}

void Image::lighten(double amount) {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.l = curr_pixel.l + amount;
            if (curr_pixel.l > 1){
                curr_pixel.l = 1;
            }
            else if (curr_pixel.l < 0){
                curr_pixel.l = 0;
            }

        }
    }
}

void Image::darken() {
    this->darken(0.1);
}

void Image::darken(double amount) {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.l = curr_pixel.l - amount;
            if (curr_pixel.l > 1){
                curr_pixel.l = 1;
            }
            else if (curr_pixel.l < 0){
                curr_pixel.l = 0;
            }

        }
    }
}

void Image::saturate() {
    this->saturate(0.1);
}

void Image::saturate(double amount) {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.s = curr_pixel.s + amount;
            if (curr_pixel.s > 100){
                curr_pixel.s = 100;
            }
            else if (curr_pixel.s < 0){
                curr_pixel.s = 0;
            }

        }
    }
}

void Image::desaturate() {
    this->desaturate(0.1);
}

void Image::desaturate(double amount) {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.s = curr_pixel.s - amount;
            if (curr_pixel.s > 100){
                curr_pixel.s = 100;
            }
            else if (curr_pixel.s < 0){
                curr_pixel.s = 0;
            }

        }
    }
}

void Image::grayscale() {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.s = 0;

        }
    }
}

void Image::rotateColor (double degrees) {
    for (unsigned y = 0; y < this->height(); y++) {
        for (unsigned x = 0; x < this->width(); x++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            curr_pixel.h = curr_pixel.h + degrees;
            if (curr_pixel.h > 360){
                curr_pixel.h = curr_pixel.h - 360;
            }
            else if (curr_pixel.h < 0){
                curr_pixel.h = curr_pixel.h + 360;
            }
        }
    }
}

void Image::illinify() {
    double IlliniOrange = 11.0;
    double IlliniBlue = 216.0;

    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & curr_pixel = getPixel(x, y);
            int distance_blue = std::min(360 - IlliniBlue + curr_pixel.h, abs(IlliniBlue - curr_pixel.h));
            int distance_orange = std::min(abs(IlliniOrange - curr_pixel.h), 360 - IlliniOrange + curr_pixel.h);
            if (distance_blue > distance_orange){
                curr_pixel.h = IlliniOrange;
            }
            else{
                curr_pixel.h = IlliniBlue;
            }
}
    }
}

void Image::scale(double factor) {
    unsigned scaled_width = this->width() * factor; // new width factored
    unsigned scaled_height = this->height() * factor; // new height factored

    PNG* scaled_image = new PNG(scaled_width,scaled_height); // create new image, check here
    
    for (unsigned x = 0; x < scaled_width; x++) { // loop through the new image pixels
        for (unsigned y = 0; y < scaled_height; y++) {
            unsigned new_pixel_x = x / factor;  // specify the location to get the pixel from original image
            unsigned new_pixel_y = y / factor;
            if (new_pixel_x < this->width() && new_pixel_y < this->height()) {
            HSLAPixel & og_pixel = getPixel(new_pixel_x, new_pixel_y); // get the pixel from original image
            HSLAPixel & scaled_pixel = (*scaled_image).getPixel(x,y); // get the pixel from new image
            scaled_pixel = og_pixel; // set the new pixels equal to the acquired pixels from old image
            }
        }
    }
    this->resize(scaled_width, scaled_height);
    
    for (unsigned x = 0; x < scaled_width; x++) {
        for (unsigned y = 0; y < scaled_height; y++) {
            HSLAPixel & new_pixel = (*scaled_image).getPixel(x, y);
            HSLAPixel & resized_pixel = this->getPixel(x, y);
            resized_pixel = new_pixel;
        }
    }
    delete scaled_image;
}

void Image::scale(unsigned w, unsigned h) {
    double new_width;
    double new_height;

    if (this->width() * h > this->height() * w) {
        new_width = w;
        new_height = w * this->height() / this->width();
}   else {
        new_height = h;
        new_width = h * this->width() / this->height();
}

    PNG* scaled_final = new PNG(new_width,new_height);

    for (unsigned x = 0; x < new_width; x++) { // loop through the new image pixels
        for (unsigned y = 0; y < new_height; y++) {
            unsigned new_pixel_x = x * this->width() / new_width;  //the location to get the pixel from original image
            unsigned new_pixel_y = y * this->height() / new_height;
            if (new_pixel_x < this->width() && new_pixel_y < this->height()) {
                HSLAPixel & og_pixel = getPixel(new_pixel_x, new_pixel_y); // get the pixel from original image
                HSLAPixel & scaled_pixel = (*scaled_final).getPixel(x,y); // get the pixel from new image
                scaled_pixel = og_pixel; // set the new pixels equal to the acquired pixels from old image
        }
        }
    }
    this->resize(new_width, new_height);
    
    for (unsigned x = 0; x < new_width; x++) {
        for (unsigned y = 0; y < new_height; y++) {
            HSLAPixel & new_pixel = (*scaled_final).getPixel(x, y);
            HSLAPixel & resized_pixel = this->getPixel(x, y);
            resized_pixel = new_pixel;
        }
    }
   delete scaled_final;
}
