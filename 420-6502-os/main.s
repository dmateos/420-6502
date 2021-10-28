.segment "kernel"

.PROC Main
loop:   
        JSR Test
        JMP loop
.ENDPROC

.PROC Test
        LDX #$10        ;How many times do we want to loop?
loop:   
        LDX #$69
        STX $2004
        DEX             ;Dec counter
        BNE loop        ;Loop until counter is 0
        RTS
.ENDPROC
