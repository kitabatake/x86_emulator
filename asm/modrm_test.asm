# assuming results: eax is 0x00000001
BITS 32
start:
    MOV ESI, 0x0200
    MOV dword [ESI], 0x01
    MOV dword EAX, [ESI]
    jmp short start    
