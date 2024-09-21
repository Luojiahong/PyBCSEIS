# PyBCSEIS
Python implementation of BCSEIS seismic cross-correlation calculation program

## Dependency
obspy
pandas
pyasdf

pygmt (for drawing, optional)

## output
Cross-correlation calculation was performed directly on asdf seismic data format and output the dt.cc format of hypoDD or tomoDD

## Result plotting
Draw dt.cc using pygmt

![avatar -w50](figures/CJP_P_200900004_200900007.png)

## References
Jiahong Luo, Wentao Ma, Chunzheng Li; Application of the Double-difference Relocation Method Combined with Waveform Cross-correlation on the Three Gorges Reservoir Seismicity, Earthquake Research in China, 2017, 31(3), 368-380.

Wen-Xuan Du, Clifford H. Thurber, Donna Eberhart-Phillips; Earthquake Relocation Using Cross-Correlation Time Delay Estimates Verified with the Bispectrum Method. Bulletin of the Seismological Society of America ; 94 (3): 856–866. doi: https://doi.org/10.1785/0120030084

Lion Krischer, James Smith, Wenjie Lei, Matthieu Lefebvre, Youyi Ruan, Elliott Sales de Andrade, Norbert Podhorszki, Ebru Bozdağ, Jeroen Tromp, An Adaptable Seismic Data Format, Geophysical Journal International, Volume 207, Issue 2, 1 November 2016, Pages 1003–1011, https://doi.org/10.1093/gji/ggw319

This is the Adaptable Seismic Data Format - if you are looking for the Advanced Scientific Data Format, go here: https://asdf.readthedocs.io/en/latest/

pyasdf:https://seismicdata.github.io/pyasdf/index.html
