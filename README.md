
<h1> 
  &nbsp distance_between_polygons :straight_ruler:
  
</h1>

<br>

<p>
<img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/Shangin-Leonid/distance_between_polygons?color=green&label=Last%20commit&logo=Github&logoColor=white&style=plastic" height=25> &nbsp &nbsp &nbsp
<img alt="GitHub watchers" src="https://img.shields.io/github/watchers/Shangin-Leonid/distance_between_polygons?color=green&label=Views&logo=Github&logoColor=white&style=plastic" height=25> &nbsp &nbsp &nbsp
</p>



<h2> 
  :information_source: &nbsp Common info
</h2>
 
  The programm can read 2 plane polygons from input ".txt" file, calculate distance between them and put the result to output ".txt" file. <br>
  **How to start using? See below!** <br>
  The project also includes the system of <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests">auto tests</a>.



<br> <br>



<h2> 
  :bulb: &nbsp How to use?
</h2>

  Each ***makefile*** support ``` make help ``` , which will show you extra information about its commands and possibilities. 

<h3> 
  :chart_with_upwards_trend: &nbsp Main programm
</h3>

  Stay in current main directory.
  * ``` make build ``` - to build the programm;
  * ``` make run ``` - to run the programm with **default** input and output files;
  * ``` make run INPUT_FILE=... OUTPUT_FILE=... ``` - to run the programm with **your** input and output files;
  * ``` make print-INPUT_FILE ``` or  ``` make print-OUTPUT_FILE ``` - to see default input or output file names;
  * ``` make total_clean ``` - the most powerful clean rule; \
  See more with ``` make help ``` .

<h3> 
  :balance_scale: &nbsp Tests
</h3>

  Move to <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests">auto_tests</a> directory.
  * ``` make build ``` - to build everything;
  * ``` make do_test ``` - to run tests with **default** report file;
  * ``` make total_clean ``` - the most powerful clean rule; \
  See more with ``` make help ``` .

<h3> 
  :test_tube: &nbsp Analisis and code coverage
</h3>

  Move to <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests">auto_tests</a> directory. \
  Add ``` REDIRECT=YES ``` after any command to redirect analysis results to the file.
  * ``` make cppcheck_a ```     - static analysis by ***cppcheck***;
  * ``` make cpplint_a ```      - static analysis by ***cpplint***;
  * ``` make valgrind_a ```     - memory leak, runtime analysis by ***valgrind***;
  * ``` make clang_tidy_a ```   - by ***clang-tidy***;
  * ``` make coverage_a ```     - test coverage by ***gcc --coverage***, ***gcov***, ***lcov***;
  * ``` make clean_coverage ``` - clean everything that was created by 'coverage_a'; \
  See more with ``` make help ``` .

<h3> 
  :pencil2: &nbsp Input format and requirements
</h3>
  
  A polygon must be concluded into angle brackets. A number of vertices must be first. A vertex must have the following format: ``` {x ; y} ``` . Any amount of spaces between symbols is possible. \
  Examples of correct input:
  ```
    < 4 {-1.0 ; -1.0} {-1.0 ; 1.0} {1.0 ; 1.0} {1.0 ; -1.0} >
    < 3 {0;0} { 0 ; 1 } {   1    ;    3}            >
  ```
  Be careful with the order of vertices. Polygon's sides will be build in the same order. It is really important, because self-intersections are forbidden. Or a polygon will be accepted as incorrect.



<br> <br>



<h2> 
  :heavy_check_mark: &nbsp Standarts, technologies and software
</h2>
<p>
  <img alt="C"    src="https://img.shields.io/static/v1?label=&message=Language&style=flat&logo=C&color=dc143c" height=30> &nbsp &nbsp
  <img alt="Make" src="https://img.shields.io/static/v1?label=&message=Makefile&style=flat&logo=CMake&color=FE7A16" height=30> &nbsp &nbsp
  <img alt="GCC" src="https://img.shields.io/static/v1?label=&message=GCC&style=flat&logo=GNU&color=2B822F" height=30> &nbsp &nbsp
  <img alt="Linux" src="https://img.shields.io/static/v1?label=&message=Linux&style=flat&logo=linux&color=blue" height=30> &nbsp &nbsp
</p>

  All makefiles are requires ***Linux OS*** and ***gcc*** compiler. Of course, you can change files to use with another OS or compiler. I have added some variables especially for the this. <br>
  The <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests">test system</a> is able to make static and dynamic analizes.
  This goals requires the following tools: ***cpplint***, ***cppcheck***, ***clang-tidy***, ***valgrind***. I am not a guru in the static analisis tools, so I use them in default configurations.
  You can improve the settings and options and share them with me. <br>
  For code coverage analisis ***lcov***, ***gcov*** and ***genhtml*** are needed.



<br> <br>



<h2> 
  :mag_right: &nbsp Something about code
</h2>

<h3> 
  :file_folder: &nbsp Structure:
</h3>

  <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master"><em> ./ </em></a> -  main part; <br>
  <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests"><em> auto_tests </em></a>  - tests; <br>
  <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/geom_lib"><em> geom_lib </em></a> - reading (parsing) utils and geometric algos; <br>
  Each (sub-)directory has its own ***makefile***  with ***make_reference.txt*** and subdirectories: ***header*** and ***src***. <br>
  I have added <a href="https://github.com/Shangin-Leonid/distance_between_polygons/tree/master/auto_tests/test_report"><em> results of code analisis </em></a> (static and runtime).

<h3> 
  :gear: &nbsp Algorithms:
</h3>

  The algorithms can be not the most effective. It is a training C-project. <br>
  Calculating distance between polygons ***P1*** and ***P2*** means taking ***min{ dist(S1, S2) | Si - side of Pi }***. So all sides are sorted out. I bet it is possible to avoid it.



<br> <br>



<h2> 
  :interrobang: &nbsp Feedback: problems, questions, suggestions...
</h2>

  You can find my contacts in <a href="https://github.com/Shangin-Leonid">my Github profile</a> . <br>
  I would be happy to have any feedback! Particularly if you have any idea of improving my project.

<br>
