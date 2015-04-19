*embsanity*
-----------

This project is me going insane with trying to hook together as many languages as I can into one executable. Also stored here are my misc notes on working with such crazy stuff. Note that this is mostly a play thing for myself and although I try and keep it working on all my machines (linux) that by no means means it will work anywhere else... too much platform specific (or install specific) code needed to get this many languages to play nice together.


*status:*
---------

* Project failure as of 2014/4/18 for embedding pypy3, too many internal failures.
* 2014/4/21 Attempted again after reading a bit about `valgrind` and `gdb`, to see if I could track down and explain the issues.
    * success! able to run bootstrap.py (needed to patch `targetpypystandalone.py`, details follow. should be in mainline "soon")
    * The function `pypy_setup_home` in `pypy/goal/targetpypystandalone.py` sets up the `builtins` module as well, but oops! tried to call __builtins__ instead or some such. replace `import_ = space.getattr(space.getbuiltinmodule('__builtin__'), space.wrap('__import__'))` with `import_ = space.getattr(space.getbuiltinmodule('builtins'), space.wrap('__import__'))`
    * Another issue was found after this: for some reason the FD's of sys.stdin,sys.stdout,sys.stderr were all missing/bad. simple fix with os.fdopen() though inside the bootstrap.py
    * issue on my end was not properly null-terminating my `source` string. derp.
* 2014/5/31 Reworking project to now be about embedding *anything* instead of embedding just pypy. (because why not?)
* 2014/5/31 added java. Still unsure about a bit of the calling conventions used with it...
* 2015/4/19 touched this code again, was somehow relevant to work so some of the advanced things with mono/pypy/java are not going to be shown thanks to IP. Enough to be a proof of concept will still always be in this repository, and when my company is ok with enough time passed on our few key projects I can then open/re-implement some of them here. (Still open to pull requests, no issues if someone from the outside rediscover some of what our internal code does)



*TODO::*
--------

* check that bootstrapping supports UTF-8 paths. (it *should* from what I can tell, but well...)
* when Viridian Sciences gives the all clear
    * implement example interpreter callback chaining
    * implement threading
    * implement main event looping (choose VM to host, others delegate to that one)
    * I feel like I am forgetting some things...

*Compile/Run Instructions:*
-----------------------

1. Edit `c_src/makefile`, point to your local pypy3k, and java jvm.
2. on Debian based distros, you will need `libmono-2.0-dev openjdk-7-jdk`
3. the `runme.sh` file has some of the current make instructions (make the c project, then make the data dir)
4. if mono complains, 90% chance you messed up something related to your files not being found, class or namespaces being messed up for the c->c# or c#->c layer

*License*
---------

MIT, with some non-binding clauses of:

    * try to source to the primary author (Eric D Driggers, aka admalledd)
    * If this was found useful for you and/or your company and might have a interesting employment opportunity I (admalledd) would love to hear about it :D
