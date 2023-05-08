        lw      0       1       iter    load iter count
        lw      0       2       neg
reload  lw      0       3       x       consecutive load test
        lw      0       4       y
        lw      0       5       z
        lw      0       6       w
        add     1       2       1
        beq     0       1       done
        beq     0       0       reload
done    halt
x       .fill   0
y       .fill   1
z       .fill   2
w       .fill   3
iter    .fill   100
reAddr  .fill   reload
dnAddr  .fill   done
neg     .fill   -1
