#include <stdio.h>
#include <stdlib.h>



// Início do arquivo Strassen.h
typedef struct {
    int red;
    int green;
    int blue;
} Pixel;

typedef struct {
    int largura;
    int altura;
    int faixa_cor;
    Pixel** pixels;  // Matriz de pixels
} Imagem;

// Funções para Imagens
void le_arquivo(const char* path_arquivo, Imagem* imagem, Imagem* filtro); // Não utilizada na entrega final
void libera_memoria(const Imagem* imagem);

// Funções para Matrizes
Pixel** aloca_matriz(const int n);
void libera_matriz(Pixel** matriz, const int n);

void soma_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
void subtrai_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
void separar_matriz(const Pixel** matrizA, Pixel** A11, Pixel** A12, Pixel** A21, Pixel** A22, const int n);
void multiplica_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
void juntar_submatrizes(Pixel** matrizC, const Pixel** C11, const Pixel** C12, const Pixel** C21, const Pixel** C22, const int n);

// Função principal de Strassen
Pixel** strassen(Pixel** matrizA, Pixel** matrizB, const int n);
// Fim do arquivo Strassen.h;


// Início do arquivo Strassen.c;
//#include "strassen.h"
#define caso_base 32 // Caso Base para o Algoritmo de Strassen utilizar a Multiplicação de Matrizes Tradicional;

// Lê o arquivo e aloca memória para a imagem e filtro;
void le_arquivo(const char* path_arquivo, Imagem* imagem, Imagem* filtro)
{
    FILE* arquivo = fopen(path_arquivo, "r"); // Abre o arquivo para Leitura ("r" -> Read);
    if (!arquivo)
    {
        printf("Parece que deu ruim ao abrir o arquivo %s, meu chapa!\n", path_arquivo);
        exit(1);
    }
    // Lê os dados da imagem;
    if (fscanf(arquivo, "P3\n%d %d\n%d\n", &imagem->largura,
                                                &imagem->altura,
                                                &imagem->faixa_cor) != 3)
    {
        printf("Parece que deu ruim na leitura dos dados da imagem, meu chapa!\n");
        fclose(arquivo);
        exit(1);
    }
    // Alocando memória para os pixels da imagem;
    imagem->pixels = calloc(imagem->altura, sizeof(Pixel*));
    if (!imagem->pixels)
    {
        printf("Parece que deu ruim na alocação de memória pros pixels da imagem, meu chapa!\n");
        fclose(arquivo);
        exit(1);
    }
    for (int i = 0; i < imagem->altura; i++)
    {
        imagem->pixels[i] = calloc(imagem->largura, sizeof(Pixel));
        if (!imagem->pixels[i])
        {
            printf("Parece que deu ruim na alocação de memória pra linha %d, meu chapa!.\n", i);
            for (int j = 0; j < i; j++)
            {
                free(imagem->pixels[j]);
            }
            fclose(arquivo);
            libera_memoria(imagem); // Libera qualquer memória já alocada
            exit(1);
        }
        for (int j = 0; j < imagem->largura; j++)
        {
            if (fscanf(arquivo, "%d %d %d", &imagem->pixels[i][j].red,
                                                  &imagem->pixels[i][j].green,
                                                  &imagem->pixels[i][j].blue) != 3)
            {
                printf("Parece que deu ruim na leitura dos pixels, meu chapa!\n");
                fclose(arquivo);
                libera_memoria(imagem);
                exit(1);
            }
        }
    }

    // Lê os dados do filtro;
    filtro->largura = filtro->altura = imagem->largura; // Tamanho do filtro deve ser igual ao da imagem;
    filtro->pixels = calloc(filtro->altura, sizeof(Pixel*));
    if (!filtro->pixels)
    {
        printf("Parece que deu ruim na alocação de memória pros pixels do filtro, meu chapa!\n");
        fclose(arquivo);
        libera_memoria(imagem);
        exit(1);
    }
    for (int i = 0; i < filtro->altura; i++)
    {
        filtro->pixels[i] = calloc(filtro->largura, sizeof(Pixel));
        if (!filtro->pixels[i])
        {
            printf("Parece que deu ruim na alocação de memória pra linha %d do filtro, meu chapa!\n", i);
            for (int j = 0; j < i; j++)
            {
                free(filtro->pixels[j]);
            }
            fclose(arquivo);
            libera_memoria(imagem);
            libera_memoria(filtro);
            exit(1);
        }
        for (int j = 0; j < filtro->largura; j++)
        {
            if(fscanf(arquivo, "%d %d %d", &filtro->pixels[i][j].red,
                                                 &filtro->pixels[i][j].green,
                                                 &filtro->pixels[i][j].blue) != 3)
            {
                printf("Parece que deu ruim na leitura dos pixels do filtro, meu chapa!\n");
                fclose(arquivo);
                libera_memoria(imagem);
                libera_memoria(filtro);
                exit(1);
            }
        }
    }

    fclose(arquivo);
}

// Libera memória da imagem;
void libera_memoria(const Imagem* imagem)
{
    for (int i = 0; i < imagem->altura; i++)
    {
        free(imagem->pixels[i]); // Libera cada pixel[i];
    }
    free(imagem->pixels); // Libera o vetor de ponteiros para os pixels;
}

// Aloca memória pra a matriz;
Pixel** aloca_matriz(const int n)
{
    Pixel** matriz = (Pixel**)calloc(n,  sizeof(Pixel*));
    if (!matriz)
    {
        printf("Parece que deu ruim ao alocar memória pra matriz, meu chapa!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        matriz[i] = (Pixel*)calloc(n, sizeof(Pixel));
        if (!matriz[i])
        {
            printf("Parece que deu ruim ao alocar memória pra linha %d da matriz, meu chapa!\n", i);
            for (int j = 0; j < i; j++)
            {
                free(matriz[j]);
            }
            free(matriz);
            exit(1);
        }

    }
    return matriz;
}

// Libera memória da matriz;
void libera_matriz(Pixel** matriz, const int n)
{
    for (int i = 0; i < n; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

// Soma a matrizA e a matrizB e armazena o resultado na matrizC;
void soma_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrizC[i][j].red = matrizA[i][j].red + matrizB[i][j].red;
            matrizC[i][j].green = matrizA[i][j].green + matrizB[i][j].green;
            matrizC[i][j].blue = matrizA[i][j].blue + matrizB[i][j].blue;
        }
    }
}

// Subtrai a matrizB da matrizA e armazena em matrizC;
void subtrai_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrizC[i][j].red = matrizA[i][j].red - matrizB[i][j].red;
            matrizC[i][j].green = matrizA[i][j].green - matrizB[i][j].green;
            matrizC[i][j].blue = matrizA[i][j].blue - matrizB[i][j].blue;
        }
    }
}

// Separa a matrizA nas Submatrizes A11(Q2), A12(Q1), A21(Q3), A22(Q4);
void separar_matriz(const Pixel** matrizA, Pixel** A11, Pixel** A12, Pixel** A21, Pixel** A22, const int n)
{
    const int meio = n / 2;
    for (int i = 0; i < meio; i ++)
    {
        for (int j = 0; j < meio; j++)
        {
            A11[i][j] = matrizA[i][j];
            A12[i][j] = matrizA[i][j + meio];
            A21[i][j] = matrizA[i + meio][j];
            A22[i][j] = matrizA[i + meio][j + meio];
        }
    }
}

void juntar_submatrizes(Pixel** matrizC, const Pixel** C11, const Pixel** C12, const Pixel** C21, const Pixel** C22, const int n)
{
    const int meio = n / 2;
    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            matrizC[i][j] = C11[i][j];
            matrizC[i][j + meio] = C12[i][j];
            matrizC[i + meio][j] = C21[i][j];
            matrizC[i + meio][j + meio] = C22[i][j];
        }
    }
}

// Metodo padrão(Normal) de multiplicação de matrizes (tamanho³) = (n³);
void multiplica_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrizC[i][j].red = 0;
            matrizC[i][j].green = 0;
            matrizC[i][j].blue = 0;
            for (int k = 0; k < n; k++)
            {
                matrizC[i][j].red += matrizA[i][k].red * matrizB[k][j].red;
                matrizC[i][j].green += matrizA[i][k].green * matrizB[k][j].green;
                matrizC[i][j].blue += matrizA[i][k].blue * matrizB[k][j].blue;
            }
        }
    }
}

// Algoritmo de Strassen para Multiplicação de Matrizes;
Pixel** strassen(Pixel** matrizA, Pixel** matrizB, const int n)
{
    // Caso onde não será utilizado Strassen por ser um caso considerado pequeno;
    if (n <= caso_base) // Se necessário, alterar o caso_base no #define no começo do código;
    {
        Pixel** matrizC = aloca_matriz(n);
        multiplica_matriz(matrizA, matrizB, matrizC, n);
        return matrizC;
    }

    // Alocação de memória para cada submatriz;
    const int meio = n / 2;
    Pixel** A11 = aloca_matriz(meio);
    Pixel** A12 = aloca_matriz(meio);
    Pixel** A21 = aloca_matriz(meio);
    Pixel** A22 = aloca_matriz(meio);
    Pixel** B11 = aloca_matriz(meio);
    Pixel** B12 = aloca_matriz(meio);
    Pixel** B21 = aloca_matriz(meio);
    Pixel** B22 = aloca_matriz(meio);

    separar_matriz(matrizA, A11, A12, A21, A22, n);
    separar_matriz(matrizB, B11, B12, B21, B22, n);

    // Alocação de matriz auxiliar temporária;
    Pixel** matriz_auxiliar1 = aloca_matriz(meio);

    // P1 = A11 * (B12 - B22);
    subtrai_matriz(B12, B22, matriz_auxiliar1, meio);
    Pixel** P1 = strassen(A11, matriz_auxiliar1, meio);

    // P2 = (A11 + A12) * B22;
    soma_matriz(A11, A12, matriz_auxiliar1, meio);
    Pixel** P2 = strassen(matriz_auxiliar1, B22, meio);

    // P3 = (A21 + A22) * B11;
    soma_matriz(A21, A22, matriz_auxiliar1, meio);
    Pixel** P3 = strassen(matriz_auxiliar1, B11, meio);

    // P4 = A22 * (B21 - B11);
    subtrai_matriz(B21, B11, matriz_auxiliar1, meio);
    Pixel** P4 = strassen(A22, matriz_auxiliar1, meio);

    // Segunda matriz auxiliar temporária;
    Pixel** matriz_auxiliar2 = aloca_matriz(meio);

    // P5 = (A11 + A22) * (B11 + B22);
    soma_matriz(A11, A22, matriz_auxiliar1, meio);
    soma_matriz(B11, B22, matriz_auxiliar2, meio);
    Pixel** P5 = strassen(matriz_auxiliar1, matriz_auxiliar2, meio);

    // P6 = (A12 - A22) * (B21 + B22);
    subtrai_matriz(A12, A22, matriz_auxiliar1, meio);
    soma_matriz(B21, B22, matriz_auxiliar2, meio);
    Pixel** P6 = strassen(matriz_auxiliar1, matriz_auxiliar2, meio);

    // P7 = (A11 - A21) * (B11 + B12);
    subtrai_matriz(A11, A21, matriz_auxiliar1, meio);
    soma_matriz(B11, B12, matriz_auxiliar2, meio);
    Pixel** P7 = strassen(matriz_auxiliar1, matriz_auxiliar2, meio);

    // Liberando memória das matrizes auxiliares temporárias;
    libera_matriz(matriz_auxiliar1, meio);
    libera_matriz(matriz_auxiliar2, meio);

    //Liberando memória de A11, A12, A21, A22, B11, B12, B21, B22;
    libera_matriz(A11, meio);
    libera_matriz(A12, meio);
    libera_matriz(A21, meio);
    libera_matriz(A22, meio);
    libera_matriz(B11, meio);
    libera_matriz(B12, meio);
    libera_matriz(B21, meio);
    libera_matriz(B22, meio);

    // Alocando memória para as submatrizees da matrizC;
    Pixel** C11 = aloca_matriz(meio);
    Pixel** C12 = aloca_matriz(meio);
    Pixel** C21 = aloca_matriz(meio);
    Pixel** C22 = aloca_matriz(meio);

    // C11 = P5 + P4 - P2 + P6
    soma_matriz(P5, P4, C11, meio);     //  P5 + P4;
    subtrai_matriz(C11, P2, C11, meio); // (P5 + P4) - P2;
    soma_matriz(C11, P6, C11, meio);    // (P5 + P4 - P2) + P6;

    // C12 = P1 + P2
    soma_matriz(P1, P2, C12, meio);     //  P1 + P2;

    // C21 = P3 + P4
    soma_matriz(P3, P4, C21, meio);     //  P3 + P4;

    // C22 = P5 + P1 - P3 - P7
    soma_matriz(P5, P1, C22, meio);     //  P5 + P1;
    subtrai_matriz(C22, P3, C22, meio); // (P5 + P1) - P3;
    subtrai_matriz(C22, P7, C22, meio); // (P5 + P1 - P3) - P7;

    // Liberando memória de P1, P2, P3, P4, P5, P6 e P7;
    libera_matriz(P1, meio);
    libera_matriz(P2, meio);
    libera_matriz(P3, meio);
    libera_matriz(P4, meio);
    libera_matriz(P5, meio);
    libera_matriz(P6, meio);
    libera_matriz(P7, meio);

    // Juntando as quatro submatrizes em uma;
    Pixel** matrizC = aloca_matriz(n);
    juntar_submatrizes(matrizC, C11, C12, C21, C22, n);

    //Liberando memória de C11, C12, C21, C22;
    libera_matriz(C11, meio);
    libera_matriz(C12, meio);
    libera_matriz(C21, meio);
    libera_matriz(C22, meio);

    return matrizC;
}
// Fim do arquivo Strassen.c


// Função Main;
int main(void) {

    // Inicializa as estruturas de imagem e filtro;
    Imagem imagem;
    Imagem filtro;

    char formato[3];
    scanf("%s", formato);                               // P3
    scanf("%d %d\n%d", &imagem.largura, &imagem.altura, // n n
                             &imagem.faixa_cor);              // 255

    imagem.pixels = aloca_matriz(imagem.altura);

    // Lê os dados da imagem;
    for (int i = 0; i < imagem.altura; i++)
    {
        for (int j = 0; j < imagem.largura; j++)
        {
            scanf("%d %d %d", &imagem.pixels[i][j].red,
                                    &imagem.pixels[i][j].green,
                                    &imagem.pixels[i][j].blue);
        }
    }

    // O tamanho da imagem e do filtro (n * n);
    const int n = imagem.largura; // imagem.largura = imagem.altura = n = filtro.largura = filtro.altura;

    filtro.largura = filtro.altura = n;
    filtro.pixels = aloca_matriz(filtro.altura);

    // Lê os dados do filtro;
    for (int i = 0; i < filtro.altura; i++)
    {
        for (int j = 0; j < filtro.largura; j++)
        {
            scanf("%d %d %d", &filtro.pixels[i][j].red,
                                    &filtro.pixels[i][j].green,
                                    &filtro.pixels[i][j].blue);
        }
    }

    Pixel** resultado = aloca_matriz(n);
    resultado = strassen(imagem.pixels, filtro.pixels, n);

    // Imprimindo o resultado no formato PPM;
    printf("P3\n%d %d\n255\n", n, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d %d %d ", resultado[i][j].red,
                                      resultado[i][j].green,
                                      resultado[i][j].blue);
        }
        printf("\n");
    }

    // Liberando memória da imagem, filtro e Matriz de Resultado;
    libera_memoria(&imagem);
    libera_memoria(&filtro);
    libera_matriz(resultado, n);

    return 0;
}