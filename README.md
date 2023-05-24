# Space Invaders em C

## Descrição do Problema
O objetivo deste trabalho é que o estudante aplique os conceitos de threads, exclusão mútua e coordenação de processos por meio do projeto e implementação de uma versão (muito) simplificada do jogo clássico Space Invaders.

Nessa versão, 20 naves alienígenas prepara um ataque à Terra, cuja única defesa é uma bateria antiaérea que dispara foguetes tentando abater as naves. Diferentemente do jogo original, das naves não saltam alienígenas.

As naves deslocam-se da direita para a esquerda da tela, em diferentes alturas, da metade da tela para cima. O jogador controla a bateria antiaérea disparando contra as naves. A bateria dispara sempre na vertical. O disparo é feito via teclado ou mouse. O jogo acaba quando se esgotou o tempo t para que todas 20 naves fossem abatidas (nesse caso, o jogador e a Terra perderam) ou quando as 20 naves foram derrubadas.

### Algumas Restrições
1. A capacidade de fabricação de foguetes é infinita, mas a bateria só consegue armazenar 6 foguetes;
2. O jogo tem 3 níveis de dificuldade:
    * fácil: as naves se  deslocam não muito depressa;
    * médio: as naves se deslocam mais depressa;
    * difícil: as naves se deslocam muito rapidamente.
3. O jogo TEM que ser implementado em C ou C++ e usando pthreads.

## Instruções de uso
**Código de uso restrito em sistemas operacionais windows**

Para compilar, usar o seguinte comando:
```console
gcc -pthread space_invaders.c -o "nome_do_arquivo_destino"
```
