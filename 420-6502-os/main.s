.segment "kernel"

.proc Main
loop:       JSR Header
            JSR HelloWorldTest
            
            LDY #$FF                ;Clear display
            STY $F420

            JMP loop
.endproc

.proc Header
            LDY #$6D                ;m
            STY $F420
            LDY #$61                ;a
            STY $F420
            LDY #$74                ;t
            STY $F420
            LDY #$65                ;e
            STY $F420
            LDY #$4F                ;O
            STY $F420
            LDY #$53                ;S
            STY $F420
            LDY #$20                ;space
            STY $F420
            LDY #$76                ;v
            STY $F420
            LDY #$30                ;0
            STY $F420
            LDY #$2E                ;.
            STY $F420
            LDY #$31                ;1
            STY $F420
            LDY #$0A                ;\n
            STY $F420
            RTS
.endproc

.proc HelloWorldTest
            LDX #$6                 ;How many times do we want to loop?
loop:       LDY #$68                ;h
            STY $F420
            LDY #$65                ;e
            STY $F420
            LDY #$6C                ;l
            STY $F420
            LDY #$6C                ;l
            STY $F420
            LDY #$6F                ;o
            STY $F420
            LDY #$20                ;space
            STY $F420
            LDY #$77                ;w
            STY $F420
            LDY #$6F                ;o
            STY $F420
            LDY #$72                ;r
            STY $F420
            LDY #$6C                ;l
            STY $F420
            LDY #$64                ;d
            STY $F420
            LDY #$0A                ;\n
            STY $F420
            DEX                     ;Dec counter
            BNE loop                ;Loop until counter is 0
            RTS
.endproc
