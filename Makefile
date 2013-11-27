CXX = g++
CXXFLAGS = -g \
		   -Wall

progress_bar_test: progress_bar.cc progress_bar_test.cc progress_bar.h
	$(CXX) $(CXXFLAGS) -o progress_bar_test \
		progress_bar.cc \
		progress_bar_test.cc

clean:
	rm progress_bar_test
