
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>
#include <stb_image_write.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width, height;
    int channels;
    Pixel *pixels;
} Img;

Img in, out;

void load(char *name, Img *pic);

void confusao(Pixel *src, Pixel *dst, int total);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: bagunceitor [imagem]\n");
        return 1;
    }

    load(argv[1], &in);

    int total = in.width * in.height;
    int shift = total / 3;

    out.width = in.width;
    out.height = in.height;
    out.channels = 3;

    out.pixels = malloc(total * sizeof(Pixel));
    Pixel *temp = malloc(total * sizeof(Pixel));
    Pixel *recuperada = malloc(total * sizeof(Pixel));

    if (out.pixels == NULL || temp == NULL || recuperada == NULL) {
        printf("Erro ao alocar memoria.\n");

        free(in.pixels);
        free(out.pixels);
        free(temp);
        free(recuperada);
        return 1;
    }

    printf("Imagem original: %d x %d\n",
           in.width, in.height);

    /* 1. CONFUSAO: original -> out */
    confusao(in.pixels, out.pixels, total);
}

/* Confusao: altera os valores  */
void confusao(Pixel *src, Pixel *dst, int total) {
    for (int i = 0; i < total; i++) {
        dst[i].r = (src[i].r + 37) % 256;
        dst[i].g = (src[i].g + 73) % 256;
        dst[i].b = (src[i].b + 109) % 256;
    }
}

