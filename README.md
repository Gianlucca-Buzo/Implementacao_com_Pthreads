# Trabalho_Pratico-Implementacao_com_Pthreads
#### Membros do grupo: Darlei Matheus Schmegel, Dauan Ghisleni Zolinger, Gianlucca de Mendonça Buzô, Heitor Felipe Matozo dos Santos e Letícia Pegoraro Garcez.

[Acesse o vídeo de apresentação do trabalho](https://www.youtube.com/watch?v=9GyFEU691D4) **Em breve**

### O que é este projeto?
Este projeto se trata de uma biblioteca com suporte de execução de threads utilizando o modelo *n x m* para uma linguagem concorrente baseada na criação de tarefas utilizando a linguagem C++ e a biblioteca de threads Pthreads.

Este projeto também representa um trabalho avaliativo para a disciplina de Sistemas Operacionais cursada no semestre de 2020/2.

### Como rodar este projeto?
Para testar o funcionamento do código, é necessário fazer o download do repositório, que pode ser feito através de um arquivo *.zip* ou clonando o repositório. 

Uma vez com uma versão dos arquivos deste repositório em sua máquina, o usuário deve descompactar os arquivos, caso tenha optado pelo download em formato *.zip* e abrir a pasta denomindada *src*, onde estão os arquivos fontes do projeto.

O projeto pode ser executado através de linha de comando o comando `make` que fará a compilação e a execução de maneira automática, ou ainda com os seguintes comandos na linha de comando (lembrando que é necessário estar no diretório *src* para que os comandos funcionem):

   <code class="hljs language-shell">$ g++ main.cpp pokeLib.cpp -lpthread -o main</code>
   
   <code class="hljs language-shell">$ ./main</code>

### Especificações do programa de exemplo desenvolvido
O algoritmo implementado no arquivo `main.cpp` se trata de um menu onde é possível selecionar um entre dois algorimot para execução, sendo um deles o cálculo de um Fibonacci, e o segundo a execução de um algortimo de ordenação *merge sort*. Após selecionar o algoritmo que deseja executar, o usuário deve selecionar um entre dois tipos de escalonamento implementados, bem como a quantidade de threads que deseja utilizar para realizar a execução do algoritmo.

Como políticas de escalonamento foram implementadas as lógicas de *FIFO (First-in-first-out)* e *LIFO (Last-in-first-out)*, sendo o escalonamento *FIFO* a opção padrão. 

### Extras incluídos no trabalho
- Segundo método de escalonamento (LIFO);
- Segundo algoritmo de exemplo (merge sort).


