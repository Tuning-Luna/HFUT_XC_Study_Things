DATA SEGMENT
  PROMPT_X   DB 'Input x : $'
  PROMPT_Y   DB 'Input y : $'
  RESULT_MSG DB 'Result (z): $'
  BUFFER     DB 8 DUP('$')
DATA ENDS

CODE SEGMENT
                  ASSUME CS:CODE, DS:DATA

  START:          
                  MOV    AX, DATA
                  MOV    DS, AX

                  LEA    DX, PROMPT_X
                  MOV    AH, 09H
                  INT    21H

                  CALL   READ_SIGNED_INT
                  PUSH   AX

                  LEA    DX, PROMPT_Y
                  MOV    AH, 09H
                  INT    21H

                  CALL   READ_SIGNED_INT
                  PUSH   AX

                  CALL   COMPUTE_Z

                  LEA    DX, RESULT_MSG
                  MOV    AH, 09H
                  INT    21H

                  MOV    BX, AX
                  LEA    DI, BUFFER
                  CALL   DISPLAY_INT

                  MOV    AH, 4CH
                  INT    21H

COMPUTE_Z PROC
                  POP    CX
                  POP    DX

                  CMP    CX, 0
                  JZ     DIV_BY_ZERO

                  MOV    AX, DX
                  CWD
                  IDIV   CX

                  PUSH   AX

                  MOV    AX, DX
                  MOV    BX, 24
                  IMUL   BX
                  NEG    AX

                  POP    BX
                  ADD    AX, BX

                  ADD    AX, 2024

                  RET
  DIV_BY_ZERO:    
                  MOV    AX, -1
                  RET
COMPUTE_Z ENDP

READ_SIGNED_INT PROC
                  XOR    AX, AX
                  MOV    AH, 01H
                  INT    21H

                  CMP    AL, '-'
                  JNE    READ_DIGIT
                  MOV    BL, -1
                  INT    21H

  READ_DIGIT:     
                  SUB    AL, '0'
                  MOV    BL, AL

                  INT    21H
                  SUB    AL, '0'
                  MOV    BH, AL

                  MOV    AL, BL
                  CBW
                  MOV    AH, BH
                  CMP    BL, -1
                  JNE    RETURN
                  NEG    AX
  RETURN:         
                  RET
READ_SIGNED_INT ENDP

DISPLAY_INT PROC
                  MOV    AX, BX
                  CALL   INT_TO_ASCII
                  LEA    DX, BUFFER
                  MOV    AH, 09H
                  INT    21H
                  RET
DISPLAY_INT ENDP

INT_TO_ASCII PROC
                  XOR    CX, CX
                  LEA    DI, BUFFER

                  TEST   AX, AX
                  JNS    CONVERT
                  NEG    AX
                  MOV    BYTE PTR [DI], '-'
                  INC    DI

  CONVERT:        
                  MOV    BX, 10
  DIV_LOOP:       
                  XOR    DX, DX
                  DIV    BX
                  ADD    DL, '0'
                  DEC    DI
                  MOV    [DI], DL
                  INC    CX
                  TEST   AX, AX
                  JNZ    DIV_LOOP

                  ADD    DI, CX
                  MOV    BYTE PTR [DI], '$'
                  RET
INT_TO_ASCII ENDP

CODE ENDS
END START


