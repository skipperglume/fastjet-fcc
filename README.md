# Overview:
A repository to keep progress on PU mitigation task for HL-LHC and FCC.

## Start fresh:
Snippet to refresh installation:
```bash
rm -rf fastjet-install
```

## Installing `fastjet`

### NOTE: 
`fastjet-3.5.1` requires `automake-1.17`
```bash
wget https://fastjet.fr/repo/fastjet-3.5.1.tar.gz
tar zxvf fastjet-3.5.1.tar.gz
cd fastjet-3.5.1/
```



### NOTE: `fastjet-3.5.0` requires `automake-1.17`
```bash
wget https://fastjet.fr/repo/fastjet-3.5.0.tar.gz
tar zxvf fastjet-3.5.0.tar.gz
cd fastjet-3.5.0/
```

### NOTE: `fastjet-3.4.3` requires `automake-1.17`
```bash
wget https://fastjet.fr/repo/fastjet-3.4.3.tar.gz
tar zxvf fastjet-3.4.3.tar.gz
cd fastjet-3.4.3/
```

### NOTE: `fastjet-3.4.1` requires `automake-1.16`
```bash
wget https://fastjet.fr/repo/fastjet-3.4.1.tar.gz
tar zxvf fastjet-3.4.1.tar.gz
cd fastjet-3.4.1/
```


### NOTE: `fastjet-3.3.4` requires `automake-1.16`enough
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

### NOTE: Didn't manage to install automake-1.17:
```bash
wget https://ftp.gnu.org/gnu/automake/automake-1.17.tar.xz
# tar zxvf automake-1.17.tar.xz
tar -xJf automake-1.17.tar.xz
# alias automake-1.17=/home/timoshyd/PU_FCC/automake-1.17/bin/automake-1.17
```

## `fastjet-contrib`:
```
https://fastjet.fr/contrib/
```

#### NOTE: `fjcontrib-1.101` requires at least `fastjet-3.4.1`
Getting `fastjet-contrib`
```bash
wget https://fastjet.fr/contrib/downloads/fjcontrib-1.101.tar.gz
tar zxvf fjcontrib-1.101.tar.gz
cd fjcontrib-1.101/
```

Installing `fastjet-contrib`
```bash
wget https://fastjet.fr/contrib/downloads/fjcontrib-1.056.tar.gz
tar zxvf fjcontrib-1.056.tar.gz
cd fjcontrib-1.056/
```

```bash
./configure --fastjet-config=`pwd`/../fastjet-install/bin/fastjet-config
make
make check         # optional
make install
```

[ConstituentSubtractor](https://phab.hepforge.org/source/fastjetsvn/browse/contrib/contribs/ConstituentSubtractor/tags/1.4.7/README?as=source&blame=off)

[software for evaluating the performance](https://github.com/PileupWorkshop/2014PileupWorkshop.git)

## Work plan: 
 - Simulate events
 - Write C++ code for combination of PUPPI and Constituent Subtraction
 - evaluate the performance of the new method.
 - Publication


## Compiling examples for `fastjets`
```bash
g++ short-example.cc -o short-example `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./short-example

g++ ./basic01.cc -o basic01 `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./basic01

g++ jetDef02.cc -o jetDef02 `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./jetDef02

g++ exercises/boostedTop13.cc -o boostedTop13 `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./boostedTop13 < data/boosted_top_event.dat
# 9: User Info exercise:
g++ exercises/userInfo09.cc -o userInfo09 `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins`
./userInfo09 < data/Pythia-dijet-ptmin100-lhc-pileup-1ev.dat


g++ fjcontrib-1.101/ConstituentSubtractor/example_event_wide.cc -o example_event_wide `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins` -lConstituentSubtractor

./example_event_wide < data/Pythia-Zp2jets-lhc-pileup-1ev.dat

```



Examples and docs taken from: 
 - https://www.lpthe.jussieu.fr/~cacciari/public/VM/fastjet-examples/
 - https://phystev.cnrs.fr/wiki/_media/2013:talks:fast_jet3_soyez_tutorial.pdf

Python repository
```bash
git clone --recursive https://github.com/scikit-hep/fastjet.git
```

Test folowing things:
 - fastjet_version_string

### NOTE: In CS.cc code two constant values are used: `3.1415` and `3.141593` . Why are lines all over the place?

