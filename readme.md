*embsanity*
-----------

This project is me going insane with trying to hook together as many languages as I can into one executable. Also stored here are my misc notes on working with such crazy stuff. Note that this is mostly a play thing for myself and although I try and keep it working on all my machines that by no means means it will work anywhere else... too much platform specific (or install specific) code needed to get this many languages to play nice together.


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




*TODO::*
--------

* check that bootstrapping supports UTF-8 paths. (it *should* from what I can tell, but well...)