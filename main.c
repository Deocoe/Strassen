#include <stdio.h>
#include "strassen.h"

int main(void) {
    // Inicializa as estruturas de imagem e filtro;
    Imagem imagem;
    Imagem filtro;
    FILE *arq = fopen("../Testes_Output/1.txt", "w"); // Abre o arquivo para Escrita ("w" -> Write);
    if (!arq)
    {
       printf("Parece que deu ruim ao abrir o arquivo de Output, meu chapa!\n");
        return 1;
    }

    // Leitura do arquivo de entrada
    le_arquivo("../Testes/1.in", &imagem, &filtro);

    // O tamanho da imagem e do filtro
    const int n = imagem.largura; // imagem.largura = imagem.altura = n = filtro.largura = filtro.altura

    Pixel** resultado = aloca_matriz(n);

    resultado = strassen(imagem.pixels, filtro.pixels, n);

    // Imprimindo o resultado no formato PPM;
    fprintf(arq, "P3\n%d %d\n255\n", n, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(arq, "%d %d %d ", resultado[i][j].red, resultado[i][j].green, resultado[i][j].blue);
        }
        fprintf(arq, "\n");
    }
    fclose(arq);

    // Liberar memória
    libera_memoria(&imagem);
    libera_memoria(&filtro);
    libera_matriz(resultado, n);

    return 0;
}