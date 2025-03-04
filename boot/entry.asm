
; entry.asm
;
; Antoine LANDRIEUX
; BORIUM <https://github.com/AntoineLandrieux/BORIUM>
;
; MIT License

section .text
    [bits 32]
    [extern start]
    call start
