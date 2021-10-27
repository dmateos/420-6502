.segment "kernel"

.PROC Main
loop:   JSR Test
        JMP loop
.ENDPROC

.PROC Test
        LDX #$FF        ;How many times do we want to loop?
loop:   NOP
        DEX				;Dec counter
        BNE loop	    ;Loop until counter is 0
        RTS
.ENDPROC
