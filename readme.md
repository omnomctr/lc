# LC - lambda calculus interpreter built using yacc

Install lc:
```sh
$ make
... (will download std_ds.h)
$ rlwrap ./lc # (if you want readline)
```

Run the example program `fact.lc`:
```sh
$ ./lc <fact.lc
.... (will output each step, this might take a while)
11382: λf.λx.(f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f x))))))))))))))))))))))))
done
$ # 4! is equal to 24, encoded in church numerals (f is applied to x 24 times)
```

`lc` also has the argument `-hide-steps` which will make evaluation much quicker:
```sh
$ time ./lc -hide-steps <fact.lc
λf.λx.(f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f x))))))))))))))))))))))))
done
./lc -hide-steps -hide-steps < fact.lc  0.30s user 0.00s system 99% cpu 0.305 total
```
