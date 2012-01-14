



Known Problems

1. Memory leaks with dlopen:
------------------------------------
When dynmaically loading link lybraries in Unix with dlopen, Valgrind might detect memory leaks with the function. This is a known problem with some distributions and systems having released fixes.
To manually suppress the leak, add the following to $PREFIX/lib/valgrind/default.supp ($PREFIX will normally be /usr):

{
  dl_catch_error-leak-1
  Memcheck:Leak
  ...
  fun:_dl_catch_error
  ...
}
{
  dl_catch_error-leak-2
  Memcheck:Leak
  ...
  fun:_dlerror_run
  ...
}
{
  dl_catch_error-leak-3
  Memcheck:Leak
  ...
  fun:_dlopen
  ...
}

2. Possible memory leaks with Qt:
-----------------------------------

Use these commands

valgrind --leak-check=full --show-reachable=yes --error-limit=no --gen-suppressions=all --log-file=minimalraw.log ./visore
cat ./minimalraw.log | gawk -f parse_valgrind_suppressions > minimal.supp

valgrind --tool=memcheck --leak-check=full --suppressions=minimal.supp ./visore
