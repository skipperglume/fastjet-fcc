
`fastjet-3.5.1` requires `automake-1.17`
```bash
wget https://fastjet.fr/repo/fastjet-3.5.1.tar.gz
tar zxvf fastjet-3.5.1.tar.gz
cd fastjet-3.5.1/
```



`fastjet-3.5.0` requires `automake-1.17`
```bash
wget https://fastjet.fr/repo/fastjet-3.5.0.tar.gz
tar zxvf fastjet-3.5.0.tar.gz
cd fastjet-3.5.0/
```

<!-- `fastjet-3.4.3` requires `automake-1.17` -->
```bash
wget https://fastjet.fr/repo/fastjet-3.4.3.tar.gz
tar zxvf fastjet-3.4.3.tar.gz
cd fastjet-3.4.3/
```
Compile fastjet `automake-1.17`:


```bash
wget https://fastjet.fr/repo/fastjet-3.3.4.tar.gz
tar zxvf fastjet-3.3.4.tar.gz
cd fastjet-3.3.4/
```




Compile `fastjet`:
```bash
./configure --prefix=`pwd`/../fastjet-install
make
make check
make install
cd ..
```

```bash
wget https://ftp.gnu.org/gnu/automake/automake-1.17.tar.xz
# tar zxvf automake-1.17.tar.xz
tar -xJf automake-1.17.tar.xz
# alias automake-1.17=/home/timoshyd/PU_FCC/automake-1.17/bin/automake-1.17
```

fastjet-contrib:
```
https://fastjet.fr/contrib/
```


ConstituentSubtractor
```
https://phab.hepforge.org/source/fastjetsvn/browse/contrib/contribs/ConstituentSubtractor/tags/1.4.7/README?as=source&blame=off
```

software for evaluating the performance:
```
https://github.com/PileupWorkshop/2014PileupWorkshop.git
```

Work plan: 
 - Write C++ code for combination of PUPPI and Constituent Subtraction
 - evaluate the performance of the new method.
 - Publication




g++ short-example.cc -o short-example `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./short-example


https://phystev.cnrs.fr/wiki/_media/2013:talks:fast_jet3_soyez_tutorial.pdf