.segment "kernel"

.PROC Main
loop: JSR NopTest
			JMP loop
.ENDPROC

.PROC NopTest
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			RTS
.ENDPROC
