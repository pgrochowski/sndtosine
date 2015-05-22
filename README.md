# sndtosine
This program analyzes sound files and can filter and output results from an FFT
calculation. This allows one to see the frequency spectrum of the given sound
file. This will work with any sound file supported by libsndfile.
## Usage
./sndtosine operation level input output

* Operation - This should be the string "all" or "peaks". Determines whether the
output will be all frequency/amplitude pairs or just the peak values, where peak
is defined as all points where the preceding and next value are lower than the
current value.

* Level - A decimal value between 0 and 1. Used to calculate a value by
multiplying against the largest amplitude found. If other amplitudes are below
this value, they are considered noise and changed to zero.

* Input - Path to the sound file that will be analyzed. Can be any format that
libsndfile supports.

* Output - Path for the csv output file. First column is frequency, second is
amplitude.

## License
Licensed under GPLv2.
