# local_hook
We hook a function that exported from an ELF dynamic library via injecting its GOT entrance. Then we can accurately "locally hook" for any one of dynamic library or executable file because each of them has their own GOT sections.
