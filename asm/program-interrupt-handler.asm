
LDA #<InterruptHandler  ; Carrega o byte menos significativo do endereço
STA $FFFE              ; Armazena no byte menos significativo do vetor de interrupção
LDA #>InterruptHandler  ; Carrega o byte mais significativo do endereço
STA $FFFF              ; Armazena no byte mais significativo do vetor de interrupção

; Código principal
    LDA #$01  ; Carrega o valor 1 no registrador A
    BRK       ; Gera uma interrupção de software

    ; O código a seguir será ignorado após o BRK, pois o BRK gera uma interrupção

    LDA #$02  ; Este código não será executado
    STA $33 ; Este código não será executado

; Tratador de interrupção
InterruptHandler:
    PHA       ; Salva o registrador A na pilha
    LDA #$FF  ; Carrega um valor qualquer (por exemplo, $FF) no registrador A
    STA $33 ; Armazena o valor na memória (pode ser em outro endereço)
    PLA       ; Restaura o registrador A a partir da pilha
    RTI       ; Retorna da interrupção

; Fim do programa
