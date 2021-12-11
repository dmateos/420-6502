.FEATURE STRING_ESCAPES
.segment "kernel"

.proc Main
loop:       JSR PrintHeader
            JSR PrintHelloWorldTest
            JSR PrintHelloWorldTest
            JSR PrintHelloWorldTest
            JSR PrintCountTest
            JSR PrintCountTest
            JSR PrintCountTest
            JSR ClearDisplay
            JMP loop
.endproc

.proc PrintHeader
            LDX #0              
loop:       CPX #HeaderMsgLen       ;Loop untill we reach end of msg
            BEQ done                ;
            LDY HeaderMsg,X         ;Load message offset ito Y
            STY $F420               ;Print
            INX                     ;
            JMP loop                ;Keep going
done:
            RTS
.endproc

.proc PrintHelloWorldTest
            LDX #0              
loop:       CPX #HelloMsgLen        ;Loop untill we reach end of msg
            BEQ done                ;
            LDY HelloMsg,X          ;Load message offset ito Y
            STY $F420               ;Print
            INX                     ;
            JMP loop                ;Keep going
done:
            RTS
.endproc

.proc PrintCountTest
            LDX #$A                 ;Loop 10 times
            LDY #$30                ;Start at 0

loop:       STY $F420               ;Print number 
            INY                     ;Inc number
            DEX                     ;Dec loop counter
            BNE loop                ;Keep looping

            LDY #$0A                ;\n
            STY $F420               ;Print newline
            RTS
.endproc

.proc ClearDisplay
            LDY #$FF
            STY $F420
            RTS
.endproc

HeaderMsg:  .byte "mateOS v0.1\n"
HeaderMsgLen = 12

HelloMsg:   .byte "Hello World!\n"
HelloMsgLen = 13