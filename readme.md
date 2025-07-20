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
.... (might take a while)
11382: λf.λx.(f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f x))))))))))))))))))))))))
done
$ # 4! is equal to 24, encoded in church numerals (f is applied to x 24 times)
```
