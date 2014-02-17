C++ app using Qt4:
* Discrete cosine transform (DCT) of image
* Discrete Fourier transform (DFT) of image
* Filter (convolution) images using DFT or DCT

Both DFT and DCT are calculated using FFT.

Packages:
```
sudo apt-get install qt4-qmake libqt4-dev build-essential
```

Compile:
```
qmake -project "QMAKE_CXXFLAGS += -std=c++0x"
qmake
make
```

Screenshot:
![Alt text](/images/screen.jpeg "Screenshot")
