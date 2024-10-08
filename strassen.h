#ifndef STRASSEN_H
#define STRASSEN_H

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
void le_arquivo(const char* path_arquivo, Imagem* imagem, Imagem* filtro);
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

#endif // STRASSEN_H
