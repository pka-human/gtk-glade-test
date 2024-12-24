![GitHub Release](https://img.shields.io/github/v/release/pka-human/gtk-glade-test?style=plastic)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/pka-human/gtk-glade-test/total?style=plastic)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/pka-human/gtk-glade-test?style=plastic)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/pka-human/gtk-glade-test/ci.yml?style=plastic)

# gtk-glade-test

![screenshot](screenshot.png)

Build with CLANG (***recommended***):
```bash
export CC=clang && export CXX=clang++ && cmake -S . -B build && cmake --build build
```

Build with GCC:
```bash
export CC=gcc && export CXX=g++ && cmake -S . -B build && cmake --build build
```

Clean build:
```bash
rm -r build bin
```

Run:
```bash
./bin/gtk-glade-test
```
