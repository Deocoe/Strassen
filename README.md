# Algoritmo de Strassen para Multiplicação de Matrizes $(n * n)$
### Do Trabalho Proposto
 Este projeto foi um trabalho acadêmico da disciplina **Projeto e Análise de Algoritmos - CTCO04**, proposto pelo professor **Pedro Henrique Del Bianco Hokama**, onde o principal objetivo é implementar o algoritmo de Strassen para multiplicação de matrizes seguindo algumas diretrizes:
 
 * O programa deve ser implementado em linguagem C;
 * O programa deve executar no run.codes em menos de 1 segundo;  
 * Não é permitido o uso de bibliotecas especializadas, sendo permitido executar operações de tipos primitivos (int). E sendo permitido realizar outros tipos de operações, desde que eu mesmo às implemente;

Para o trabalho em questão, foi utilizado input de imagens em formato PPM pela facilidade de leitura, compreensão e manipulação. Existem algumas codificações do formato PPM, mas o formato utilizado para este projeto, foi o P3, que tem a seguinte forma:

* Começa com o nome da codificação utilizada "P3";  
* Em seguida a largura e a altura → Serão utilizadas apenas matrizes n * n onde n é uma potência de 2; 
* Um inteiro com a escala máxima de cor, normalmente sendo o valor 255;  
* Seguido de $n$ linhas, cada linha contém $n$ pixels;  
 Exemplo:
     >P3 //Formato  
4 4  //Largura e Altura  
255  //Faixa de Cor  
255 0 0 255 0 0 0 255 0 0 255 0  
255 0 0 255 0 0 0 255 0 0 255 0  
0 0 255 0 0 255 0 255 255 0 255 255  
0 0 255 0 0 255 0 255 255 0 255 255  
0 0 0 0 0 0 0 0 0 1 1 0  
0 0 0 0 0 0 1 1 0 0 0 0  
0 0 0 1 1 0 0 0 0 0 0 0  
1 1 0 0 0 0 0 0 0 0 0 0 

### Do Algoritmo de Strassen
O Algoritmo de Strassen, desenvolvido por **Volker Strassen** em 1969, é uma técnica para multiplicação de matrizes que se baseia na estratégia de dividir para conquistar. Sendo mais eficiente do que a forma tradicional de multiplicação de matrizes, que possui complexidade cúbica $O(n^3)$.  
O Algoritmo de Strassen tem complexidade $O(n^{log_27})≈O(n^{2.81})$, sendo muito eficaz para matrizes de grande dimensão.  

#### Como Strassen Funciona ?
O processo pode ser dividido em 3 etapas principais:  
* **Dividir as Matrizes de Entrada**: As matrizes de entrada vão sendo divididas em matrizes proporcionalmente menores. Para cada matriz quadrada de tamanho $n$, vamos gerar submatrizes de tamanho $n/2$;

```math
A =
\begin{pmatrix}
A_{11} & A_{12} \\\
A_{21} & A_{22}
\end{pmatrix}
\,\,\,\,\,\,\,\,\,
B = 
\begin{pmatrix}
B_{11} & B_{12} \\\
B_{21} & B_{22}
\end{pmatrix}
```
Neste cenário, a Matriz A será dividida em $A_{11}$, $A_{12}$, $A_{21}$ e $A_{22}$;  
Enquanto a Matriz B será dividida em $B_{11}$, $B_{12}$, $B_{21}$ e $B_{22}$.

* **Calcular os Produtos Intermediários**: São calculados sete produtos intermediários ($P_1$, $P_2$, $P_3$, $P_4$, $P_5$, $P_6$, $P_7$) usando combinações das submatrizes:
 
   $P_1 = A_{11}\cdot(B_{12}-B_{22})$  
   $P_2 = (A_{11}+ A_{12})\cdot B_{22}$  
   $P_3 = (A_{21} + A_{22})\cdot B_{11}$  
   $P_4 = A_{22}\cdot(B_{21} - B_{11})$  
   $P_5 = (A_{11} + A_{22})\cdot(B_{11} + B_{22})$  
   $P_6 = (A_{12} - A_{22})\cdot(B_{21} + B_{22})$  
   $P_7 = (A_{11} - A_{21})\cdot(B_{11} + B_{12})$
  
* **Combinar as Submatrizes Resultantes**: Cada submatriz da matriz final passa por algumas operações, e em seguida as submatrizes-resultado são combinadas em uma única matriz final:

   $C_{11} = P_5 + P_4 - P2 + P6$  
   $C_{12} = P_1 + P_2$  
   $C_{21} = P_3 + P_4$  
   $C_{22} = P_5 + P_1 - P_3 - P_7$

  Após realizar as operações acima para obter os valores finais de $C_{11}$, $C_{12}$, $C_{21}$ e $C_{22}$, basta juntas as 4 submatrizes na matriz C:

```math
C=
\begin{pmatrix}
C_{11} & C_{12} \\\
C_{21} & C_{22}
\end{pmatrix}
```

Com isso, basta utilizar o resultado obtido da matriz C da forma que for necessária, e o algoritmo é encerrado!

### Da Implementação
#### Structs Utilizadas:  
```C
typedef struct {
    int red;         // Representação do Pixel Vermelho;
    int green;       // Representação do Pixel Verde;
    int blue;        // Representação do Pixel Azul;
} Pixel;

typedef struct {
    int largura;     // Largura da Imagem(n);
    int altura;      // Altura da Imagem (n);
    int faixa_cor;   // Faixa de Cor (RGB) → 255;
    Pixel** pixels;  // Matriz de pixels;
} Imagem;
```
#### Funções Utilizadas:  
```C
void le_arquivo(const char* path_arquivo, Imagem* imagem, Imagem* filtro); → Não utilizada na Entrega Final  
```  
A função le_arquivo recebe como parâmentro o caminho para um arquivo de texto, que conterá uma imagem e um filtro $(n * n)$,
 ela preenche a imagem(matrizA) e o filtro(matrizB) com os dados de altura, largura e pixels(R,G,B).<br/><br/>
```C
void libera_memoria(const Imagem* imagem);  
``` 
A função libera_memória libera cada pixel alocado na imagem, e depois libera a imagem alocada.<br/><br/>
```C
Pixel** aloca_matriz(const int n);
```  
A função aloca_matriz aloca n * Pixel** de memória para uma matriz. <br/><br/>
```C
void libera_matriz(Pixel** matriz, const int n);
```  
Assim como a função libera_memória, a função libera_matriz itera sob o tamanho da matriz, liberando toda a memória alocada por ela.<br/><br/>
```C
void soma_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
```  
A função soma_matriz soma duas matrizes RGB, de forma que cada componente RGB realize operações com a mesma componente.<br/><br/>
```C
void subtrai_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
```  
A função subtrai_matriz subtrai de cada componente RGB da matrizA, a compontente RGB da matrizB.<br/><br/>
```C
void separar_matriz(const Pixel** matrizA, Pixel** A11, Pixel** A12, Pixel** A21, Pixel** A22, const int n);  
```  
A função separar_matriz é função responsável por separar a matrizA nas submatrizes $A_{11}$, $A_{12}$, $A_{21}$, $A_{22}$, $B_{11}$, $B_{12}$, $B_{21}$, $B_{22}$.<br/><br/>
```C
void multiplica_matriz(const Pixel** matrizA, const Pixel** matrizB, Pixel** matrizC, const int n);
```  
A função multiplica_matriz realiza a multiplicação "tradicional" de matrizes, de complexidade $O(n^3)$. É utilizada para calcular os produtos intermediários ($P_1$, $P_2$, $P_3$, $P_4$, $P_5$, $P_6$, $P_7$).<br/><br/>
```C
void juntar_submatrizes(Pixel** matrizC, const Pixel** C11, const Pixel** C12, const Pixel** C21, const Pixel** C22, const int n);
```
A função juntar_submatrizes recebe as submatrizes $C_{11}$, $C_{12}$, $C_{21}$ e $C_{22}$ e forma a matrizC.<br/><br/>
```C 
Pixel** strassen(Pixel** matrizA, Pixel** matrizB, const int n);  
```
A função strassen é a principal função, utilizando das funções previamente citadas para executar o **Algoritmo de Strassen** em matrizes $(n * n)$.<br/><br/>

 #### Problemas Enfrentados:
 1. Inicialmente, antes de pensar em utilizar structs, eu comecei a implementação utilizando ponteiro de ponteiro de ponteiro, o que parecia ser menos difícil quando eu desenhava a estrutura no papel, mas na hora de implementar foi bem confuso, o que me levou a utilizar structs;  
 2. Fui então seguindo a lógica do algoritmo, e o próximo problema com que me deparei, foi que não considerei, na hora de somar, subtrair e multiplicar, as três componentes RGB, e foi considerávelmente fácil de solucionar;  
 3. Para testar, após finalizado, eu já lia o input direto do arquivo, e escrevia em outro arquivo. Utilizei de um código em Python que compara linha por linha de arquivos de texto. E os arquivos 1.in e 2.in (matrizes $4*4$) apresentavam os outputs esperados, já os outros, não;  
 4. Depois de muitas horas procurando por possíveis erros no código, por possíveis vazamentos de memória, eu:
    * Fiz a substituição da função malloc para alocação dinâmica, pela função calloc: Já inicializaria os dados com zero, evitando que pudesse ter algum lixo de memória atrapalhando a execução limpa do código;
    * Decidi refazer o código do zero → Assim que terminei, eu fui comparar os dois códigos feitos, e foi então que encontrei o erro do primeiro código:
      
      * Um erro de digitação na função Strassen: Na hora de calcular $C_{22} = P_5 + P_1 - P_3 - P_7$, eram feitas três chamadas de função, uma soma_matriz, e duas subtrai_matriz, e nessas chamadas, ao invés de guardar o resultado obtido em C22, eu estava guardando em C11, por ter cometido um erro de digitação.
     
 #### Conclusão
 Mesmo que o formato de correção necessitando que o input fosse feito por scanf me incomodando(achei mais chato fazer a main para entrega do que o resto da implementação do algoritmo), foi um trabalho prazeroso de ser implementado, onde o desperdício de memória era algo que deveria ser levado em conta, o que me obrigou a otimizar o tratamento com a memória, e as verificações a cada alocação. Apesar de não ser um problema caso a implementação seja feita corretamente, achei legal ter um "tempo de execução limite".
 
 
