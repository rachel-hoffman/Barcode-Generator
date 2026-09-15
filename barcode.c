#include "barcode.h"
#include <stdio.h>


// Helper functions
void whiteBar(struct image* imageOutput, int index){
    int width = imageOutput->width;
    int height = imageOutput->height;
    for(int h = 0; h < height;h++){
        imageOutput->pixels[h * width + index].r = 255;
        imageOutput->pixels[h * width + index].g = 255;
        imageOutput->pixels[h * width + index].b = 255;
    }
}
void blackBar(struct image* imageOutput, int index){
    int width = imageOutput->width;
    int height = imageOutput->height;
    for(int h = 0; h < height;h++){
        imageOutput->pixels[h * width + index].r = 0;
        imageOutput->pixels[h * width + index].g = 0;
        imageOutput->pixels[h * width + index].b = 0;
    }
}

void fillDigit(struct image* imageOutput, int mask, int start){
    for(int i = 0; i < 7; i++){
        int bit = (mask >> (6-i)) & 1;

        if(bit == 1){
            whiteBar(imageOutput, start + i);
        }
        else{
            blackBar(imageOutput, start + i);
        }
    }
}

int getBit(int digit){
    // Return bits mask of each digit
    switch(digit){
        case 0:
            return 0b1110010;
        case 1:
            return 0b1100110;
        case 2:
            return 0b1101100;
        case 3:
            return 0b1010000;
        case 4:
            return 0b1011100;
        case 5:
            return 0b1001110;
        case 6:
            return 0b1000010;
        case 7:
            return 0b1000100;
        case 8:
            return 0b1001000;
        case 9:
            return 0b1110100;
        default:
            printf("Error encountered, invalid digit");
            return -1;
    }
}

void middleBit(struct image* imageOutput, int start){
    // Insert the middle section
    int middleBit = 0b10101;
    for(int i=0; i < 5; i++){
        int bit = (middleBit >> (4 - i)) & 1;
        if (bit == 1){
            whiteBar(imageOutput, start + i);
        }
        else{
            blackBar(imageOutput, start + i);
        }
    }
}

void quietZone(struct image* imageOutput, int start){
    // Quiet zones are 9 white stripes
    for(int i=0; i < 9; i++){
        whiteBar(imageOutput, start + i);
    }
}
void startEndSequence(struct image* imageOutput, int start){
    // Insert start/ed sequence (010)
    int bits = 0b010;

    for(int i=0; i < 3; i++){
        int bit = (bits >> (2 - i)) & 1;
        if (bit == 1){
            whiteBar(imageOutput, start + i);
        }
        else{
            blackBar(imageOutput, start + i);
        }
    }
}

// Main function
struct image* barcode(char* data, int width, int height) {
    // Allocate image structure size
    struct image* imageOutput = malloc(sizeof(struct image));
    
    int x = 0;

    // Allocate pixel/image size
    imageOutput->pixels = malloc(width * height * sizeof(struct pixel));

    // Initialize image fields
    imageOutput->width = width;
    imageOutput->height = height;
    
    // Convert each digits into its barcode form
    
    for (int i=0; i < width; i++){ // Setting everything to white so we dont get nonsense values later, by cycling through all pixels
        whiteBar(imageOutput, i);
    }

    //Start sequence
    quietZone(imageOutput, x);
    x += 9;
    startEndSequence(imageOutput, x);
    x += 3;
    // Loop through digits and set barcode value
    for (int i=0; i < 12; i++){
        // Add the middle section in after the 6th digit
        if(i == 6){
            middleBit(imageOutput, x);
            // Advance 5 stripes
            x += 5;
        }
        // Get 7-bit value of the digit
        int mask = getBit(data[i] - '0');
        // Check which half we are in
        if (i >= 6){
            mask = mask ^ 0b1111111;
        }
        // Make bars white/black for each bit
        fillDigit(imageOutput, mask, x);
        x += 7;
    }
    // End sequence
    startEndSequence(imageOutput, x);
    x += 3;
    quietZone(imageOutput, x);
    x += 9;

    // Return final image
    return imageOutput;
}

