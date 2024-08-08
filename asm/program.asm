    .ORG $0600
Start:
    LDA #$00         ; Carrega o valor 0 no acumulador (apenas para consumir ciclos)
    RTS              ; Retorna, o que faz com que o programa volte para Start
