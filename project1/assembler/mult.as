        lw       0       5       mcand   set reg5 as 32766
        lw       0       6       mplier  set reg6 as 12328
        lw       0       7       one     set reg7 as 1
        lw       0       4       iter    reg4 = 32
        sw       0       4       33      save it into memory
        add      0       0       1       set reg1 as 0 
        beq      0       0       main    jump to main      
and     nor      6       6       3       reg3 = not reg6
        nor      7       7       4       reg4 = not reg7
        nor      3       4       4       reg4 = reg3 and reg4
        jalr     2       3               jump to original call point
main    lw       0       3       andAddr load 'and' function location to reg2
        jalr     3       2               calculate 'and'
        beq      0       4       jmp
        add      1       5       1       reg1 += reg5
jmp     add      5       5       5       reg5 << 1
        add      7       7       7       reg7 << 1
        lw       0       4       33      load iter value.
        lw       0       3       neg     load minus 1.
        add      4       3       4       iter--
        beq      0       4       done    go to end.
        sw       0       4       33      save iter on memory.
        beq      0       0       main 
done    halt 
mcand   .fill    32766
mplier  .fill    12328
andAddr .fill    and
mainAdr .fill    main   
jmpAdrr .fill    jmp
one     .fill    1
neg     .fill    -1
iter    .fill    16
