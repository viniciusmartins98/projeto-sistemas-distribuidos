# Trabalho Sistemas Distribuídos
## Projeto - Integral paralela 
Escrever um programa para o cálculo da integral da função abaixo, usando a regra do Trapézio: 

> f(x) = SQRT(100² - x²)


Seu programa deve calcular a integral no intervalo [0,100], sendo que o mesmo deverá usar uma estrutura mestre-escravo (o inverso de cliente-servidor) usando sockets para a comunicação entre mestre e escravos. O programa mestre deve requisitar a cada escravo o cálculo de parte do intervalo, esperando que os mesmos retornem a parte deles, que serão acumuladas para o resultado final pelo mestre.  

Seu programa deverá ser executado para as seguintes condições:  
 
Número de escravos variando entre 1, 2, 4 e 10 escravos
Intervalo de discretivação sendo 0.0001, 0.00001 e 0.000001
 
Grupos de 2 ou 3 alunos (nem mais, nem menos) 
 
ENTREGA: 01/06, até 23h59, por e-mail para aleardo.manacero@unesp.br , sendo necessário fazer sua apresentação na semana seguinte (ou quando voltarmos).