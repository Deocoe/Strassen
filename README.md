# Algoritmo de Strassen para Multiplicação de Matrizes $(n * n)$
### Do Trabalho Proposto
 Este projeto foi um trabalho acadêmico, onde o principal objetivo era implementar o algoritmo de Strassen para multiplicação de matrizes seguindo algumas diretrizes:
 
 * O programa deve ser implementado em linguagem C;
 * O programa deve executar no run.codes em menos de 1 segundo;  
 * Não é permitido o uso de bibliotecas especializadas, sendo permitido executar operações de tipos primitivos (int). E sendo permitido realizar outros tipos de operações, desde que eu mesmo às implemente;

Para o trabalho em questão, foi utilizado input de imagens em formato PPM pela facilidade de leitura, compreensão e manipulação. Existem algumas codificações do formato PPM, mas o formato utilizado para este projeto, foi o P3, que tem a seguinte forma:

* Começa com o nome da codificação utilizada "P3";  
* Em seguida a largura e a altura;  
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
* **Dividir**: As matrizes de entrada vão sendo divididas em matrizes proporcionalmente menores. Para cada matriz quadrada de tamanho $n$, vamos gerar submatrizes de tamanho $n/2$;

![](https://latex.codecogs.com/svg.image?\large&space;&space;A=\begin{pmatrix}A\tiny{11}&A\tiny{12}\\A\tiny{21}&A\tiny{22}\end{pmatrix})




  
* **Calcular os Produtos**:
