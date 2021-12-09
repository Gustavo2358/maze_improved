Maze_improved
======

# Relatório tarefa 3



* Link do repositório: https://github.com/Gustavo2358/maze_improved
    * O código se encontra na pasta `examples/maze`.
* Link do WebAssembly: https://gustavo2358.github.io/maze_improved/maze/maze.html

Esse projeto utiliza algumas técnicas da disciplina de computação gráfica da UFABC. Consiste em uma cena simples 3d que renderiza um labirinto e um coelho dourado que representa o fim do labirinto. É em uma versão com textura e iluminação da tarefa 2. 

### Iluminação e textura

A iluminação e textura do chão, das paredes e do coelho utilizam a técnica de mapeamento de normais, utilizando o mesmo shader do projeto viewer5 desenvolvido na disciplina. A textura dourada do coelho foi obtida no site [3dtextures.me](https://3dtextures.me/2018/01/01/metal-weave-002/). O fundo foi feito utilizando a técnica de mapeamento cubico de ambientes, e o código foi adaptado do projeto viewer6 na classe 'Skybox', também desenvolvido na disciplina. 

### Cenário
Esse projeto é baseado no projeto Lookat, e possui as mesmas mecânicas de controle da câmera e rederização do chão. O Labirinto é renderizado utilizando o modelo box.obj, onde o mesmo VBO é renderizado várias vezes, mas em posições diferentes, utilizando a matriz de translação. As posições dos blocos são definidas através de uma matriz que contem o "desenho" do labirinto, utilizando o número 1 para as posições onde os blocos serão renderizados e 0 para onde não vai haver blocos. 

### Colisões
As colisões com as paredes são detectadas através de quatro pontos, na frente, atrás, na esquerda e na direita da cãmera. Esses pontos são computados através da operação de soma de ponto com vetor, onde a posição atual da câmera é somáda a um vetor "forward" (e -forward para o ponto de trás) e um vetor "left" para as colisões laterais, obtido através do produto vetorial entre o vetor "up" e o vetor "forward". Esses vetores impedem que a câmera continue andando quando se aproxíma de uma parede. 

### Coelho
Um modelo bunny.obj é rederizado em um ponto do labirinto para simbolizar o fim da trilha. O coelho tem seu próprio buffer de vértices (que é gerenciado pela classe Model).

