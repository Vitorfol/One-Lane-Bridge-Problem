Problema da Ponte de Faixa Única

Trabalho Prático Programação Concorrente e Paralela 2026.2

Apresentação
O  problema  da  ponte  de  faixa  única  é  um  clássico  problema  de  programação  concorrente,
também conhecido como One-Lane Bridge Problem.  É um tipo de ponte muito comum em áreas
rurais  onde  o  tráfego  é  baixo  e  esporádico.  Mas  se  o  movimento  crescer,  os  problemas
aparecem...

A ponte possui apenas uma faixa, permitindo a passagem de veículos em apenas um sentido por
vez. Em determinadas situações, vários veículos podem atravessar juntos, desde que estejam
no mesmo sentido. Se houver veículos no sentido contrário, é necessário aguardar a travessia
deste terminar antes de iniciar a travessia. Como não há semáforo ou sinalização, cada motorista
deve observar o fluxo e decidir se atravessa ou espera.

O  trabalho  consiste  em  propor  e  implementar  um  algoritmo  concorrente  para  resolver  o
problema da ponte de uma faixa sem deadlock ou starvation.

Proposta do Problema
O problema é constituído de quatro etapas em ordem crescente de complexidade.

1) Somente carros
Esse problema é só para esquentar.... Considere que n=100 carros irão cruzar a ponte, 50 em
cada sentido. O tempo de chegada de cada carro Ta é aleatório, variando entre 1 segundos e 3
segundos. O tempo de travessia de cada carro pela ponte Tc é de 5 segundos. Considerar um
espeço  Ts  =  1  seg  entre  os  veículos  no  mesmo  sentido,  significando  que  cabem  5  carros
simultaneamente na ponte. Quando um carro chega ponte, deve verificar se o carro que está
atravessando está no mesmo sentido, e assim ele pode atravessar respeitando o intervalo de 1
seg do carro anterior. Se todos os carros em um sentido passarem na ponte, os carros no sentido
inverso poderão atravessar.

Dados n = 100, Ta = 1 a 3 seg, Tc = 5 seg, Ts =1 seg.

2) Somente carros sem starvation
O problema 1) pode causar starvation quando a fila ficar muito cheia no final da execução, isto
é,  pode  ser  que  uma  fila  não  de  oportunidade  para  o  outro  sentido.  Como  não  existe  um
sinalizador  na  ponte  indicando  o  sentido  do  fluxo,  apenas  o  motorista  deve  decidir  se  vai
atravessar a ponte ou esperar.

Crie um mecanismo para equilibrar o uso da ponte, por exemplo, alternando o sentido na ponte
após 5 veículos atravessarem. Mas lembre-se, não existe nenhum sinalizador na ponte, apenas
a decisão dos motoristas.

Dados n = 100, Ta = 1 a 3 seg, Tc = 5 seg, Ts =1 seg,  P=5.

3) Carros e caminhões sem starvation
Agora, nosso problema inclui caminhões que são mais lentos e por questão de segurança, devem
cruzar  a  ponte  sozinho.  Considere  agora  que  n=100  carros  e  m=10  caminhões  irão  cruzar  a
ponte,  50  carros  e  5  caminhões  em  cada  sentido.  O  tempo  de  chegada  de  cada  carro  ou
caminhão Ta é aleatório, variando entre 1 segundos e 3 segundos. O tempo de travessia de cada
carro  pela  ponte  Tc  é  de  5  segundos  e  o  tempo  de  travessia  de  cada  caminhão  Tt  é  de  10
segundos, considerar um espaço Ts =1 seg entre os veículos no mesmo sentido. Por questão de
segurança, um caminhão deverá cruzar a ponte sozinho (espera o carro da frente terminar a
travessia).  Implementar um mecanismo para evitar starvation, invertendo o sentido a cada P=5
carros ou U=1 caminhão

Considere n = 100, m=10, Ta = 1 a 3 seg, Tc = 5 seg, Tt = 10 seg, Ts =1 seg, P=5 ou U=1.

4) Carros e caminhões em duas pontes
No local existe uma ponte de faixa única paralela, muito antiga, que ainda pode ser utilizada por
carros, mas não por caminhões. Nesta ponte antiga podem atravessar 2 carros simultaneamente
(Q=2 e Ts’ = 2 seg) no tempo Tc’ = 4 segs. Na fila única de cada sentido, caso a ponte principal
esteja ocupada é possível encaminhar carros para a ponte antiga e melhorar o fluxo de veículos.
Mas  agora  teremos  120  carros  atravessando.  Neste  problema  deve-se  otimizar  o  tempo  de
passagem, não apenas atravessar sem starvation.

Considere n = 120, m=10, Ta = 1 a 3 seg, Tc = 5 seg, Tc’ = 4 seg, Tt = 10 seg, Ts = 1 seg, Ts’ = 2 seg,
P=5 ou U=1 e Q=2.

Implementação
O trabalho prático consiste em escrever um programa em C, C++, Java ou Python para simular o
problema.

Utilizar as seguintes classes:

  Carros.
  Caminhões.
  Ponte(s).

Ao final da execução deve ser exibidos:

1.  Quantidade de veículos (carros e caminhões) que cruzaram cada sentido.
2.  Tempo mínimo, máximo e médio de espera dos veículos na fila.
3.  Tempo de utilização da ponte (tempo utilizado/tempo total).

Avaliação
O trabalho pode ser feito em grupo com no máximo 3 (três) alunos. A avaliação consiste em
um vídeo mostrando a execução do programa, o código fonte e um relatório escrito.

Apresentação Execução
Os alunos deverão gravar um vídeo mostrando a execução do programa (link do Youtube).

Considera-se  a execução correta se o programa cumprir o determinado sem a ocorrência de
deadlocks ou starvation.

Código fonte
Deve ser encaminha o código fonte zip ou link Github ou similar.

Trabalho Escrito
Os alunos deverão entregar um relatório escrito com os seguintes tópicos:

(a)  Formulação do problema
(b)  Descrição dos algoritmos usados
(c)  Descrição da Implementação (diagrama de classes)
(d)  Resultado da execução do problema com a estatística do final da execução

Para ajudar na preparação do relatório disponibilizamos um modelo de exemplo.

