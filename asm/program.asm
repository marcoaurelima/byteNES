; Exemplo de código assembler 6502 com JMP e INX, iniciando em $0600

    ORG $0600  ; Novo endereço de início do programa

    LDX #$22    ; Inicializa o registrador X com zero
    STX $01    ; Armazena zero em $01 (pode ser qualquer endereço de memória)

Loop:
    INX        ; Incrementa o valor no registrador X
    CPX #05    ; Compara o valor em X com 5
    BEQ End    ; Se X for igual a 5, pula para o rótulo End

    JMP Loop   ; Pula de volta para o rótulo Loop

End:
    ; Código que será executado após o loop

    BRK        ; Instrução de interrupção, encerrando o programa

    ; Você pode adicionar mais código ou dados após o BRK se necessário
