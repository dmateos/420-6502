.segment "os"

.PROC Main
JMP $FFFF
halt: JMP halt 
.ENDPROC
