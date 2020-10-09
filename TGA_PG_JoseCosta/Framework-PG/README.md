# TGA de Processamento Gráfico - UNISINOS
O trabalho consiste no desenvolvimento de uma cena ou jogo utilizando OpenGL. É uma cena simples: o personagem principal pode se locomover pelo cenário, e é exibido no console a mensagem "You died!", caso encoste na caixa de TNT.

## Para executar
Foram utilizadas libraries compiladas especificamente para o uso com o G++ do MinGW. Para gerar o executável deste projeto, pode ser utilizado o seguinte comando:

	g++ -Wall ./Source/*.cpp ./Source/*.c -I. -g -lglfw3 -lopengl32 -lglu32 -lgdi32

## Construído com
* C++
* OpenGL (GLFW + GLAD)
* STB

## Ambiente
* [MinGW](http://mingw.org/)
* [VS Code](https://code.visualstudio.com/)

## Objetivos
* Criação do background e cenário - OK
* Aplicação do efeito de Parallax - OK
* Uso de sprites para animação do personagem - OK
* Arquivo de configuração principal
* Controle do personagem com o teclado - OK
* Inserção de objetos adicionais - OK
* Controle de colisão - OK

## Autores
* [brunovieira97](https://www.github.com/brunovieira97) - Bruno Vieira
* [juliorenner](https://www.github.com/juliorenner) - Julio Renner
