# LC - lambda calculus interpreter built using yacc

Install lc:
```sh
$ git clone https://github.com/omnomctr/lc.git
$ cd lc
$ make
$ sudo cp lc /usr/bin/
$ lc
(\x.x) a;
0: (λx.x) a
1: a
done
$
```

Run the example program `fact.lc`:
```sh
$ chmod +x fact.lc
$ ./fact.lc
.... (will output each step, this might take a while)
10189: (λf.λx.f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f x))))))))))))))))))))))))
done
$ # 4! is equal to 24, encoded in church numerals (f is applied to x 24 times)
```

`lc` also has the argument `-hide-steps` which will make evaluation much quicker:
```sh
$ time ./fact.lc -hide-steps
(λf.λx.f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f (f x))))))))))))))))))))))))
./fact.lc -hide-steps  0.29s user 0.00s system 99% cpu 0.288 total
```
