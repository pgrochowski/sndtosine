/* main.c
 * Entry point for sndtosine.
 *
 * This program analyzes sound files and can filter and output results from an
 * FFT calculation. This allows one to see the frequency spectrum of the given
 * sound file. This will work with any sound file supported by libsndfile.
 *
 * Author: Patryk Grochowski
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sndfile.h>
#include <fftw3.h>
#include <math.h>
#include "samples.h"
#include "filters.h"
#include "tofile.h"

int main (int argc, char *argv[])
{
	if (argc != 5) {
		printf("usage: %s <operation> <gate level> <input wav> <output csv>\n\n", argv[0]);
		printf("Operation should be one of the following:\n");
		printf("\n\tall\n\tpeaks\n\n");
		printf("Gate level should be a number between 0 and 1 ");
		printf("representing a percentage.\nAny frequencies with an ");
		printf("amplitude under this percentage of the maximum ");
		printf("amplitude will\nbe dropped to zero.\n");
		return 1;
	}

	/* get args */
	char *operation = argv[1];

	errno = 0;
	double level = strtod(argv[2], NULL);
	if (errno != 0) {
		printf("Please use a number between 0 and 1 for gate level.\n");
		printf("Decimals are okay.\n");
		return 1;
	}

	char *infile = argv[3];
	char *outfile = argv[4];

	/* init some useful stuff */
	SF_INFO *wfinfo = malloc(sizeof(SF_INFO));
	int j = 0;
	int ret = 1;
	long long i = 0;

	if (wfinfo == NULL) {
		printf("Out of memory.\n");
		return 2;
	}

	wfinfo->format = 0;
	SNDFILE *wf = sf_open(infile, SFM_READ, wfinfo);

	if (wf == NULL) {
		printf("Could not open file.\n");
		free(wfinfo);
		return 3;
	}

	if (wfinfo->channels > 10) {
		printf("Program currently supports up to 10 channels and this file has %d. Exiting...\n", wfinfo->channels);
		free(wfinfo);
		sf_close(wf);
		return 4;
	}

	printf("Channels: %d\n", wfinfo->channels);
	printf("Sample Rate: %d\n", wfinfo->samplerate);
	printf("Format Code: %x\n", wfinfo->format);
	printf("Frame Count: %d\n", wfinfo->frames);

	struct sample_cont *samples = get_sample_cont(wfinfo);

	if (samples == NULL) {
		printf("Out of memory.\n");
		free(wfinfo);
		sf_close(wf);
		return 2;
	}

	double *value = malloc(sizeof(double) * samples->num_channels);

	if (value == NULL) {
		printf("Out of memory.\n");
		free(wfinfo);
		sf_close(wf);
		free_sample_cont(samples);
		return 2;
	}

	printf("Generating FFTW Plan\n");
	fftw_plan plan = fftw_plan_dft_r2c_1d(samples->total_samples,
					      samples->channels[0],
					      samples->fft_out,
					      0);

	printf("Initializing values\n");
	for (i = 0; i < samples->total_samples; i++) {
		ret = sf_read_double(wf, value, 1);
		if (ret == 0)
			break;
		for (j = 0; j < samples->num_channels; j++) {
			samples->channels[j][i] = value[j];
		}
	}

	printf("Calculating FFT\n");
	fftw_execute(plan);

	printf("Running filters\n");
	/* calculate frequencies and amplitudes */
	fft_out_to_freq_ampl(samples);

	/* Cut anything below user provided percentage of the max amplitude */
	noise_gate_on_ampl(samples, level);

	if (!strncmp(operation, "peaks", 5)) {
		/* find all peaks in file */
		get_ampl_peaks(samples);
		dump_peaks_to_file(outfile, samples);
	} else {
		dump_fft_results_to_file(outfile, samples);
	}

	printf("Cleaning up...\n");
	fftw_destroy_plan(plan);
	sf_close(wf);
	free(wfinfo);
	free(value);
	free_sample_cont(samples);

	printf("Done\n");
	return 0;
}
