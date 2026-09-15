#include "barcode.h"
#include "loader.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv){
    // Parse the arguments in argv
    char *digits = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char *filepath = argv[4];

    // Handle argument # error
    if(argc != 5) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <barcode_value> <width> <height> <output_image_path>\n");
        return -1;
    }

    // Call barcode
    struct image* barcodeImg = barcode(digits, width, height);

    // Save to file
    saveimage(filepath, barcodeImg);

    // Free mallocs from barcode
    free(barcodeImg->pixels);
    free(barcodeImg);

    return 0;
}
