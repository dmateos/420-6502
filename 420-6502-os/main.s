.segment "kernel"

.PROC Main
loop:   
        JSR Test
        JMP loop
.ENDPROC

.PROC Test
        LDX #$10        ;How many times do we want to loop?
loop:   
        LDY #$69
        STY $F420
        DEX             ;Dec counter
        BNE loop        ;Loop until counter is 0
        RTS
.ENDPROC
