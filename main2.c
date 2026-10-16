
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

/* Prototipos */
void load(char *name, Img *pic);

void confusao(Pixel *src, Pixel *dst, int total);
void desconfusao(Pixel *src, Pixel *dst, int total);

void difusao(Pixel *src, Pixel *dst, int total, int shift);
void dedifusao(Pixel *src, Pixel *dst, int total, int shift);


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: bagunceitor [imagem]\n");
        return 1;
    }

    /* Carrega a imagem original */
    load(argv[1], &in);

    int total = in.width * in.height;
    int shift = total / 3;

    out.width = in.width;
    out.height = in.height;
    out.channels = 3;

    /* Reserva memoria para os vetores */
    out.pixels = malloc(total * sizeof(Pixel));
    Pixel *temp = malloc(total * sizeof(Pixel));
    Pixel *recuperada = malloc(total * sizeof(Pixel));

    if (out.pixels == NULL ||
        temp == NULL ||
        recuperada == NULL) {

        printf("Erro ao alocar memoria.\n");

        free(in.pixels);
        free(out.pixels);
        free(temp);
        free(recuperada);

        return 1;
    }

    printf("Imagem original: %d x %d\n",
           in.width, in.height);

    /* 1. CONFUSAO */
    confusao(in.pixels, out.pixels, total);

    /* 2. DIFUSAO */
    difusao(out.pixels, temp, total, shift);

    /* Copia a imagem baguncada para out */
    memcpy(out.pixels, temp, total * sizeof(Pixel));

    /* Salva a imagem baguncada */
    if (stbi_write_png("saida.png",
                       out.width, out.height, 3,
                       out.pixels, 0)) {
        printf("Imagem baguncada salva em saida.png\n");
    } else {
        printf("Erro ao salvar saida.png\n");
    }

    /* 3. INVERSA DA DIFUSAO */
    dedifusao(out.pixels, temp, total, shift);

    /* 4. INVERSA DA CONFUSAO */
    desconfusao(temp, recuperada, total);

    /* 5. Verifica se a recuperacao foi exata */
    if (memcmp(in.pixels, recuperada,
               total * sizeof(Pixel)) == 0) {
        printf("SUCESSO: imagem recuperada exatamente!\n");
    } else {
        printf("ERRO: imagem recuperada e diferente da original!\n");
    }

    /* Salva a imagem recuperada */
    if (stbi_write_png("recuperada.png",
                       in.width, in.height, 3,
                       recuperada, 0)) {
        printf("Imagem recuperada salva em recuperada.png\n");
    } else {
        printf("Erro ao salvar recuperada.png\n");
    }

    /* Libera a memoria */
    free(in.pixels);
    free(out.pixels);
    free(temp);
    free(recuperada);

    return 0;
}


/* CONFUSAO: modifica os valores RGB */
void confusao(Pixel *src, Pixel *dst, int total) {
    for (int i = 0; i < total; i++) {
        dst[i].r = (src[i].r + 37) % 256;
        dst[i].g = (src[i].g + 73) % 256;
        dst[i].b = (src[i].b + 109) % 256;
    }
}


/* INVERSA DA CONFUSAO: recupera os valores RGB */
void desconfusao(Pixel *src, Pixel *dst, int total) {
    for (int i = 0; i < total; i++) {
        dst[i].r = (src[i].r + 256 - 37) % 256;
        dst[i].g = (src[i].g + 256 - 73) % 256;
        dst[i].b = (src[i].b + 256 - 109) % 256;
    }
}


/* DIFUSAO: desloca circularmente os pixels */
void difusao(Pixel *src, Pixel *dst, int total, int shift) {
    for (int i = 0; i < total; i++) {
        int destino = (i + shift) % total;
        dst[destino] = src[i];
    }
}


/* INVERSA DA DIFUSAO: restaura as posicoes */
void dedifusao(Pixel *src, Pixel *dst, int total, int shift) {
    for (int i = 0; i < total; i++) {
        int origem = (i + shift) % total;
        dst[i] = src[origem];
    }
}


/* Le a imagem e converte os pixels para RGB */
void load(char *name, Img *pic) {
    pic->pixels = (Pixel *) stbi_load(
        name,
        &pic->width,
        &pic->height,
        &pic->channels,
        3
    );

    if (pic->pixels == NULL) {
        printf("Erro de leitura: %s\n",
               stbi_failure_reason());
        exit(1);
    }

    printf("Imagem carregada: %d x %d\n",
           pic->width, pic->height);
}
