.segment "kernel"

.proc Main
loop:       JSR MagicRegisterTest
            JMP loop
.endproc

.proc MagicRegisterTest
            LDX #$7         ;How many times do we want to loop?
loop:       LDY #$68        ;h
            STY $F420
            LDY #$65        ;e
            STY $F420
            LDY #$6C        ;l
            STY $F420
            LDY #$6C        ;l
            STY $F420
            LDY #$6F        ;o
            STY $F420
            LDY #$20        ;space
            STY $F420
            LDY #$77        ;w
            STY $F420
            LDY #$6F        ;o
            STY $F420
            LDY #$72        ;r
            STY $F420
            LDY #$6C        ;l
            STY $F420
            LDY #$64        ;d
            STY $F420
            LDY #$0A        ;\n
            STY $F420
            DEX             ;Dec counter
            BNE loop        ;Loop until counter is 0

            LDY #$FF        ;Clear display
            STY $F420
            RTS
.endproc
