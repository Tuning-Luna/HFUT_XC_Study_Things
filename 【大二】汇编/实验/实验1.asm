; *****************实验1.1***************
; .386                          ; 32位寻址模式
; DATA SEGMENT USE16                     ; 数据段
;   MESG DB '0208 10 11 11 11', 0AH,'$'  ; 定义字符串
; DATA ENDS                              ; 数据段结束

; CODE SEGMENT USE16              ; 段代码段
;        ASSUME CS:CODE, DS:DATA  ; 段寻址
;   BEG:                          ; 程序入口
;        MOV    AX, DATA          ; 选择数据段
;        MOV    DS, AX            ; 加载数据段
;        MOV    AH, 9             ; 输出字符串功能号
;        MOV    DX, OFFSET MESG   ; 输出字符串地址
;        INT    21H               ; 调用DOS中断输出字Q符串
;        MOV    AH, 4CH           ; 结束程序
;        INT    21H               ; 调用DOS中断结束程序
; CODE ENDS                       ; 段代码段结束
; END BEG ; 程序结束

; DATA SEGMENT
;   X    DW 5
;   Y    DW 10
;   Z    DW 50
;   V    DW 1000
;   SUM1 DW ?
;   SUM2 DW ?
; DATA ENDS

; *****************实验1.2***************
; CODE SEGMENT
;         ASSUME CS:CODE, DS:DATA

;   START:
;         MOV    AX, DATA          ; 初始化数据段
;         MOV    DS, AX

;         MOV    AX, X             ; AX = X
;         IMUL   WORD PTR Y        ; AX = X * Y

;         ADD    AX, Z             ; AX = X * Y + Z

;         SUB    AX, 720           ; AX = X * Y + Z - 720

;         MOV    BX, AX            ; 将 (X * Y + Z - 720) 保存到 BX
;         MOV    AX, V             ; AX = V
;         SUB    AX, BX            ; AX = V - (X * Y + Z - 720)

;         MOV    BX, X             ; BX = X
;         IDIV   BX                ; AX = 商 (整数部分), DX = 余数

;         MOV    SUM1, AX          ; 存储商到 SUM1
;         MOV    SUM2, DX          ; 存储余数到 SUM2

;         MOV    AH, 4CH           ; 返回到 DOS
;         INT    21H
; CODE ENDS

; END START

; *****************实验1.3*************** 
; DATA SEGMENT
;   MSG_TABLE DW OFFSET MSG0
;             DW OFFSET MSG1
;             DW OFFSET MSG2
;             DW OFFSET MSG3
;             DW OFFSET MSG4
;             DW OFFSET MSG5
;             DW OFFSET MSG6
;             DW OFFSET MSG7

;   ; 提示信息内容
;   MSG0      DB 'The 0 Bit is 0', 0DH, 0AH, '$'
;   MSG1      DB 'The 1 Bit is 0', 0DH, 0AH, '$'
;   MSG2      DB 'The 2 Bit is 0', 0DH, 0AH, '$'
;   MSG3      DB 'The 3 Bit is 0', 0DH, 0AH, '$'
;   MSG4      DB 'The 4 Bit is 0', 0DH, 0AH, '$'
;   MSG5      DB 'The 5 Bit is 0', 0DH, 0AH, '$'
;   MSG6      DB 'The 6 Bit is 0', 0DH, 0AH, '$'
;   MSG7      DB 'The 7 Bit is 0', 0DH, 0AH, '$'
; DATA ENDS

; CODE SEGMENT
;              ASSUME CS:CODE, DS:DATA

;   START:     
;              MOV    AX, DATA              ; 初始化数据段
;              MOV    DS, AX

;              MOV    BL, 11111101B

;              MOV    CL, 0
;   FIND_ZERO: 
;              TEST   BL, 80H
;              JZ     OUTPUT_MSG
;              SHL    BL, 1
;              INC    CL
;              CMP    CL, 8
;              JNZ    FIND_ZERO


;   OUTPUT_MSG:
;              MOV    BX, OFFSET MSG_TABLE
;              MOV    AL, CL
;              MOV    AH, 0
;              MOV    SI, AX
;              SHL    SI, 1
;              ADD    BX, SI
;              MOV    AX, [BX]

;              MOV    DX, AX
;              MOV    AH, 09H
;              INT    21H

;              MOV    AH, 4CH
;              INT    21H
; CODE ENDS

; END START


; ****************实验1.4*************** 
DATA SEGMENT
  Score         DB 80, 75, 90, 85, 70, 95, 88, 76, 66, 99
                DB 100 DUP(0)
          
  PROMPT        DB 'Input Score(0-99): $'
  FOUND_MSG     DB 'Y$', 0DH, 0AH, '$'
  NOT_FOUND_MSG DB 'N$', 0DH, 0AH, '$'
DATA ENDS

CODE SEGMENT
              ASSUME CS:CODE, DS:DATA

  START:      
              MOV    AX, DATA
              MOV    DS, AX
              LEA    DX, PROMPT
              MOV    AH, 09H
              INT    21H

              MOV    AH, 01H
              INT    21H
              SUB    AL, '0'
              MOV    BL, AL
              INT    21H
              SUB    AL, '0'
              MOV    BH, AL

  
              MOV    AL, BL
              MUL    BYTE PTR [TEN]
              ADD    AL, BH

              MOV    CX, 100
              LEA    SI, Score
  SEARCH_LOOP:
              MOV    BL, [SI]
              CMP    BL, AL
              JE     FOUND
              INC    SI
              LOOP   SEARCH_LOOP

  NOT_FOUND:  
              LEA    DX, NOT_FOUND_MSG
              MOV    AH, 09H
              INT    21H
              JMP    END_PROGRAM

  FOUND:      
              LEA    DX, FOUND_MSG
              MOV    AH, 09H
              INT    21H

  END_PROGRAM:
              MOV    AH, 4CH
              INT    21H

  TEN         DB     10
CODE ENDS

END START
