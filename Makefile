FREQ_HZ = 10
GROUP_SIZE_BINS = 100000
PACKETS = 10
VERBOSE = 1
py_histograms = ..\..\..\..\data_testing\histograms.py

.PHONY: Triggering
Triggering: bnc525.ps1
	powershell -Command "Start-Process -NoNewWindow bnc525.ps1 10000; Start-Process -NoNewWindow get_data.exe $(FREQ_HZ) $(GROUP_SIZE_BINS) $(PACKETS) 0 $(VERBOSE)"


.PHONY: Histogram
Histogram: Acquire_data
	python py_histograms

.PHONY: Acquire_data

Acquire_data: get_data.exe Triggering
	get_data.exe $(FREQ_HZ) $(GROUP_SIZE_BINS) $(PACKETS) 0 $(VERBOSE)






