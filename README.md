ga.shrinker
========

Source code, supplemental data, and obtained results for "Shrinking Counterexamples in Property-Based Testing with Genetic Algorithms"

----------


Execution Environment in Study
------------

* CPU: Intel I7-6700 3.4GHz
* RAM: 32GB
* OS: Windows 10 64-bit
* Written in C++
* Compiled with TDM-GCC 4.9.2 64-bit release (-std=c++11)


Showcase: Mergesort
------------

#### Directory content
* Mergesort/
	* MS_ce.txt<br/>Simple list of the counterexample for merge sort
	* counterexample-[SerialNo]-[Length].txt<br/>The 50 initial counterexamples found by QuickChick and used in paper
	* auxfunction.h & mergesort.cpp<br/>Source code of shrinkers: RS, RW, GA
		1. Change line 27<br/>```ifstream ifile("counterexample-13-508.txt",ios::in);```<br/>to shrink the specified counterexample
		2. For each counterexample, 30 trials will be conducted, and four output files will be created or appended if exist:
			* MS_ga.txt, MS_rs.txt, MS_rw.txt<br/>Results of GA, RS, RW for merge sort
				* `*' indicates the beginning of a 30-trial execution
				* The first column is the minimal size of a trail
				* The second column is the running time in milliseconds
			* MSstat.txt<br/>One line statistics for a 30-trail execution
				* A sample outcome<br/>```130.733 44.5136 22.4667 1.0873 57.7667 27.06 21.0667 1.65194 2.13333 0.718022 13.1333 0.763035```
					* Three 4-tuple statistics for RS, RW, and GA shrinkers, respectively
					* min size AVG, min size SD, running time AVG, running time SD
		3. You may uncomment all the file input/output code in the three shrinkers. All the counterexamples generated during the shrinking process will be printed out. It may be helpful for observing how shrinkers work, but file input/output is extremely time consuming.


Showcase: Red-Black Tree Insertion
------------

#### Directory content
* Red-Black Tree Insertion/
	* RBT_ce.txt<br/>Simple list of the counterexample for red-black tree insertion
	* counterexample-[SerialNo]-[TreeSize]-[KeyValue].txt<br/>The 50 initial counterexamples found by QuickChick and used in paper
	* RBTree.h & main.cpp<br/>Source code of shrinkers: RS, RW, GA
		1. Change line 28<br/>```ifstream ifile("counterexample-24-949-1277.txt",ios::in);```<br/>to shrink the specified counterexample
		2. For each counterexample, 30 trials will be conducted, and four output files will be created or appended if exist:
			* RBT_ga.txt, RBT_rs.txt, RBT_rw.txt<br/>Results of GA, RS, RW for red-black tree insertion
				* `*' indicates the beginning of a 30-trial execution
				* The first column is the minimal size of a trail
				* The second column is the running time in milliseconds
			* RBTstat.txt<br/>One line statistics for a 30-trail execution
				* A sample outcome<br/>```255.5 65.5849 10377.1 63.0555 217.1 52.7733 12003.1 66.9028 15.5 13.4406 1102.4 124.694```
					* Three 4-tuple statistics for RS, RW, and GA shrinkers, respectively
					* min size AVG, min size SD, running time AVG, running time SD
		3. You may uncomment all the file input/output code in the three shrinkers. All the counterexamples generated during the shrinking process will be printed out. It may be helpful for observing how shrinkers work, but file input/output is extremely time consuming.


Mann-Whitney U Test and Statistics
------------

#### Directory content
* Mann-Whitney U Test/
	* *.xlsx<br/>In the file names:
		* "S-" stands for "size" (of the resultant counterexamples)
		* "T-" stands for "running time"
		* Showcase_Method-vs-Showcase_Method<br/>
		E.g.,
			* "MS_rw-vs-MS_ga" means Mann-Whitney U test on RW and GA for merge sort<br/>
			* "RBT_rs-vs-RBT_rw" means Mann-Whitney U test on RS and RW for RBT insertion
