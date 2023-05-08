        lw      0       1       x       check lw working
        sw      0       1       32      check sw working
        lw      0       6       32
        beq     0       0       main    check beq working
func    add     0       1       2       check function / add working
        jalr    3       4               check jalr working
main    nor     1       1       7       check nor working
        lw      0       2       fcAddr  
        jalr    2       3               
        add     0       1       2
        add     1       2       3
        add     2       3       4
        add     3       4       5
        add     4       5       6       check successive addition and all register converting
        noop                            check noop
done    halt
fcAddr  .fill   func
x       .fill   12345
mnAddr  .fill   main
